#include <Arduino.h>
#include <SPI.h>
#include <Ticker.h>
#include <rom/rtc.h>
#include <driver/rtc_io.h>
#include <SX126x-Arduino.h>
#include "Base64ren.h"

#include "settings-device.h"
#include "settings-scenario.h"
#include "roloran-tdeck-hal.h"
#include "roloran-lora.h"
#include "roloran-tdeck-serial.h"
#include "roloran-rdcp.h"

#define BUFFER_SIZE 512 // Maximum buffer size, e.g., for handling LoRa payloads

static RadioEvents_t RadioEvents;
hw_config            hwConfig;
uint8_t              cad_repeat = 0;
int64_t              tx_time = 0;
int64_t              rx_timestamp = 0;

volatile bool        has_message_to_send = false;
uint8_t              outgoing_message[BUFFER_SIZE];
uint8_t              outgoing_message_size = 0;
volatile bool        ongoing_transmission = false;

volatile bool        has_received_message = false;
volatile bool        do_start_send = false;
volatile bool        tx_finished = false;
int64_t              tx_wallclock_time = 0;
uint8_t              cad_stats_tries = 0;
bool                 has_timeout_tx = false;
bool                 has_timeout_rx = false;
bool                 has_error_rx = false;

uint8_t              receive_buffer[BUFFER_SIZE];
uint16_t             receive_buffer_length = 0;
int16_t              receive_rssi = 0;
uint8_t              receive_snr = 0;

bool                 new_message_available = false;

bool                 new_cad_result_available = false;
bool                 new_cad_result_busy = false;
bool                 has_txed = false;
int64_t              has_txed_timestamp = 0;

/**
 * @return int64_t Current monotonic clock time in milliseconds. 
*/
int64_t timeNow(void) { return my_millis(); }

int16_t getReceiveRSSI(void) { return receive_rssi; }

void lora_send_now_base64(String txstring)
{
  char buffer[BUFFER_SIZE];

  serial_writeln(txstring);

  String b64msg = txstring.substring(3);
  b64msg.toCharArray(buffer, BUFFER_SIZE);
  int b64msg_len = strlen(buffer);
  int decoded_length = Base64ren.decodedLength(buffer, b64msg_len);
  char decoded_string[decoded_length + 1];

  Base64ren.decode(decoded_string, buffer, b64msg_len);
	for (int i=0; i < decoded_length; i++) outgoing_message[i] = decoded_string[i];

	outgoing_message_size = decoded_length;
	has_message_to_send = true;

  serial_writeln("INFO: LoRa payload size to transmit is " + String(outgoing_message_size) + " bytes.");

  return;
}

void startReceiveMode(void)
{
  digitalWrite(TDECK_TFT_CS, HIGH);
  delay(1);
  digitalWrite(TDECK_RADIO_CS, LOW);
  delay(1);

	Radio.Standby();
	SX126xSetDioIrqParams(IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT,
		                    IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT,
	                      IRQ_RADIO_NONE, IRQ_RADIO_NONE);
	// Radio.SetRxDutyCycle(2 * 1024 * 1000 * 15.625, 10 * 1024 * 1000 * 15.625);
  Radio.Rx(0); // Start to receive without timeout.
	return;
}

void radio_setup(void)
{
  static bool radio_hardware_initialized = false;

  int8_t   PIN_LORA_TXEN = -1; // part of the pinout if TX-Enable is required
  int8_t   PIN_LORA_RXEN = -1; // part of the pinout if RX-Enable is required
  bool     LORA_USE_DIO2_ANT_SWITCH = false;
  bool     LORA_USE_DIO3_TCXO = true;
  bool     LORA_USE_DIO3_ANT_SWITCH = false;

	hwConfig.CHIP_TYPE           = SX1262_CHIP;
	hwConfig.PIN_LORA_RESET      = TDECK_RADIO_RST;
	hwConfig.PIN_LORA_NSS        = TDECK_RADIO_CS;
	hwConfig.PIN_LORA_SCLK       = TDECK_SPI_SCK;
	hwConfig.PIN_LORA_MISO       = TDECK_SPI_MISO;
	hwConfig.PIN_LORA_DIO_1      = TDECK_RADIO_DIO1;
	hwConfig.PIN_LORA_BUSY       = TDECK_RADIO_BUSY;
	hwConfig.PIN_LORA_MOSI       = TDECK_SPI_MOSI;
	hwConfig.RADIO_TXEN          = PIN_LORA_TXEN;
	hwConfig.RADIO_RXEN          = PIN_LORA_RXEN;
	hwConfig.USE_DIO2_ANT_SWITCH = LORA_USE_DIO2_ANT_SWITCH;
	hwConfig.USE_DIO3_TCXO       = LORA_USE_DIO3_TCXO;
	hwConfig.USE_DIO3_ANT_SWITCH = LORA_USE_DIO3_ANT_SWITCH;

  if (radio_hardware_initialized == false)
  {
	  lora_hardware_init(hwConfig);
    radio_hardware_initialized = true;
  }
	Radio.Init(&RadioEvents);

  uint32_t freq = getMyLoRaFrequency() * 1000 * 1000;

	Radio.SetChannel(freq);

  if (getMyLoRaSyncWord() != 0x34)
  {
    Radio.SetPublicNetwork(false);
  }
  else
  {
    Radio.SetPublicNetwork(true);
  }

  uint8_t TX_OUTPUT_POWER = getMyLoRaPower();
  uint8_t LORA_BANDWIDTH = 0;
  if (getMyLoRaBandwidth() == 250.0) LORA_BANDWIDTH = 1;
  if (getMyLoRaBandwidth() == 500.0) LORA_BANDWIDTH = 2;
  uint8_t LORA_SPREADING_FACTOR = getMyLoRaSpreadingFactor();
  uint8_t LORA_CODINGRATE = getMyLoRaCodingRate() - 4;
  uint8_t LORA_PREAMBLE_LENGTH = getMyLoRaPreambleLength();

  uint8_t  LORA_SYMBOL_TIMEOUT = 0;
  bool     LORA_FIX_LENGTH_PAYLOAD_ON = false;
  bool     LORA_IQ_INVERSION_ON = false;
  uint16_t TX_TIMEOUT_VALUE = 30000;

	Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
	                  LORA_SPREADING_FACTOR, LORA_CODINGRATE,
	                  LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
	                  true, 0, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

	Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
	                  LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
	                  LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
	                  0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

  RadioEvents.TxDone    = OnTxDone;
	RadioEvents.RxDone    = OnRxDone;
	RadioEvents.TxTimeout = OnTxTimeout;
	RadioEvents.RxTimeout = OnRxTimeout;
	RadioEvents.RxError   = OnRxError;
	RadioEvents.CadDone   = OnCadDone;

  startReceiveMode();

	return;
}

void radio_apply_new_configuration(void)
{
  digitalWrite(TDECK_TFT_CS, HIGH);
  delay(1);
  digitalWrite(TDECK_RADIO_CS, LOW);
  delay(1);
	Radio.Standby();
  radio_setup();
}

char current_rdcp_msg_base64[512];           // Base64-encoded version of currently processed message
char *get_current_rdcp_msg_base64(void) { return current_rdcp_msg_base64; }
void set_current_rdcp_msg_base64(char *m64)
{
  for (int i=0; i<strlen(m64)+1; i++) current_rdcp_msg_base64[i] = m64[i];
  return;
}

void radio_loop()
{
  char buf[BUFFER_SIZE];

  if (tx_finished == true)
  {
    tx_finished = false;
    ongoing_transmission = false;
    startReceiveMode();
	  serial_writeln("INFO: LoRa transmission successfully finished!");
	  serial_writeln("INFO: TX wallclock time was " + String(tx_wallclock_time) + " ms");
    rdcp_callback_txfin();
    has_txed = true;
    has_txed_timestamp = my_millis();
  }

  if (has_received_message == true)
  {
    has_received_message = false;
    new_message_available = true;

    startReceiveMode(); /* make sure we can receive further LoRa packets */

	  serial_writeln("INFO: LoRa Radio received packet.");
    snprintf(buf, BUFFER_SIZE-1, "RXMETA %d %d %d %.3f\0", receive_buffer_length, receive_rssi, receive_snr, getMyLoRaFrequency());
	  serial_writeln(String(buf));

    int encodedLength = Base64ren.encodedLength(receive_buffer_length);
    char encodedString[encodedLength + 1];
    Base64ren.encode(encodedString, (char *) receive_buffer, receive_buffer_length);
    serial_writeln("RX " + String(encodedString));
    snprintf(current_rdcp_msg_base64, 512, "%s\0", encodedString);
  }

  /* obsolete with scheduler
  if (do_start_send == true)
  {
    do_start_send = false;
    serial_writeln("INFO: CAD reports free channel on try #" + String(cad_stats_tries) + " (" + String(cad_stats_time) + " ms), starting to transmit (length " + String(outgoing_message_size) + " bytes)");
    tx_time = timeNow();
		Radio.Send(outgoing_message, outgoing_message_size);
  }
  */

  if (has_timeout_tx == true)
  {
    has_timeout_tx = false;
    serial_writeln("WARNING: LoRa TX_timeout. Retrying.");
    radio_apply_new_configuration(); // Re-initalize the radio
    startReceiveMode();
    has_message_to_send = true;
    ongoing_transmission = false;
  }

  if ((has_timeout_rx == true) || (has_error_rx == true))
  {
    has_timeout_rx = false;
    has_error_rx = false;
    serial_writeln("WARNING: LoRa RX_timeout.");
    radio_apply_new_configuration(); // Re-initalize the radio
    startReceiveMode();
  }

  if (new_cad_result_available == true)
  {
    new_cad_result_available = false;
    digitalWrite(TDECK_TFT_CS, HIGH);
    delay(1);
    digitalWrite(TDECK_RADIO_CS, LOW);
    delay(1);
    Radio.Standby();
    rdcp_callback_cad(new_cad_result_busy);
  }

	/* Check for a queued outgoing, non-empty LoRa packet. */
  /* obsolete with scheduler.
	if ((has_message_to_send == true) && (outgoing_message_size > 0) && (ongoing_transmission == false))
	{
    serial_writeln("INFO: Radio loop has message to send");
		cad_time = timeNow();
		has_message_to_send = false;
		ongoing_transmission = true;

		Radio.Standby();
    delay(5);
		Radio.SetCadParams(LORA_CAD_08_SYMBOL, getMyLoRaSpreadingFactor() + 13, 10, LORA_CAD_ONLY, 0);
    delay(5);
		Radio.StartCad(); // listen-before-talk / channel activity detection. Send message when channel is free.
	}
  */

	return;
}

bool lora_radio_send(uint8_t *data, uint8_t len)
{
  if (len == 0) return false;
  tx_time = timeNow();
  digitalWrite(TDECK_TFT_CS, HIGH);
  delay(1);
  digitalWrite(TDECK_RADIO_CS, LOW);
  delay(1);
  Radio.Send(data, len);
  return true;
}

void lora_radio_startcad(void)
{
  serial_writeln("INFO: Starting LoRa Channel Activity Detection");
  digitalWrite(TDECK_TFT_CS, HIGH);
  delay(1);
  digitalWrite(TDECK_RADIO_CS, LOW);
  delay(1);
	Radio.Standby();
  delay(1);
	Radio.SetCadParams(LORA_CAD_08_SYMBOL, getMyLoRaSpreadingFactor() + 13, 10, LORA_CAD_ONLY, 0);
  delay(1);
	Radio.StartCad();
  return;
}

void lora_radio_receivemode(void)
{
  startReceiveMode();
  return;
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

/*
 * Callback on TX done (outgoing LoRa packet was sent)
 */
void OnTxDone(void)
{
  tx_finished = true;
  tx_wallclock_time = timeNow() - tx_time;
	return;
}

/*
 * Callback on RX done (reception of new LoRa packet)
 */
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
  has_received_message = true;

  receive_buffer_length = size;
  receive_rssi = rssi;
  receive_snr = snr;

  memcpy(receive_buffer, payload, size);
  rx_timestamp = timeNow();

	return;
}

int64_t lora_get_rx_timestamp(void)
{
  return rx_timestamp;
}

/*
 * Callback on TX Timeout event
 */
void OnTxTimeout(void)
{
  has_timeout_tx = true;
	return;
}

/*
 * Callback on RX Timeout event
 */
void OnRxTimeout(void)
{
  has_timeout_rx = true;
	return;
}

/*
 * Callback on RX Error event
 */
void OnRxError(void)
{
  has_error_rx = true;
	return;
}

/*
 * Callback on Channel Activity Detection
 */
void OnCadDone(bool cadResult)
{
	cad_repeat++;
  new_cad_result_available = true;
  new_cad_result_busy = cadResult;
  return;
}

/* EOF */