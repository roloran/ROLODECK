#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>
#include <esp_timer.h>
#include <math.h>

#include "Base64ren.h"
#include "settings-device.h"
#include "settings-scenario.h"
#include "roloran-tdeck-hal.h"
#include "roloran-lora.h"
#include "roloran-tdeck-serial.h"
#include "roloran-rdcp.h"

#define BUFFER_SIZE 512
#define RADIO_SPI_FREQUENCY 2000000UL
#define RADIO_TX_TIMEOUT_MS 30000
#define TDECK_SX1262_TCXO_VOLTAGE 1.6f

enum RadioOperation : uint8_t
{
  RADIO_OPERATION_NONE,
  RADIO_OPERATION_RX,
  RADIO_OPERATION_TX,
  RADIO_OPERATION_CAD
};

struct RadioChannelConfig
{
  float frequency;
  float bandwidth;
  uint8_t spreading_factor;
  uint8_t coding_rate;
  uint8_t sync_word;
  int8_t power;
  uint16_t preamble_length;
};

static Module radio_module(TDECK_RADIO_CS, TDECK_RADIO_DIO1, TDECK_RADIO_RST,
                           TDECK_RADIO_BUSY, SPI,
                           SPISettings(RADIO_SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
static SX1262 radio(&radio_module);

static volatile bool radio_irq_pending = false;
static volatile int64_t radio_irq_timestamp = NO_TIMESTAMP;
static volatile RadioOperation radio_operation = RADIO_OPERATION_NONE;
static bool radio_initialized = false;
static int64_t tx_time = NO_TIMESTAMP;

static uint8_t receive_buffer[BUFFER_SIZE];
static uint16_t receive_buffer_length = 0;
static int16_t receive_rssi = 0;
static int8_t receive_snr = 0;
static int64_t rx_timestamp = NO_TIMESTAMP;
static volatile bool new_message_available = false;

volatile bool has_txed = false;
volatile int64_t has_txed_timestamp = NO_TIMESTAMP;

extern uint8_t current_channel;

static char current_rdcp_msg_base64[FATLEN];

static void IRAM_ATTR on_radio_irq(void)
{
  radio_irq_timestamp = esp_timer_get_time() / 1000;
  radio_irq_pending = true;
}

static void prepare_radio_operation(RadioOperation operation)
{
  noInterrupts();
  radio_irq_pending = false;
  radio_irq_timestamp = NO_TIMESTAMP;
  radio_operation = operation;
  interrupts();
}

static bool take_radio_irq(RadioOperation *operation, int64_t *timestamp)
{
  bool pending;
  noInterrupts();
  pending = radio_irq_pending;
  if (pending)
  {
    radio_irq_pending = false;
    *operation = radio_operation;
    *timestamp = radio_irq_timestamp;
    radio_operation = RADIO_OPERATION_NONE;
  }
  interrupts();
  return pending;
}

static void log_radiolib_error(const char *operation, int16_t state)
{
  serial_writeln("ERROR: RadioLib " + String(operation) + " failed with code " + String(state));
}

static RadioChannelConfig get_channel_config(uint8_t channel)
{
  RadioChannelConfig config;
  if (channel == CHANNEL868MG)
  {
    config.frequency = getMyLoRaFrequencyTX();
    config.bandwidth = getMyLoRaBandwidthTX();
    config.spreading_factor = getMyLoRaSpreadingFactorTX();
    config.coding_rate = getMyLoRaCodingRateTX();
    config.sync_word = getMyLoRaSyncWordTX();
    config.power = getMyLoRaPowerTX();
    config.preamble_length = getMyLoRaPreambleLengthTX();
  }
  else
  {
    config.frequency = getMyLoRaFrequency();
    config.bandwidth = getMyLoRaBandwidth();
    config.spreading_factor = getMyLoRaSpreadingFactor();
    config.coding_rate = getMyLoRaCodingRate();
    config.sync_word = getMyLoRaSyncWord();
    config.power = getMyLoRaPower();
    config.preamble_length = getMyLoRaPreambleLength();
  }
  return config;
}

static int16_t configure_radio(const RadioChannelConfig &config)
{
  int16_t state;
  tdeck_spi_lock();
  state = radio.standby();
  if (state == RADIOLIB_ERR_NONE) state = radio.setFrequency(config.frequency);
  if (state == RADIOLIB_ERR_NONE) state = radio.setBandwidth(config.bandwidth);
  if (state == RADIOLIB_ERR_NONE) state = radio.setSpreadingFactor(config.spreading_factor);
  if (state == RADIOLIB_ERR_NONE) state = radio.setCodingRate(config.coding_rate);
  if (state == RADIOLIB_ERR_NONE) state = radio.setSyncWord(config.sync_word);
  if (state == RADIOLIB_ERR_NONE) state = radio.setOutputPower(config.power);
  if (state == RADIOLIB_ERR_NONE) state = radio.setPreambleLength(config.preamble_length);
  if (state == RADIOLIB_ERR_NONE) state = radio.explicitHeader();
  if (state == RADIOLIB_ERR_NONE) state = radio.setCRC(2);
  if (state == RADIOLIB_ERR_NONE) state = radio.invertIQ(false);
  if (state == RADIOLIB_ERR_NONE) state = radio.autoLDRO();
  tdeck_spi_unlock();
  return state;
}

int16_t getReceiveRSSI(void) { return receive_rssi; }
int8_t getReceiveSNR(void) { return receive_snr; }
bool radio_is_initialized(void) { return radio_initialized; }

char *get_current_rdcp_msg_base64(void) { return current_rdcp_msg_base64; }

void set_current_rdcp_msg_base64(char *m64)
{
  snprintf(current_rdcp_msg_base64, FATLEN, "%s", m64);
}

void startReceiveMode(void)
{
  if (!radio_initialized) return;

  prepare_radio_operation(RADIO_OPERATION_RX);
  tdeck_spi_lock();
  int16_t state = radio.startReceive();
  tdeck_spi_unlock();

  if (state != RADIOLIB_ERR_NONE)
  {
    prepare_radio_operation(RADIO_OPERATION_NONE);
    radio_initialized = false;
    log_radiolib_error("startReceive", state);
  }
}

void radio_setup(void)
{
  serial_writeln("INFO: Setting up SX1262 with RadioLib, switching to CHANNEL868DA");

  prepare_radio_operation(RADIO_OPERATION_NONE);
  radio_initialized = false;
  RadioChannelConfig config = get_channel_config(CHANNEL868DA);
  ConfigLoRa_t radio_config;
  radio_config.frequency = config.frequency;
  radio_config.bandwidth = config.bandwidth;
  radio_config.spreadingFactor = config.spreading_factor;
  radio_config.codingRate = config.coding_rate;
  radio_config.syncWord = config.sync_word;
  radio_config.power = config.power;
  radio_config.preambleLength = config.preamble_length;

  // LilyGo T-Deck Plus defaults: DIO3-controlled 1.6 V TCXO, DC-DC regulator,
  // and DIO2-controlled RF switch (the latter is enabled by RadioLib begin()).
  radio.tcxoVoltage = TDECK_SX1262_TCXO_VOLTAGE;
  radio.useRegulatorLDO = false;

  tdeck_spi_lock();
  int16_t state = radio.begin(radio_config);
  if (state == RADIOLIB_ERR_NONE) state = radio.explicitHeader();
  if (state == RADIOLIB_ERR_NONE) state = radio.setCRC(2);
  if (state == RADIOLIB_ERR_NONE) state = radio.invertIQ(false);
  if (state == RADIOLIB_ERR_NONE) state = radio.autoLDRO();
  tdeck_spi_unlock();

  if (state != RADIOLIB_ERR_NONE)
  {
    log_radiolib_error("begin", state);
    return;
  }

  radio.setDio1Action(on_radio_irq);
  radio_initialized = true;
  current_channel = CHANNEL868DA;
  startReceiveMode();
}

void radio_apply_new_configuration(void)
{
  radio_setup();
}

void radio_switch_channel(uint8_t channel)
{
  if ((channel != CHANNEL868DA) && (channel != CHANNEL868MG))
  {
    serial_writeln("WARNING: Invalid LoRa radio channel switch");
    return;
  }

  if (!radio_initialized)
  {
    radio_setup();
    if (!radio_initialized) return;
  }

  serial_writeln(channel == CHANNEL868DA
                   ? "INFO: Switching radio to CHANNEL868DA"
                   : "INFO: Switching radio to CHANNEL868MG");

  RadioChannelConfig config = get_channel_config(channel);
  prepare_radio_operation(RADIO_OPERATION_NONE);
  int16_t state = configure_radio(config);
  if (state != RADIOLIB_ERR_NONE)
  {
    log_radiolib_error("channel configuration", state);
    radio_initialized = false;
    radio_setup();
    return;
  }

  current_channel = channel;
  startReceiveMode();

  char radio_info[INFOLEN];
  snprintf(radio_info, INFOLEN,
           "INFO: Channel %d parameters are F %.3f MHz SW %02X PW %d dBm BW %.1f kHz SF %d CR 4/%d PL %d",
           channel, config.frequency, config.sync_word, config.power, config.bandwidth,
           config.spreading_factor, config.coding_rate, config.preamble_length);
  serial_writeln(radio_info);
}

static void process_received_packet(int64_t interrupt_timestamp)
{
  size_t packet_length;
  float packet_rssi;
  float packet_snr;

  tdeck_spi_lock();
  packet_length = radio.getPacketLength();
  packet_rssi = radio.getRSSI();
  packet_snr = radio.getSNR();
  int16_t state = radio.readData(receive_buffer, packet_length);
  tdeck_spi_unlock();

  startReceiveMode();

  if (state == RADIOLIB_ERR_CRC_MISMATCH)
  {
    serial_writeln("WARNING: RadioLib rejected LoRa packet with invalid PHY CRC");
    return;
  }
  if (state != RADIOLIB_ERR_NONE)
  {
    log_radiolib_error("readData", state);
    return;
  }
  if ((packet_length == 0) || (packet_length > RDCP_MAX_PAYLOAD_SIZE_LORA))
  {
    serial_writeln("WARNING: Dropping LoRa packet with unsupported length " + String(packet_length));
    return;
  }

  receive_buffer_length = packet_length;
  receive_rssi = (int16_t)lroundf(packet_rssi);
  receive_snr = (int8_t)lroundf(packet_snr);
  rx_timestamp = interrupt_timestamp;
  new_message_available = true;

  serial_writeln("INFO: LoRa Radio received packet.");
  char buf[BUFFER_SIZE];
  snprintf(buf, BUFFER_SIZE, "RXMETA %u %d %d %.3f", receive_buffer_length,
           receive_rssi, receive_snr,
           current_channel == CHANNEL868DA ? getMyLoRaFrequency() : getMyLoRaFrequencyTX());
  serial_writeln(String(buf));

  int encoded_length = Base64ren.encodedLength(receive_buffer_length);
  char encoded_string[encoded_length + 1];
  Base64ren.encode(encoded_string, (char *)receive_buffer, receive_buffer_length);
  serial_writeln("RX " + String(encoded_string));
  snprintf(current_rdcp_msg_base64, FATLEN, "%s", encoded_string);
  set_gui_needs_screen_refresh(true);
}

static void process_finished_transmission(void)
{
  tdeck_spi_lock();
  int16_t state = radio.finishTransmit();
  tdeck_spi_unlock();

  int64_t tx_wallclock_time = my_millis() - tx_time;
  startReceiveMode();

  if (state != RADIOLIB_ERR_NONE)
  {
    log_radiolib_error("finishTransmit", state);
    rdcp_callback_radio_failure("TX completion");
    return;
  }

  serial_writeln("INFO: LoRa transmission successfully finished!");
  serial_writeln("INFO: TX wallclock time was " + String(tx_wallclock_time) + " ms");
  rdcp_callback_txfin();
  has_txed = true;
  has_txed_timestamp = my_millis();
  set_gui_needs_screen_refresh(true);
}

static void process_cad_result(void)
{
  tdeck_spi_lock();
  int16_t state = radio.getChannelScanResult();
  tdeck_spi_unlock();

  startReceiveMode();

  if ((state != RADIOLIB_LORA_DETECTED) && (state != RADIOLIB_CHANNEL_FREE))
  {
    log_radiolib_error("getChannelScanResult", state);
    rdcp_callback_radio_failure("CAD result");
    return;
  }

  rdcp_callback_cad(state == RADIOLIB_LORA_DETECTED);
  set_gui_needs_screen_refresh(true);
}

void radio_loop(void)
{
  RadioOperation completed_operation = RADIO_OPERATION_NONE;
  int64_t interrupt_timestamp = NO_TIMESTAMP;
  if (take_radio_irq(&completed_operation, &interrupt_timestamp))
  {
    if (completed_operation == RADIO_OPERATION_RX)
    {
      process_received_packet(interrupt_timestamp);
    }
    else if (completed_operation == RADIO_OPERATION_TX)
    {
      process_finished_transmission();
    }
    else if (completed_operation == RADIO_OPERATION_CAD)
    {
      process_cad_result();
    }
    else
    {
      serial_writeln("WARNING: Ignoring unexpected SX1262 DIO1 interrupt");
      startReceiveMode();
    }
  }

  if ((radio_operation == RADIO_OPERATION_TX) &&
      (my_millis() - tx_time >= RADIO_TX_TIMEOUT_MS))
  {
    prepare_radio_operation(RADIO_OPERATION_NONE);
    tdeck_spi_lock();
    int16_t state = radio.finishTransmit();
    tdeck_spi_unlock();
    if (state != RADIOLIB_ERR_NONE) log_radiolib_error("TX timeout cleanup", state);
    serial_writeln("WARNING: LoRa TX timeout; returning the message to the RDCP queue");
    startReceiveMode();
    rdcp_callback_radio_failure("TX timeout");
  }
}

bool lora_radio_send(uint8_t *data, uint8_t len)
{
  if ((len == 0) || (len > RDCP_MAX_PAYLOAD_SIZE_LORA)) return false;
  if (!radio_initialized)
  {
    serial_writeln("ERROR: Cannot transmit because the SX1262 is not initialized");
    rdcp_callback_radio_failure("radio unavailable");
    return false;
  }

  tx_time = my_millis();
  prepare_radio_operation(RADIO_OPERATION_TX);
  tdeck_spi_lock();
  int16_t state = radio.startTransmit(data, len);
  tdeck_spi_unlock();

  if (state != RADIOLIB_ERR_NONE)
  {
    prepare_radio_operation(RADIO_OPERATION_NONE);
    log_radiolib_error("startTransmit", state);
    startReceiveMode();
    rdcp_callback_radio_failure("TX start");
    return false;
  }
  return true;
}

void lora_radio_startcad(uint8_t channel)
{
  serial_writeln("INFO: Starting LoRa Channel Activity Detection");
  radio_switch_channel(channel);
  if (!radio_initialized || (current_channel != channel))
  {
    rdcp_callback_radio_failure("CAD channel setup");
    return;
  }

  RadioChannelConfig config = get_channel_config(channel);
  ChannelScanConfig_t cad_config;
  cad_config.cad.symNum = RADIOLIB_SX126X_CAD_ON_8_SYMB;
  cad_config.cad.detPeak = config.spreading_factor + 13;
  cad_config.cad.detMin = 10;
  cad_config.cad.exitMode = RADIOLIB_SX126X_CAD_GOTO_STDBY;
  cad_config.cad.timeout = 0;
  cad_config.cad.irqFlags = RADIOLIB_IRQ_CAD_DEFAULT_FLAGS;
  cad_config.cad.irqMask = RADIOLIB_IRQ_CAD_DEFAULT_MASK;

  prepare_radio_operation(RADIO_OPERATION_CAD);
  tdeck_spi_lock();
  int16_t state = radio.startChannelScan(cad_config);
  tdeck_spi_unlock();

  if (state != RADIOLIB_ERR_NONE)
  {
    prepare_radio_operation(RADIO_OPERATION_NONE);
    log_radiolib_error("startChannelScan", state);
    startReceiveMode();
    rdcp_callback_radio_failure("CAD start");
  }
}

void lora_radio_receivemode(void)
{
  startReceiveMode();
}

bool lora_has_new_message(void)
{
  return new_message_available;
}

uint16_t lora_copy_received_message(uint8_t *destination)
{
  new_message_available = false;
  memcpy(destination, receive_buffer, receive_buffer_length);
  return receive_buffer_length;
}

int64_t lora_get_rx_timestamp(void)
{
  return rx_timestamp;
}

/* EOF */
