#include "roloran-rdcp.h"

#include <Arduino.h>

#include "roloran-tdeck-serial.h"       // Serial/UART communication
#include "roloran-tdeck-hal.h"          // T-Deck hardware-related functions
#include "roloran-tdeck-persistence.h"  // File-system / persistence related functions
#include "settings-device.h"            // Device-specific settings
#include "settings-scenario.h"          // Scenario-specific settings
#include "roloran-lora.h"               // LoRa-specific functions
#include "roloran-board.h"              // Message Board functions

#include "ui.h"                         // GUI as exported from SquareLine Studio
#include "tdeck_gui.h"                  // GUI helper functions
#include <Hash.h>
#include <Crypto.h>
#include <FFat.h>

#define BUFFER_SIZE 512
#define FILENAME_CIRE "/cire"

struct rdcp_message rdcp_msg_in, rdcp_msg_out; // Global incoming and outgoing RDCP Message
bool   rdcp_ignore_duplicates = false;         // Do ignore RDCP Message duplicates currently?
struct rdcp_dup_table dupe_table;              // One global RDCP Message Duplicate Table

int64_t CFEst = my_millis();       // Current Channel Free Estimator
int64_t CFEst_old = my_millis();   // Previous value of CFEst

struct txqueue txq;       // One global TX Queue
struct txaheadqueue txaq; // One global TX Ahead Queue

int tx_ongoing = -1;      // Index of TXQ entry currently up for transmission
int64_t tx_process_start = 0;
int retransmission_count = 0;
int64_t last_tx_activity = 0;

int32_t bad_crc_counter = 0;

int64_t rdcp_get_channel_free_estimation(void)
{
  return CFEst;
}

bool rdcp_set_channel_free_estimation(int64_t new_value)
{
  CFEst_old = CFEst;
  CFEst = new_value;
  return rdcp_txqueue_reschedule(0);
}

bool rdcp_update_channel_free_estimation(int64_t new_value)
{
  if (rdcp_get_channel_free_estimation() < new_value)
  {
    rdcp_set_channel_free_estimation(new_value);
    return true;
  }
  return false;
}

bool rdcp_toggle_ignore_duplicates(void)
{
  if (rdcp_ignore_duplicates == true)
  {
    rdcp_ignore_duplicates = false;
  }
  else
  {
    rdcp_ignore_duplicates = true;
  }
  return rdcp_ignore_duplicates;
}

uint16_t crc16(uint8_t *data, uint16_t len)
{
    uint16_t lookup[] = {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108,
        0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF, 0x1231, 0x0210,
        0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B,
        0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE, 0x2462, 0x3443, 0x0420, 0x1401,
        0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE,
        0xF5CF, 0xC5AC, 0xD58D, 0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6,
        0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D,
        0xC7BC, 0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B, 0x5AF5,
        0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC,
        0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 0x6CA6, 0x7C87, 0x4CE4,
        0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD,
        0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13,
        0x2E32, 0x1E51, 0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A,
        0x9F59, 0x8F78, 0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E,
        0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1,
        0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256, 0xB5EA, 0xA5CB,
        0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0,
        0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xA7DB, 0xB7FA, 0x8799, 0x97B8,
        0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657,
        0x7676, 0x4615, 0x5634, 0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9,
        0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882,
        0x28A3, 0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
        0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92, 0xFD2E,
        0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07,
        0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1, 0xEF1F, 0xFF3E, 0xCF5D,
        0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74,
        0x2E93, 0x3EB2, 0x0ED1, 0x1EF0};

    uint16_t crc = 0xFFFF;
    for (int i=0; i < len; i++)
    {
        uint8_t b = data[i];
        crc = (crc << 8) ^ lookup[(crc >> 8) ^ b];
        crc &= 0xFFFF;
    }
    return crc;
}

bool rdcp_check_crc_in(uint8_t real_packet_length)
{
  uint8_t data_for_crc[256];

  /* Copy RDCP header and payload into data structure for CRC calculation */
  memcpy(&data_for_crc, &rdcp_msg_in.header, RDCP_HEADER_SIZE - 2);
  for (int i=0; i < real_packet_length - RDCP_HEADER_SIZE; i++)
  {
    data_for_crc[i + RDCP_HEADER_SIZE - 2] = rdcp_msg_in.payload.data[i];
  }

  /* Calculate and check CRC */
  uint16_t actual_crc = crc16(data_for_crc, real_packet_length - 2);

  if (actual_crc == rdcp_msg_in.header.checksum)
  {
    return true;
  }

  return false;
}

bool rdcp_mg_process_rxed_lora_packet(uint8_t *lora_packet, uint16_t lora_packet_length, int64_t lora_packet_timestamp)
{
  /* Check whether it could be an RDCP message at all */
  if (lora_packet_length < RDCP_HEADER_SIZE)
  {
    serial_writeln("INFO: LoRa packet too small - not an RDCP message, not processing");
    // NB: Received non-RDCP LoRa packets have no influence on CFEst, so there is nothing else to do.
    return false;
  }

  /* Copy the message to process into the rdcp_msg_in data structure */
  memcpy(&rdcp_msg_in.header, lora_packet, RDCP_HEADER_SIZE);
  memcpy(&rdcp_msg_in.payload.data, lora_packet + RDCP_HEADER_SIZE, lora_packet_length - RDCP_HEADER_SIZE);

  /* Verify the CRC-16 checksum */
  if (!rdcp_check_crc_in(lora_packet_length))
  {
    serial_writeln("INFO: RDCP checksum mismatch - not processing");
    // NB: Any RDCP Header or RDCP Payload field may have been corrupted,
    //     so we do not process anything further, including updates to CFEst.
    bad_crc_counter++;
    if (bad_crc_counter % 10 == 0)
    {
      serial_writeln("WARNING: Bad CRC counter exceeded threshold - consider additional countermeasures!");
      radio_apply_new_configuration();
    }
    return false;
  }

  /* Stop ongoing sending attempts currently in a CAD-induced pause */
  for (int i=0; i < MAX_TXQUEUE_ENTRIES; i++) txq.entries[i].cad_retry = 0;
  if (tx_ongoing != -1)
  {
    serial_writeln("INFO: Freezing current transmission due to LoRa packet reception");
    txq.entries[tx_ongoing].in_process = false;
    tx_ongoing = -1;
  }

  rdcp_update_channel_free_estimator_rx();

  /* Use getReceiveRSSI if Sender is a DA to update the roaming table. */
  int16_t rssi = getReceiveRSSI();
  if ((rdcp_msg_in.header.sender >= 0x0200) && (rdcp_msg_in.header.sender < 0x0300))
  {
    updateRoamingTable(rdcp_msg_in.header.sender, rssi);
  }

  /* Check for RDCP message duplicate */
  if (rdcp_check_duplicate_message(rdcp_msg_in.header.origin, rdcp_msg_in.header.sequence_number) == true)
  {
    if (rdcp_ignore_duplicates == false)
    {
      char buf[256];
      snprintf(buf, 256, "INFO: RDCP message (origin %04X, seqnr %04X) is duplicate - not processing", rdcp_msg_in.header.origin, rdcp_msg_in.header.sequence_number);
      serial_writeln(buf);
      /*
       * NB: If we are a designated Relay for this RDCP Message, we have to relay it without processing its content again.
       *     This is not implemented here for T-Deck MGs, because even if we operate as a mobile 868 MHz Relay, we can
       *     arrange for that when first receiving the RDCP Message (there are no "3 waves of relays" on the 868 MHz channel).
       */
      return false;
    }
  }

  rdcp_mg_process_incoming_message();

  return true;
}

void rdcp_reset_duplicate_message_table()
{
  dupe_table.num_entries = 0;
  for (int i=0; i != 256; i++)
  {
    dupe_table.tableentry[i].origin = 0;
    dupe_table.tableentry[i].sequence_number = 0;
    dupe_table.tableentry[i].last_seen = 0;
  }
  return;
}

bool rdcp_check_duplicate_message(uint16_t origin, uint16_t sequence_number)
{
  int pos = -1;
  for (int i=0; i != dupe_table.num_entries; i++)
  {
    if (dupe_table.tableentry[i].origin == origin) pos = i;
  }

  if (pos == -1) // new entry
  {
    if (dupe_table.num_entries > 254)
    {
      Serial.println("WARNING: RDCP duplicate table overflow - increase size!");
      return false;
    }
    dupe_table.tableentry[dupe_table.num_entries].origin = origin;
    dupe_table.tableentry[dupe_table.num_entries].sequence_number = sequence_number;
    dupe_table.tableentry[dupe_table.num_entries].last_seen = my_millis();
    dupe_table.num_entries++;
    return false;
  }
  else
  {
    dupe_table.tableentry[pos].last_seen = my_millis();
    if (dupe_table.tableentry[pos].sequence_number < sequence_number)
    { // update highest sequence number
      dupe_table.tableentry[pos].sequence_number = sequence_number;
      return false;
    }
    else
    { // duplicate found
      return true;
    }
  }
  return false;
}

uint16_t airtime_in_ms(uint8_t payload_size)
{
  uint16_t time_for_packet = 0;

  uint32_t bandwidth_in_hz = (uint32_t) getMyLoRaBandwidth() * 1000;
  uint8_t  low_data_rate_optimization = 1;
  uint8_t  implicit_header_mode = 0;
  uint8_t  coding_rate = getMyLoRaCodingRate() - 4;
  uint8_t  SF = getMyLoRaSpreadingFactor();

  double time_per_symbol = pow(2, SF) / bandwidth_in_hz;

  /* Calculate the airtime for the preamble */
  uint8_t number_of_preamble_symbols = getMyLoRaPreambleLength();
  double time_for_preamble = (number_of_preamble_symbols + 4.25) * time_per_symbol;

  /* Calculate the airtime for the payload */
  double number_of_payload_symbols = 8 + max((coding_rate + 4)*ceil((8 * payload_size - 4 * SF + 28 + 16 - 20 * implicit_header_mode) / (4*(SF - 2*low_data_rate_optimization))), 0.0);
  double time_for_payload = number_of_payload_symbols * time_per_symbol;

  /* Sum it up, converting from seconds to milliseconds and from Double to Int */
  time_for_packet = (uint16_t) (1000 * (time_for_preamble + time_for_payload));

  return time_for_packet;
}

void rdcp_update_channel_free_estimator_rx(void)
{
  uint16_t airtime = airtime_in_ms(RDCP_HEADER_SIZE + rdcp_msg_in.header.rdcp_payload_length);
  uint16_t airtime_with_buffer = airtime + 1000;

  uint32_t remaining_current_sender_time = airtime_with_buffer * rdcp_msg_in.header.counter;

  uint8_t nrt = 0;
  uint8_t mt = rdcp_msg_in.header.message_type;
  if ( (mt == RDCP_MSGTYPE_INFRASTRUCTURE_RESET) || (mt == RDCP_MSGTYPE_ACK) ||
       (mt == RDCP_MSGTYPE_RESET_ALL_ANNOUNCEMENTS) ) nrt = 2;
  if ( (mt == RDCP_MSGTYPE_OFFICIAL_ANNOUNCEMENT) || (mt == RDCP_MSGTYPE_CITIZEN_REPORT) ||
       (mt == RDCP_MSGTYPE_SIGNATURE) ) nrt = 4;

  uint32_t timeslot_duration = (nrt+1) * airtime_with_buffer;

  uint8_t future_timeslots = 0;

  if ((rdcp_msg_in.header.sender < 0x0300) && (rdcp_msg_in.header.sender >= 0x0100))
  { // DA or BBK sending
    /* Previously used values for a 14 timeslots propagation cycle: */
    /*
    if ((rdcp_msg_in.header.relay1 & 0x0F) == 0x00) future_timeslots = 13;
    if ((rdcp_msg_in.header.relay1 & 0x0F) == 0x02) future_timeslots = 12;
    if ((rdcp_msg_in.header.relay1 & 0x0F) == 0x04) future_timeslots = 11;
    if ((rdcp_msg_in.header.relay1 & 0x0F) == 0x06) future_timeslots = 10;
    if (rdcp_msg_in.header.relay1 == 0xF8) future_timeslots = 9;
    if (rdcp_msg_in.header.relay1 == 0xF7) future_timeslots = 8;
    if (rdcp_msg_in.header.relay1 == 0xF6) future_timeslots = 7;
    if (rdcp_msg_in.header.relay1 == 0xF5) future_timeslots = 6;
    if (rdcp_msg_in.header.relay1 == 0xF4) future_timeslots = 5;
    if (rdcp_msg_in.header.relay1 == 0xF3) future_timeslots = 4;
    if (rdcp_msg_in.header.relay1 == 0xF2) future_timeslots = 3;
    if (rdcp_msg_in.header.relay1 == 0xF1) future_timeslots = 2;
    if (rdcp_msg_in.header.relay1 == 0xF0) future_timeslots = 1;
    if (rdcp_msg_in.header.relay1 == 0xEE) future_timeslots = 0;
    */
    /* Updated version for a 9 timeslots propagation cycle: */
    if ((rdcp_msg_in.header.relay1 & 0x0F) == 0x00) future_timeslots = 8;
    if ((rdcp_msg_in.header.relay1 & 0x0F) == 0x02) future_timeslots = 7;
    if ((rdcp_msg_in.header.relay1 & 0x0F) == 0x03) future_timeslots = 4; // Third Hop 1 assigned with Delay 3
    if ((rdcp_msg_in.header.relay2 & 0x0F) == 0x04) future_timeslots = 6; // Second Hop 4 assigned with Delay 4
    if (rdcp_msg_in.header.relay1 == 0xE4) future_timeslots = 5;
    if (rdcp_msg_in.header.relay1 == 0xE2) future_timeslots = 3;
    if (rdcp_msg_in.header.relay1 == 0xE1) future_timeslots = 2;
    if (rdcp_msg_in.header.relay1 == 0xE0) future_timeslots = 1;
    if (rdcp_msg_in.header.relay1 == 0xEE) future_timeslots = 0;
  }
  else
  { // other device sending, not leading to relay on same channel
    future_timeslots = 0;
  }

  uint32_t channel_free_after = remaining_current_sender_time + future_timeslots * timeslot_duration;
  int64_t channel_free_at = my_millis() + channel_free_after;

  rdcp_update_channel_free_estimation(channel_free_at);

  char buf[256];
  snprintf(buf, 256, "INFO: Channel free estimation: +%zu ms, @%llu ms (airtime %u ms, retrans %zu ms, timeslot %zu ms, %d fut ts)", channel_free_after, channel_free_at, airtime, remaining_current_sender_time, timeslot_duration, future_timeslots);
  serial_writeln(buf);

  return;
}

void rdcp_mg_print_test_message(void)
{
  char content[256];
  char buf [512];
  int i=0;
  for (i=0; i < rdcp_msg_in.header.rdcp_payload_length; i++) content[i] = rdcp_msg_in.payload.data[i];
  content[i] = '\0';
  snprintf(buf, 512, "INFO: RDCP Test Message from %04X to %04X -> %s", rdcp_msg_in.header.origin, rdcp_msg_in.header.destination, content);
  serial_writeln(buf);
  return;
}

void rdcp_mg_fill_outgoing_header(struct rdcp_message *rm)
{
  uint16_t me = getMyRDCPAddress();

  rm->header.origin = me;
  rm->header.sender = me;

  uint8_t mt = rm->header.message_type;
  rm->header.counter = 0;
  if ( (mt == RDCP_MSGTYPE_INFRASTRUCTURE_RESET) || (mt == RDCP_MSGTYPE_ACK) ||
       (mt == RDCP_MSGTYPE_RESET_ALL_ANNOUNCEMENTS) ) rm->header.counter = 2;
  if ( (mt == RDCP_MSGTYPE_OFFICIAL_ANNOUNCEMENT) || (mt == RDCP_MSGTYPE_CITIZEN_REPORT) ||
       (mt == RDCP_MSGTYPE_SIGNATURE) ) rm->header.counter = 4;

  rm->header.sequence_number = get_next_rdcp_sequence_number(me);

  uint16_t primary_ep = getSuggestedRelay(0); //getEntryPoint(0);
  rm->header.relay1 = (uint8_t) ((primary_ep & 0x000F) * 8) + (uint8_t) 0x0;
  rm->header.relay2 = 0xEE;
  rm->header.relay3 = 0xEE;

  uint8_t data_for_crc[256];
  memcpy(&data_for_crc, &rm->header, RDCP_HEADER_SIZE - 2);
  for (int i=0; i < rm->header.rdcp_payload_length; i++)
  {
    data_for_crc[i + RDCP_HEADER_SIZE - 2] = rm->payload.data[i];
  }
  uint16_t actual_crc = crc16(data_for_crc, RDCP_HEADER_SIZE - 2 + rm->header.rdcp_payload_length);
  rm->header.checksum = actual_crc;

  return;
}

void rdcp_mg_send_echo_response(uint16_t desta)
{
  struct rdcp_message rm;
  uint8_t data[256];

  char info[256];
  snprintf(info, 256, "INFO: Responding to RDCP Echo Request by %04X with RDCP Echo Response", desta);
  serial_writeln(info);

  rm.header.destination  = desta;
  rm.header.message_type = RDCP_MSGTYPE_ECHO_RESPONSE;
  rm.header.rdcp_payload_length = 0;

  rdcp_mg_fill_outgoing_header(&rm);

  memcpy(&data, &rm.header, RDCP_HEADER_SIZE);
  rdcp_txqueue_add(data, RDCP_HEADER_SIZE, NOTIMPORTANT, NOTFORCEDTX, TX_CALLBACK_NONE, 0);
  return;
}

void rdcp_mg_send_echo_request(uint16_t desta)
{
  struct rdcp_message rm;
  uint8_t data[256];

  char info[256];
  snprintf(info, 256, "INFO: Sending RDCP Echo Request to %04X", desta);
  serial_writeln(info);

  rm.header.destination  = desta;
  rm.header.message_type = RDCP_MSGTYPE_ECHO_REQUEST;
  rm.header.rdcp_payload_length = 0;

  rdcp_mg_fill_outgoing_header(&rm);

  memcpy(&data, &rm.header, RDCP_HEADER_SIZE);
  rdcp_txqueue_add(data, RDCP_HEADER_SIZE, NOTIMPORTANT, NOTFORCEDTX, TX_CALLBACK_NONE, 0);
  return;
}

void rdcp_mg_send_test_message(uint16_t desta, String m)
{
  struct rdcp_message rm;
  uint8_t data[256];

  char info[256];
  snprintf(info, 256, "INFO: Sending RDCP Test message to %04X", desta);
  serial_writeln(info);

  rm.header.destination  = desta;
  rm.header.message_type = RDCP_MSGTYPE_TEST;
  rm.header.rdcp_payload_length = m.length();

  char buffer[200];
  m.toCharArray(buffer, 200);
  for (int i=0; i<m.length(); i++) rm.payload.data[i] = buffer[i];

  rdcp_mg_fill_outgoing_header(&rm);

  memcpy(&data, &rm.header, RDCP_HEADER_SIZE);
  for (int i=0; i<rm.header.rdcp_payload_length; i++) data[i + RDCP_HEADER_SIZE] = rm.payload.data[i];
  rdcp_txqueue_add(data, RDCP_HEADER_SIZE + rm.header.rdcp_payload_length, NOTIMPORTANT, NOTFORCEDTX, TX_CALLBACK_NONE, 0);

  return;
}

void get_inline_hash(struct rdcp_message *m, uint8_t payloadprefixlength, uint8_t *hashtarget)
{
  /* Prepare the data to be signed */
  uint8_t data_to_sign[256];
  data_to_sign[0] = m->header.origin % 256;
  data_to_sign[1] = m->header.origin / 256;
  data_to_sign[2] = m->header.sequence_number % 256;
  data_to_sign[3] = m->header.sequence_number / 256;
  data_to_sign[4] = m->header.destination % 256;
  data_to_sign[5] = m->header.destination / 256;
  data_to_sign[6] = m->header.message_type;
  data_to_sign[7] = m->header.rdcp_payload_length;
  for (int i=0; i<payloadprefixlength; i++) data_to_sign[8+i] = m->payload.data[i];
  uint8_t data_to_sign_length = 8 + payloadprefixlength;

  /* Get the SHA-256 hash for the data */
  SHA256 h = SHA256();
  h.reset();
  h.update(data_to_sign, data_to_sign_length);
  uint8_t sha[32];
  h.finalize(sha, 32);

  /* Copy result to target buffer */
  for (int i=0; i<32; i++) hashtarget[i] = sha[i];

  return;
}

void rdcp_send_timestamp(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t status)
{
  struct rdcp_message rm;
  uint16_t MY_HQ_DEVICE_ADDRESS = 0x00F0;

  /* Prepare the RDCP Header */
  rm.header.sender       = MY_HQ_DEVICE_ADDRESS;
  rm.header.origin       = MY_HQ_DEVICE_ADDRESS;
  rm.header.sequence_number = get_next_rdcp_sequence_number(MY_HQ_DEVICE_ADDRESS);
  rm.header.destination  = RDCP_BROADCAST_ADDRESS;
  rm.header.message_type = RDCP_MSGTYPE_TIMESTAMP;
  rm.header.rdcp_payload_length = RDCP_SIGNATURE_LENGTH + 6;
  rm.header.counter      = 0x00;
  uint16_t primary_ep = getSuggestedRelay(0); //getEntryPoint(0);
  rm.header.relay1       = (uint8_t) ((primary_ep & 0x000F) * 8) + (uint8_t) 0x0;
  rm.header.relay2       = 0xEE;
  rm.header.relay3       = 0xEE;

  /* Prepare the RDCP Payload */
  rm.payload.data[0] = year;
  rm.payload.data[1] = month;
  rm.payload.data[2] = day;
  rm.payload.data[3] = hour;
  rm.payload.data[4] = minute;
  rm.payload.data[5] = status;

  /* Create the Schnorr signature */
  uint8_t sha[32];
  get_inline_hash(&rm, 6, sha);
  uint8_t sig[128];
  int res = schnorr_create_signature(sha, 32, sig);
  /* Add the Schnorr signature to the RDCP Payload */
  for (int i=0; i<res; i++) rm.payload.data[6+i] = sig[i];

  /* Finalize the RDCP Header by calculating the checksum */
  uint8_t data_for_crc[256];
  memcpy(&data_for_crc, &rm.header, RDCP_HEADER_SIZE - 2);
  for (int i=0; i < rm.header.rdcp_payload_length; i++)
  {
    data_for_crc[i + RDCP_HEADER_SIZE - 2] = rm.payload.data[i];
  }
  uint16_t actual_crc = crc16(data_for_crc, RDCP_HEADER_SIZE - 2 + rm.header.rdcp_payload_length);
  rm.header.checksum = actual_crc;

  /* Schedule the crafted message for sending */
  uint8_t data[256];
  memcpy(&data, &rm.header, RDCP_HEADER_SIZE);
  for (int i=0; i<rm.header.rdcp_payload_length; i++) data[i + RDCP_HEADER_SIZE] = rm.payload.data[i];
  rdcp_txqueue_add(data, RDCP_HEADER_SIZE + rm.header.rdcp_payload_length, NOTIMPORTANT, NOTFORCEDTX, TX_CALLBACK_NONE, 0);

  return;
}

void rdcp_mg_process_incoming_timestamp(void)
{
  if (rdcp_msg_in.header.rdcp_payload_length != RDCP_SIGNATURE_LENGTH + 6)
  {
    serial_writeln("WARNING: Size of received RDCP Timestamp is invalid");
    return;
  }

  uint8_t sha[32];
  get_inline_hash(&rdcp_msg_in, 6, sha);
  uint8_t sig[128];
  for (int i=0; i<65; i++) sig[i] = rdcp_msg_in.payload.data[6+i];
  bool valid_signature = schnorr_verify_signature(sha, 32, sig);
  if (!valid_signature)
  {
    serial_writeln("WARNING: Invalid HQ Schnorr signature for RDCP Timestamp - ignoring");
    return;
  }

  uint8_t year = rdcp_msg_in.payload.data[0];
  uint8_t month = rdcp_msg_in.payload.data[1];
  uint8_t day = rdcp_msg_in.payload.data[2];
  uint8_t hour = rdcp_msg_in.payload.data[3];
  uint8_t minute = rdcp_msg_in.payload.data[4];
  uint8_t status = rdcp_msg_in.payload.data[5];

  char msg[256];
  snprintf(msg, 256, "INFO: Received valid RDCP Timestamp: %02d.%02d.%04d %02d:%02d (Status %d)", day, month, 2025 + year, hour, minute, status);
  serial_writeln(msg);

  tdeck_set_time(2025 + year, month, day, hour, minute);
  if (status == 0x00)
  { // Infrastructure in non-crisis mode
      if (gui_get_current_screen() != SCREEN_EULA) gui_transition_to_screen(SCREEN_OANONCRISIS);
      set_infrastructure_in_crisis(false);
      mb_count_and_show_last(true, false);
  }
  else
  { // Infrastructure is in crisis mode
    if (gui_get_current_screen() == SCREEN_OANONCRISIS) // save to move from there without interrupting interaction
    {
      gui_transition_to_screen(SCREEN_OACRISIS);
    } // otherwise, don't change screen, the user might be entering a message somewhere
    set_infrastructure_in_crisis(true);
    mb_count_and_show_last(true, true);
  }
  return;
}

void rdcp_mg_process_incoming_ack(void)
{
  bool ack_valid = true;
  String invalidity_reason = "";
  bool has_signature = true;
  bool valid_signature = false;

  if (rdcp_msg_in.header.rdcp_payload_length < 3)
  {
    serial_writeln("WARNING: RDCP Payload too short for RDCP MT ACK");
    return;
  }

  if ((rdcp_msg_in.header.origin <= RDCP_HQ_MULTICAST_ADDRESS) && (rdcp_msg_in.header.rdcp_payload_length < 3+65))
  {
    serial_writeln("WARNING: RDCP ACK from HQ device is not signed, ignoring");
    return;
  }

  if (rdcp_msg_in.header.rdcp_payload_length == 3+RDCP_SIGNATURE_LENGTH)
  {
    has_signature = true;
  }
  else
  {
    has_signature = false;
  }

  uint16_t confirmed_seqnr = rdcp_msg_in.payload.data[0] + rdcp_msg_in.payload.data[1] * 256;
  uint8_t acktype = rdcp_msg_in.payload.data[2];

  if (has_signature)
  {
    uint8_t sha[32];
    get_inline_hash(&rdcp_msg_in, 3, sha);
    uint8_t sig[128];
    for (int i=0; i<65; i++) sig[i] = rdcp_msg_in.payload.data[3+i];
    valid_signature = schnorr_verify_signature(sha, 32, sig);
    if (valid_signature)
    {
      serial_writeln("INFO: RDCP ACK message has valid Schnorr signature from HQ device");
    }
    else
    {
      serial_writeln("WARNING: Invalid HQ Schnorr signature for RDCP ACK - ignoring");
      return;
    }
  }

  // Handle ACKs for CIREs
  rdcp_cire_ack(rdcp_msg_in.header.origin, confirmed_seqnr, acktype);

  return;
}

void rdcp_mg_process_incoming_message(void)
{
  /* Handle rdcp_msg_in depending on its RDCP Message Type and Destination. */
  /* It is known to be a valid (correct checksum), new (non-duplicate) RDCP Message. */

  uint8_t  mt   = rdcp_msg_in.header.message_type;
  uint16_t dest = rdcp_msg_in.header.destination;
  uint16_t me   = getMyRDCPAddress();

  /* Skip RDCP Message processing unless it matches my RDCP address,
     any of my multicast addresses, or the broadcast address */
  if (!matchesAnyOfMyAddresses(dest)) return;

  if (mt == RDCP_MSGTYPE_TEST) rdcp_mg_print_test_message();
  if ((mt == RDCP_MSGTYPE_ECHO_REQUEST) && (dest == me)) rdcp_mg_send_echo_response(rdcp_msg_in.header.origin);
  if ((mt == RDCP_MSGTYPE_ACK) && (dest == me)) rdcp_mg_process_incoming_ack();
  if (mt == RDCP_MSGTYPE_TIMESTAMP) rdcp_mg_process_incoming_timestamp();
  if (mt == RDCP_MSGTYPE_OFFICIAL_ANNOUNCEMENT)
  {
    if (dest == me)
    {
      rdcp_mg_process_incoming_private_oa();
    }
    else
    {
      rdcp_mg_process_incoming_public_oa();
    }
  }
  if (mt == RDCP_MSGTYPE_SIGNATURE) rdcp_mg_process_signature();
  if (mt == RDCP_MSGTYPE_RESET_ALL_ANNOUNCEMENTS) rdcp_mg_process_resetoa();
  if (mt == RDCP_MSGTYPE_INFRASTRUCTURE_RESET) rdcp_mg_process_resetinfrastructure();
  if (mt == RDCP_MSGTYPE_DEVICE_RESET) rdcp_mg_process_resetdevice();
  if (mt == RDCP_MSGTYPE_DEVICE_REBOOT) rdcp_mg_process_rebootdevice();
  if (mt == RDCP_MSGTYPE_MAINTENANCE) rdcp_mg_process_maintenance();
  if (mt == RDCP_MSGTYPE_DEVICE_BLOCK_ALERT) rdcp_mg_process_blockalert();

  return;
}

int64_t rdcp_get_timeslot_duration(uint8_t *data)
{
  int64_t duration = 0;

  struct rdcp_header h;
  memcpy(&h, data, RDCP_HEADER_SIZE);

  uint16_t airtime = airtime_in_ms(RDCP_HEADER_SIZE + h.rdcp_payload_length);
  uint16_t airtime_with_buffer = airtime + 1000;

  uint8_t nrt = 0;
  uint8_t mt = h.message_type;
  if ( (mt == RDCP_MSGTYPE_INFRASTRUCTURE_RESET) || (mt == RDCP_MSGTYPE_ACK) ||
       (mt == RDCP_MSGTYPE_RESET_ALL_ANNOUNCEMENTS) ) nrt = 2;
  if ( (mt == RDCP_MSGTYPE_OFFICIAL_ANNOUNCEMENT) || (mt == RDCP_MSGTYPE_CITIZEN_REPORT) ||
       (mt == RDCP_MSGTYPE_SIGNATURE) ) nrt = 4;

  duration = (nrt+1) * airtime_with_buffer;

  char buf[256];
  snprintf(buf, 256, "INFO: Queue message timeslot estimation: %llu ms (airtime_buf %u ms, nrt %d)", duration, airtime_with_buffer, nrt);
  serial_writeln(buf);

  return duration;
}

int get_num_txq_entries(void)
{
  return txq.num_entries;
}

int get_num_txaq_entries(void)
{
  return txaq.num_entries;
}

bool rdcp_txqueue_add(uint8_t *data, uint8_t len, bool important, bool force_tx, uint8_t callback_selector, int64_t forced_time=0)
{
  if (txq.num_entries == MAX_TXQUEUE_ENTRIES)
  {
    serial_writeln("WARNING: rdcp_txqueue_add() failed -- TX Queue is full");
    return false;
  }

  int64_t now = my_millis();

  for (int i=0; i < MAX_TXQUEUE_ENTRIES; i++)
  {
    if (txq.entries[i].waiting == false)
    {
      txq.num_entries += 1;
      txq.entries[i].waiting = true;
      txq.entries[i].in_process = false;
      txq.entries[i].timeslot_duration = rdcp_get_timeslot_duration(data);
      txq.entries[i].callback_selector = callback_selector;
      txq.entries[i].force_tx = force_tx;
      txq.entries[i].important = important;
      if (forced_time == 0)
      {
        txq.entries[i].originally_scheduled_time = rdcp_get_channel_free_estimation();
        if (txq.entries[i].originally_scheduled_time < now) txq.entries[i].originally_scheduled_time = now;
      }
      else
      {
        txq.entries[i].originally_scheduled_time = forced_time;
      }
      txq.entries[i].currently_scheduled_time = txq.entries[i].originally_scheduled_time;
      txq.entries[i].num_of_reschedules = 0;
      txq.entries[i].payload_length = len;
      txq.entries[i].in_process = false;
      txq.entries[i].cad_retry = 0;
      for (int j=0; j < len; j++) txq.entries[i].payload[j] = data[j];

      char buf[256];
      snprintf(buf, 256, "INFO: Outgoing message scheduled -> TXQi %d, len %d, TSd %" PRId64 ", @%" PRId64, i, len, txq.entries[i].timeslot_duration, txq.entries[i].currently_scheduled_time);
      serial_writeln(buf);

      return true; // found free spot and added entry, exit loop here.
    }
  }

  return false;
}

bool rdcp_txqueue_reschedule(int64_t offset=0)
{
  int64_t now = my_millis();
  int64_t delta = CFEst - now; // - CFEst_old;
  bool dropped = false;

  if (offset != 0) delta = offset;

  for (int i=0; i < MAX_TXQUEUE_ENTRIES; i++)
  {
    if (txq.entries[i].waiting)
    {
      if (txq.entries[i].in_process) continue;
      if (txq.entries[i].force_tx) continue;

      txq.entries[i].num_of_reschedules++;
      txq.entries[i].currently_scheduled_time += delta;

      if (txq.entries[i].currently_scheduled_time < now)
      {
        txq.entries[i].currently_scheduled_time = now + offset;
      }

      if (txq.entries[i].important) continue;
      if ( (txq.entries[i].num_of_reschedules > 20) ||
           (txq.entries[i].currently_scheduled_time - txq.entries[i].originally_scheduled_time > 300 * 1000) )
      {
        txq.entries[i].waiting = false; // drop message due to excessive delay when trying to send
        txq.entries[i].payload_length = 0;
        txq.entries[i].in_process = false;
        txq.num_entries--;
        dropped = true;
      }
    }
  }
  return dropped;
}

bool rdcp_txqueue_loop(void)
{
  int64_t now = my_millis();

  /* Skip if any transmission is already ongoing */
  if (tx_ongoing != -1)
  {
    if (now - last_tx_activity > 180000)
    {
      serial_writeln("WARNING: TX Activity Timeout, restarting TXQ processing");
      txq.entries[tx_ongoing].in_process = false;
      txq.entries[tx_ongoing].cad_retry = 0;
      tx_ongoing = -1;
      radio_apply_new_configuration(); // Re-initialize the radio hardware
    }
    return false;
  }

  last_tx_activity = now;

  /* Feed fresh messages into our queue */
  rdcp_txaheadqueue_loop();

  for (int i=0; i < MAX_TXQUEUE_ENTRIES; i++)
  {
    if (txq.entries[i].waiting)
    {
      if (txq.entries[i].currently_scheduled_time <= now)
      {
        if (tx_ongoing == -1)
        { // found a first message to start send-processing now
          tx_ongoing = i;
        }
        else
        { // already had found a suitable message, but maybe chose another one
          if (txq.entries[i].currently_scheduled_time < txq.entries[tx_ongoing].currently_scheduled_time) tx_ongoing = i; // try to keep the order
          if (txq.entries[i].force_tx) tx_ongoing = i; // but prioritize hard-scheduled messages even more
        }
      }
    }
  }
  if (tx_ongoing == -1) return false;

  txq.entries[tx_ongoing].in_process = true;
  tx_process_start = now;

  char buf[256];
  snprintf(buf, 256, "INFO: Outgoing message up for send-processing -> TXQi %d, len %d, TSd %" PRId64 ", @%" PRId64 ", =%" PRId64,
           tx_ongoing, txq.entries[tx_ongoing].payload_length, txq.entries[tx_ongoing].timeslot_duration, txq.entries[tx_ongoing].currently_scheduled_time, now);
  serial_writeln(buf);

  if (txq.entries[tx_ongoing].force_tx == false)
  {
    rdcp_send_message_cad();
  }
  else
  {
    rdcp_send_message_force();
  }

  return true;
}

bool rdcp_txaheadqueue_add(uint8_t *data, uint8_t len, bool important, bool force_tx, uint8_t callback_selector, int64_t delay_in_ms)
{
  if (txaq.num_entries == MAX_TXAHEADQUEUE_ENTRIES)
  {
    serial_writeln("WARNING: rdcp_txaheadqueue_add() failed -- TX Ahead Queue is full");
    return false;
  }

  int64_t now = my_millis();

  for (int i=0; i < MAX_TXAHEADQUEUE_ENTRIES; i++)
  {
    if (txaq.entries[i].waiting == false)
    {
      txaq.num_entries += 1;
      txaq.entries[i].waiting = true;
      txaq.entries[i].callback_selector = callback_selector;
      txaq.entries[i].force_tx = force_tx;
      txaq.entries[i].important = important;
      txaq.entries[i].scheduled_time = now + delay_in_ms;
      txaq.entries[i].payload_length = len;
      for (int j=0; j < len; j++) txaq.entries[i].payload[j] = data[j];

      char buf[256];
      snprintf(buf, 256, "INFO: Delayed message scheduled -> TXAQi %d, len %d, @%" PRId64, i, len, txaq.entries[i].scheduled_time);
      serial_writeln(buf);

      return true; // found free spot and added entry, exit loop here.
    }
  }

  return false;
}

bool rdcp_txaheadqueue_loop(void)
{
  if (txq.num_entries == TX_QUEUE_LENGTH)
  { /* TX Queue is full, cannot add more entries right now */
    return false;
  }

  int64_t now = my_millis();

  for (int i=0; i < MAX_TXAHEADQUEUE_ENTRIES; i++)
  {
    if ((txaq.entries[i].waiting == true) && (txaq.entries[i].scheduled_time <= now))
    {
      if (rdcp_txqueue_add(txaq.entries[i].payload, txaq.entries[i].payload_length, txaq.entries[i].important, txaq.entries[i].force_tx, txaq.entries[i].callback_selector, txaq.entries[i].scheduled_time) == true)
      {
        txaq.entries[i].waiting = false;
        txaq.num_entries--;
        return true;
      }
      else
      { // Moving to TXQ failed, probably full. Try again next time.
        return false;
      }
    }
  }

  return true;
}

bool rdcp_tx_interface(String b64rdcpmsg, int64_t time=TX_WHEN_CF, uint8_t channel=CHANNEL868)
{
  int64_t now = my_millis();
  char buffer[BUFFER_SIZE];

  b64rdcpmsg.toCharArray(buffer, BUFFER_SIZE);
  int b64msg_len = strlen(buffer);
  int decoded_length = Base64ren.decodedLength(buffer, b64msg_len);
  char decoded_string[decoded_length + 1];
  Base64ren.decode(decoded_string, buffer, b64msg_len);

  uint8_t outgoing_message[256];
	for (int i=0; i < decoded_length; i++) outgoing_message[i] = decoded_string[i];

  if (decoded_length < RDCP_HEADER_SIZE)
  {
    serial_writeln("ERROR: Message too short to be an RDCP Message, refusing to schedule.");
    return false;
  }

  if (time == TX_WHEN_CF)
  {
    rdcp_txqueue_add(outgoing_message, decoded_length, IMPORTANT, NOTFORCEDTX, TX_CALLBACK_NONE, NO_FORCED_TIME);
  }
  else if (time == TX_IMMEDIATELY)
  {
    rdcp_txqueue_add(outgoing_message, decoded_length, IMPORTANT, FORCEDTX, TX_CALLBACK_NONE, now);
  }
  else
  {
    rdcp_txaheadqueue_add(outgoing_message, decoded_length, IMPORTANT, FORCEDTX, TX_CALLBACK_NONE, time);
  }

  return true;
}

void rdcp_send_message_cad(void)
{
  lora_radio_startcad();
  return;
}

bool rdcp_send_message_force(void)
{
  int64_t now = my_millis();
  int64_t timediff = now - txq.entries[tx_ongoing].currently_scheduled_time - retransmission_count * (airtime_in_ms(txq.entries[tx_ongoing].payload_length) + 1000);
  char buf[256];
  snprintf(buf, 256, "INFO: TX Start for TXQi %d, len %d, TSd %" PRId64 "ms, latency %" PRId64 " ms", tx_ongoing, txq.entries[tx_ongoing].payload_length, txq.entries[tx_ongoing].timeslot_duration, timediff);
  serial_writeln(buf);

  snprintf(buf, 256, "TXMETA %d %" PRId64 " %3.3f", txq.entries[tx_ongoing].payload_length, now, getMyLoRaFrequency());
  serial_writeln(buf);

  int encodedLength = Base64ren.encodedLength(txq.entries[tx_ongoing].payload_length);
  char b64msg[encodedLength + 1];
  Base64ren.encode(b64msg, (char *) txq.entries[tx_ongoing].payload, txq.entries[tx_ongoing].payload_length);

  snprintf(buf, 256, "TX %s", b64msg);
  serial_writeln(buf);

  return lora_radio_send(txq.entries[tx_ongoing].payload, txq.entries[tx_ongoing].payload_length);
}

void rdcp_callback_txfin(void)
{
  char buf[256];

  last_tx_activity = my_millis();

  int num_waiting = -1;
  for (int i=0; i < MAX_TXQUEUE_ENTRIES; i++) if (txq.entries[i].waiting)
  {
     num_waiting++;
  }

  int num_retransmissions = 0;
  struct rdcp_message rm;
  memcpy(&rm.header, txq.entries[tx_ongoing].payload, RDCP_HEADER_SIZE);
  for (int i=0; i < rm.header.rdcp_payload_length; i++) rm.payload.data[i] = txq.entries[tx_ongoing].payload[RDCP_HEADER_SIZE + i];
  num_retransmissions = rm.header.counter;

  snprintf(buf, 256, "INFO: TXFIN 4 TXQi %d, %d retransmissions ahead, %d/%d more messages waiting", tx_ongoing, num_retransmissions, num_waiting, txq.num_entries);
  serial_writeln(buf);

  txq.entries[tx_ongoing].cad_retry = 0;

  if (num_retransmissions > 0)
  {
    rm.header.counter -= 1;

    uint8_t data_for_crc[256];
    /* Copy RDCP header and payload into data structure for CRC calculation */
    memcpy(&data_for_crc, &rm.header, RDCP_HEADER_SIZE - 2);
    for (int i=0; i < rm.header.rdcp_payload_length; i++)
    {
      data_for_crc[i + RDCP_HEADER_SIZE - 2] = rm.payload.data[i];
    }
    /* Calculate and update CRC */
    uint16_t actual_crc = crc16(data_for_crc, RDCP_HEADER_SIZE - 2 + rm.header.rdcp_payload_length);
    rm.header.checksum = actual_crc;
    memcpy(txq.entries[tx_ongoing].payload, &rm.header, RDCP_HEADER_SIZE); // no need to overwrite RDCP payload

    delay(PAUSE_BETWEEN_RETRANSMISSIONS_ON_THIS_DEVICE);
    retransmission_count++;
    rdcp_send_message_force();
  }
  else
  {
    if (txq.entries[tx_ongoing].callback_selector == TX_CALLBACK_NONE)
    {
      // Nothing to do; no callback necessary.
    }
    else if (txq.entries[tx_ongoing].callback_selector == TX_CALLBACK_CIRE)
    {
      rdcp_cire_callback();
    }
    else /* Add more callback options here later */
    {
      // do nothing
    }

    txq.entries[tx_ongoing].waiting = false;
    txq.entries[tx_ongoing].payload_length = 0;
    txq.entries[tx_ongoing].in_process = false;
    txq.num_entries--;
    retransmission_count = 0;
    tx_ongoing = -1;

    // When we finished transmitting, others expect the channel to be free
    // and might want to start sending urgent messages. Thus, as we just used
    // the channel for ourselves for some time, give them a chance.
    rdcp_txqueue_reschedule(random(10000, 20000));
  }

  return;
}

bool rdcp_callback_cad(bool cad_busy)
{
  bool channel_free = !cad_busy;
  char buf[256];

  last_tx_activity = my_millis();

  txq.entries[tx_ongoing].cad_retry += 1;
  uint8_t retry = txq.entries[tx_ongoing].cad_retry;

  snprintf(buf, 256, "INFO: Send-processing: CAD reports channel %s (try %d)", channel_free ? "free" : "busy", retry);
  serial_writeln(buf);

  if (channel_free)
  {
    rdcp_send_message_force();
    return true;
  }

  if ((retry >= 0) && (retry <= 4))
  {
    lora_radio_startcad();
  }
  else if (retry == 5)
  {
    lora_radio_receivemode();
    txq.entries[tx_ongoing].in_process = false;
    tx_ongoing = -1;
    int64_t random_delay = random(21000, 25001);
    rdcp_txqueue_reschedule(random_delay);
    if (CFEst < my_millis() + random_delay) CFEst = my_millis() + random_delay; // Don't re-schedule twice
  }
  else if ((retry >= 6) && (retry <= 9))
  {
    lora_radio_startcad();
  }
  else if ((retry >= 10) && (retry <= 14))
  {
    lora_radio_receivemode();
    txq.entries[tx_ongoing].in_process = false;
    tx_ongoing = -1;
    int64_t random_delay = random(31000, 35001);
    rdcp_txqueue_reschedule(random_delay);
    if (CFEst < my_millis() + random_delay) CFEst = my_millis() + random_delay; // Don't re-schedule twice
  }
  else if (retry >= 15)
  {
    snprintf(buf, 256, "WARNING: CAD retry timeout for TXQi %d, force-sending now", tx_ongoing);
    serial_writeln(buf);
    rdcp_send_message_force();
    return true;
  }

  return false;
}

uint8_t cire_retry = 0;     // How often did we already try to send a CIRE?
int64_t cire_starttime = 0; // When was the CIRE sent?
#define CIRE_STATE_NONE    0
#define CIRE_STATE_WAIT_DA 1
#define CIRE_STATE_WAIT_HQ 2
uint8_t  cire_state = CIRE_STATE_NONE;
uint16_t cire_seqnrs[]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t cire_current_seqnr = 0x0000;
uint8_t  cire_current_subtype = 0x00;
uint16_t cire_current_refnr = 0x0000;
char     cire_current_text[256];
uint16_t cire_guitext_num = 1;

int get_cire_state(void) { return cire_state; }

void rdcp_cire_callback(void)
{
  cire_starttime = my_millis();
  cire_state = CIRE_STATE_WAIT_DA;
  for (int i=0; i<16; i++)
  {
    if (cire_seqnrs[i] == 0x0000)
    {
      cire_seqnrs[i] = cire_current_seqnr;
      break;
    }
  }
  serial_writeln("INFO: Internal CIRE Callback executed, ACK timeout timer started");

  return;
}

/**
 * Re-send the previous CIRE again after a timeout. Triggered by rdcp_cire_check().
 */
void rdcp_cire_resend(void)
{
  cire_state = CIRE_STATE_NONE;
  cire_retry++;

  if (cire_retry > 5)
  {
    cire_retry = 0;
    for (int i=0; i<16; i++) cire_seqnrs[i] = 0x0000;
    serial_writeln("WARNING: CIRE could not be sent (ACK missing), giving up");

    FFat.remove(FILENAME_CIRE);

    char gui_text[512];
    snprintf(gui_text, 512, "WICHTIG: Ihre Meldung %04X-%d konnte anhaltend NICHT an den Krisenstab zugestellt werden. Bitte weichen Sie auf andere Kommunikationswege aus oder versuchen Sie es zu einer anderen Zeit und ggf. von einem anderen Ort aus erneut.", getMyRDCPAddress(), cire_current_refnr);
    mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, true);

    gui_enable_cire_buttons();

    return;
  }

  char buf[256];
  snprintf(buf, 256, "INFO: Re-sending CIRE %d, re-try #%d", cire_current_refnr, cire_retry);
  serial_writeln(buf);

  rdcp_send_cire(cire_current_subtype, cire_current_refnr, cire_current_text);
  return;
}

uint16_t cire_timeout_ack_da = 60;  // default: 1 minute
uint16_t cire_timeout_ack_hq = 900; // default: 15 minutes

void set_ciretime_da(uint16_t timeout)
{
  cire_timeout_ack_da = timeout;
  return;
}

void set_ciretime_hq(uint16_t timeout)
{
  cire_timeout_ack_hq = timeout;
  return;
}

/**
 * Check whether we have an ongoing CIRE for which the required ACKs have not yet
 * been received. Must be called periodically.
 */
void rdcp_cire_check(void)
{
  if (cire_state == CIRE_STATE_NONE) return;

  int64_t now = my_millis();
  if (cire_state == CIRE_STATE_WAIT_DA)
  {
    if (now - cire_starttime > cire_timeout_ack_da * 1000)
    {
      serial_writeln("WARNING: CIRE Timeout waiting for ACK from DA");

      char gui_text[512];
      snprintf(gui_text, 512, "Ihre Meldung %04X-%d wird erneut gesendet, da noch keine Eingangsmeldung von der Infrastruktur vorliegt.", getMyRDCPAddress(), cire_current_refnr);
      mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, true);

      rdcp_cire_resend();
    }
  }

  if (cire_state == CIRE_STATE_WAIT_HQ)
  {
    if (now - cire_starttime > cire_timeout_ack_hq * 1000)
    {
      serial_writeln("WARNING: CIRE Timeout waiting for ACK from HQ");

      char gui_text[512];
      snprintf(gui_text, 512, "Ihre Meldung %04X-%d wird erneut gesendet, da noch keine Eingangsmeldung vom Krisenstab vorliegt.", getMyRDCPAddress(), cire_current_refnr);
      mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, true);

      rdcp_cire_resend();
    }
  }

  return;
}

/**
 * Check whether an incoming ACK matches our ongoing CIRE.
 * If it does, finish the CIRE processing.
 */
void rdcp_cire_ack(uint16_t origin, uint16_t confirmedseqnr, uint8_t acktype)
{
  // Check whether a received ACK matches one of our CIRE SeqNrs.
  // When sent by DA, continue the process. When sent by HQ, finish it.
  if (cire_state == CIRE_STATE_NONE) return; // Nothing to do.

  bool matches_cire = false;
  for (int i=0; i<16; i++) if ((cire_seqnrs[i] != 0) && (cire_seqnrs[i] == confirmedseqnr)) matches_cire = true;

  if (!matches_cire)
  {
    serial_writeln("INFO: Received ACK does not match current CIRE - ignoring");
    return;
  }

  if ((origin >= 0x0200) && (origin < 0x0300))
  {
    FFat.remove(FILENAME_CIRE);
    // Received ACK was from a DA, not by HQ
    if (acktype == RDCP_ACKNOWLEDGMENT_POSITIVE)
    {
      serial_writeln("INFO: Positive ACK from DA received, waiting for HQ ACK");
      cire_state = CIRE_STATE_WAIT_HQ;

      char gui_text[512];
      snprintf(gui_text, 512, "Ihre Meldung %04X-%d wird inzwischen von der Infrastruktur zum Krisenstab transportiert. Bitte warten Sie auf weitere Informationen.", getMyRDCPAddress(), cire_current_refnr);
      //gui_crisis_add_text(gui_text);
      mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, true);
    }
    else if (acktype == RDCP_ACKNOWLEDGMENT_NEGATIVE)
    {
      serial_writeln("INFO: Negative ACK from DA received, aborting CIRE procedure");
      cire_state = CIRE_STATE_NONE;
      for (int i=0; i<16; i++) cire_seqnrs[i] = 0x0000;
      cire_retry = 0;

      char gui_text[512];
      snprintf(gui_text, 512, "WICHTIG: Ihre Meldung %04X-%d kann nicht verarbeitet werden, da die Infrastruktur nicht im Krisen-Betriebsmodus ist. Falls Sie Hilfe brauchen, kontaktieren Sie Rettungsdienste oder die Gemeinde bitte anderweitig.", getMyRDCPAddress(), cire_current_refnr);
      mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, true);
      mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, false); // Add message to both crisis and non-crisis screen
      gui_enable_cire_buttons();
      gui_transition_to_screen(SCREEN_OANONCRISIS);
    }
    else if (acktype == RDCP_ACKNOWLEDGMENT_POSNEG)
    {
      serial_writeln("INFO: Positive-negative ACK from DA received, waiting for HQ ACK");
      cire_state = CIRE_STATE_WAIT_HQ;
      char gui_text[512];
      snprintf(gui_text, 512, "Ihre Meldung %04X-%d wird inzwischen von der Infrastruktur zum Krisenstab transportiert. Bitte warten Sie auf weitere Informationen. Bitte beachten Sie, dass der Krisenstab derzeit nicht besetzt ist.", getMyRDCPAddress(), cire_current_refnr);
      mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, true);
    }
    else
    {
      serial_writeln("WARNING: Unknown ACK type");
      return;
    }
  }

  if ((origin > 0x0000) && (origin < 0x0100))
  {
    // Received ACK was from HQ; in any case, clear current CIRE state:
    FFat.remove(FILENAME_CIRE);
    for (int i=0; i<16; i++) cire_seqnrs[i] = 0x0000;
    cire_retry = 0;
    cire_state = CIRE_STATE_NONE;
    gui_enable_cire_buttons();

    if (acktype == RDCP_ACKNOWLEDGMENT_POSITIVE)
    {
      serial_writeln("INFO: Positive ACK from HQ received");
      char gui_text[512];
      snprintf(gui_text, 512, "Ihre Meldung %04X-%d ist beim Krisenstab eingegangen. Bitte lassen Sie den Pager eingeschaltet, falls es weitere Informationen oder Fragen zu Ihrer Meldung gibt.", getMyRDCPAddress(), cire_current_refnr);
      mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, true);
    }
    else if (acktype == RDCP_ACKNOWLEDGMENT_NEGATIVE)
    {
      serial_writeln("INFO: Negative ACK from HQ received");
      char gui_text[512];
      snprintf(gui_text, 512, "WICHTIG: Ihre Meldung %04X-%d wurde an die Gemeinde geschickt, kann aber nicht verarbeitet werden, da die Infrastruktur derzeit nicht im Krisen-Betriebsmodus ist. Falls Sie Hilfe brauchen, kontaktieren Sie Rettungsdienste oder die Gemeinde bitte anderweitig.", getMyRDCPAddress(), cire_current_refnr);
      mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, true);
      mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, false); // Add message to both crisis and non-crisis screen
      gui_transition_to_screen(SCREEN_OANONCRISIS);
    }
    else if (acktype == RDCP_ACKNOWLEDGMENT_POSNEG)
    {
      serial_writeln("INFO: Positive-negative ACK from HQ received");
      char gui_text[512];
      snprintf(gui_text, 512, "Ihre Meldung %04X-%d ist beim Krisenstab eingegangen. Dieser ist derzeit nicht besetzt, wird Ihre Meldung aber noch bearbeiten.", getMyRDCPAddress(), cire_current_refnr);
      mb_add_local_message(gui_text, cire_current_refnr, ++cire_guitext_num, 60002, true);
    }
    else
    {
      serial_writeln("WARNING: Unknown ACK type");
      return;
    }
  }

  return;
}

void rdcp_send_ack_signed(uint16_t origin, uint16_t destination, uint16_t seqnr, uint8_t acktype)
{
  struct rdcp_message rm;
  uint8_t data[256];

  rm.header.origin = origin;
  rm.header.sender = origin;
  rm.header.destination = destination;
  rm.header.message_type = RDCP_MSGTYPE_ACK;
  rm.header.counter = 2;
  rm.header.sequence_number = get_next_rdcp_sequence_number(origin);
  uint16_t primary_ep = getSuggestedRelay(0); //getEntryPoint(0);
  rm.header.relay1 = (uint8_t) ((primary_ep & 0x000F) * 8) + (uint8_t) 0x0;
  if ((origin >= 0x0100) && (origin < 0x0300)) rm.header.relay1 = 0xEE; // don't use an Entry Point when spoofing a DA
  rm.header.relay2 = 0xEE;
  rm.header.relay3 = 0xEE;

  rm.payload.data[0] = seqnr % 256;
  rm.payload.data[1] = seqnr / 256;
  rm.payload.data[2] = acktype;

  rm.header.rdcp_payload_length = 3 + RDCP_SIGNATURE_LENGTH; // 3 bytes payload, 65 bytes signature

  /* Prepare the data to be signed */
  uint8_t data_to_sign[256];
  data_to_sign[0] = rm.header.origin % 256;
  data_to_sign[1] = rm.header.origin / 256;
  data_to_sign[2] = rm.header.sequence_number % 256;
  data_to_sign[3] = rm.header.sequence_number / 256;
  data_to_sign[4] = rm.header.destination % 256;
  data_to_sign[5] = rm.header.destination / 256;
  data_to_sign[6] = rm.header.message_type;
  data_to_sign[7] = rm.header.rdcp_payload_length;
  data_to_sign[8] = rm.payload.data[0];
  data_to_sign[9] = rm.payload.data[1];
  data_to_sign[10] = rm.payload.data[2];
  uint8_t data_to_sign_length = 11;

  /* Get the SHA-256 hash for the data to be signed */
  SHA256 h = SHA256();
  h.reset();
  h.update(data_to_sign, data_to_sign_length);
  uint8_t sha[32];
  h.finalize(sha, 32);

  /* Create the Schnorr signature */
  uint8_t sig[128];
  int res = schnorr_create_signature(sha, 32, sig);

  /* Add the Schnorr signature to the RDCP Payload */
  for (int i=0; i<res; i++) rm.payload.data[3+i] = sig[i];

  /* Finalize the RDCP Header by calculating the checksum */
  uint8_t data_for_crc[256];
  memcpy(&data_for_crc, &rm.header, RDCP_HEADER_SIZE - 2);
  for (int i=0; i < rm.header.rdcp_payload_length; i++)
  {
    data_for_crc[i + RDCP_HEADER_SIZE - 2] = rm.payload.data[i];
  }
  uint16_t actual_crc = crc16(data_for_crc, RDCP_HEADER_SIZE - 2 + rm.header.rdcp_payload_length);
  rm.header.checksum = actual_crc;

  /* Schedule the crafted message for sending */
  memcpy(&data, &rm.header, RDCP_HEADER_SIZE);
  for (int i=0; i<rm.header.rdcp_payload_length; i++) data[i + RDCP_HEADER_SIZE] = rm.payload.data[i];
  rdcp_txqueue_add(data, RDCP_HEADER_SIZE + rm.header.rdcp_payload_length, IMPORTANT, NOTFORCEDTX, TX_CALLBACK_NONE, 0);

  return;
}

void rdcp_send_ack_unsigned(uint16_t origin, uint16_t destination, uint16_t seqnr, uint8_t acktype)
{
  struct rdcp_message rm;
  uint8_t data[256];

  rm.header.origin = origin;
  rm.header.sender = origin;
  rm.header.destination = destination;
  rm.header.message_type = RDCP_MSGTYPE_ACK;
  rm.header.counter = 2;
  rm.header.sequence_number = get_next_rdcp_sequence_number(origin);
  uint16_t primary_ep = getSuggestedRelay(0); //getEntryPoint(0);
  rm.header.relay1 = (uint8_t) ((primary_ep & 0x000F) * 8) + (uint8_t) 0x0;
  if ((origin >= 0x0100) && (origin < 0x0300)) rm.header.relay1 = 0xEE; // don't use an Entry Point when spoofing a DA
  rm.header.relay2 = 0xEE;
  rm.header.relay3 = 0xEE;

  rm.payload.data[0] = seqnr % 256;
  rm.payload.data[1] = seqnr / 256;
  rm.payload.data[2] = acktype;

  rm.header.rdcp_payload_length = 3; // 3 bytes payload, unsigned

  /* Finalize the RDCP Header by calculating the checksum */
  uint8_t data_for_crc[256];
  memcpy(&data_for_crc, &rm.header, RDCP_HEADER_SIZE - 2);
  for (int i=0; i < rm.header.rdcp_payload_length; i++)
  {
    data_for_crc[i + RDCP_HEADER_SIZE - 2] = rm.payload.data[i];
  }
  uint16_t actual_crc = crc16(data_for_crc, RDCP_HEADER_SIZE - 2 + rm.header.rdcp_payload_length);
  rm.header.checksum = actual_crc;

  /* Schedule the crafted message for sending */
  memcpy(&data, &rm.header, RDCP_HEADER_SIZE);
  for (int i=0; i<rm.header.rdcp_payload_length; i++) data[i + RDCP_HEADER_SIZE] = rm.payload.data[i];
  rdcp_txqueue_add(data, RDCP_HEADER_SIZE + rm.header.rdcp_payload_length, IMPORTANT, NOTFORCEDTX, TX_CALLBACK_NONE, 0);

  return;
}

struct cirefileentry {
  uint8_t subtype;
  uint16_t refnr;
  char text[256];
};

cirefileentry cfile;

void rdcp_check_cirefile(void)
{
  if (hasStorage)
  {
    File cf = FFat.open(FILENAME_CIRE, FILE_READ);
    if (cf)
    {
      cf.read((uint8_t*)&cfile, sizeof(cirefileentry));
      cf.close();
      rdcp_send_cire(cfile.subtype, cfile.refnr, cfile.text);
    }
  }
  return;
}

void rdcp_send_cire(uint8_t subtype, uint16_t refnr, char *text)
{
  struct rdcp_message rm;
  uint8_t data[256];

  /* Save the information for later re-try events. */
  cire_current_subtype = subtype;
  cire_current_refnr = refnr;
  snprintf(cire_current_text, 256, "%s", text);

  if (hasStorage)
  {
    cfile.subtype = subtype;
    cfile.refnr = refnr;
    snprintf(cfile.text, 256, "%s", text);
    File cf = FFat.open(FILENAME_CIRE, FILE_WRITE);
    if (cf)
    {
      cf.write((uint8_t*)&cfile, sizeof(cirefileentry));
      cf.close();
    }
  }

  /* Fill the RDCP Header with already known information */
  uint16_t me = getMyRDCPAddress();
  rm.header.origin = me;
  rm.header.sender = me;
  rm.header.destination  = RDCP_HQ_MULTICAST_ADDRESS;
  rm.header.message_type = RDCP_MSGTYPE_CITIZEN_REPORT;
  rm.header.counter = 4;
  rm.header.sequence_number = get_next_rdcp_sequence_number(me);
  cire_current_seqnr = rm.header.sequence_number;

  uint16_t primary_ep = getSuggestedRelay(cire_retry); //getEntryPoint(cire_retry);
  rm.header.relay1 = (uint8_t) ((primary_ep & 0x000F) * 8) + (uint8_t) 0x0;
  rm.header.relay2 = 0xEE;
  rm.header.relay3 = 0xEE;

  /* Prepare the MT-specific header at the beginning of the RDCP Payload */
  rm.payload.data[0] = subtype;
  rm.payload.data[1] = refnr % 256;
  rm.payload.data[2] = refnr / 256;

  /* Convert the text message to its Unishox2 representation */
  char buf[256];
  unsigned int len = strlen(text);
  memset(buf, 0, sizeof(buf));
  int c_total = unishox2_compress_simple(text, len, buf);

  /* Fill the RDCP Payload with the Unishox2 content */
  for (int i=0; i < c_total; i++)
  {
    rm.payload.data[i+3] = buf[i];
  }

  /* RDCP Payload length is subheader length (3) + Unishox2 length + AES-GMC AuthTag size (16) */
  rm.header.rdcp_payload_length = 3 + c_total + 16;

  /* AES-GCM encrypt the RDCP Payload */
  uint8_t ciphertext[256];
  uint8_t iv[12];
  uint8_t gcmauthtag[16];
  uint8_t additional_data[8];
  uint8_t additional_data_size = 8;

  memset(iv, 0, sizeof(iv));
  iv[0] = rm.header.origin % 256;
  iv[1] = rm.header.origin / 256;
  iv[2] = rm.header.sequence_number % 256;
  iv[3] = rm.header.sequence_number / 256;
  iv[4] = rm.header.destination % 256;
  iv[5] = rm.header.destination / 256;
  iv[6] = rm.header.message_type;
  iv[7] = rm.header.rdcp_payload_length;
  for (int i=0; i<additional_data_size; i++) additional_data[i] = iv[i];

  encrypt_aes256gcm((uint8_t *) &rm.payload.data, rm.header.rdcp_payload_length - 16,
                    additional_data, additional_data_size,
                    getHQsharedSecret(), 32,
                    iv, 12,
                    ciphertext, gcmauthtag, 16);

  /* Copy ciphertext and GCM AuthTag into the RDCP Payload */
  for (int i=0; i<rm.header.rdcp_payload_length-16; i++) rm.payload.data[i] = ciphertext[i];
  for (int i=0; i<16; i++) rm.payload.data[rm.header.rdcp_payload_length-16+i] = gcmauthtag[i];

  /* Finalize the RDCP Header by calculating the checksum */
  uint8_t data_for_crc[256];
  memcpy(&data_for_crc, &rm.header, RDCP_HEADER_SIZE - 2);
  for (int i=0; i < rm.header.rdcp_payload_length; i++)
  {
    data_for_crc[i + RDCP_HEADER_SIZE - 2] = rm.payload.data[i];
  }
  uint16_t actual_crc = crc16(data_for_crc, RDCP_HEADER_SIZE - 2 + rm.header.rdcp_payload_length);
  rm.header.checksum = actual_crc;

  /* Schedule the crafted message for sending */
  memcpy(&data, &rm.header, RDCP_HEADER_SIZE);
  for (int i=0; i<rm.header.rdcp_payload_length; i++) data[i + RDCP_HEADER_SIZE] = rm.payload.data[i];
  rdcp_txqueue_add(data, RDCP_HEADER_SIZE + rm.header.rdcp_payload_length, IMPORTANT, NOTFORCEDTX, TX_CALLBACK_CIRE, 0);

  return;
}

int64_t last_heartbeat_sent = 0;
int64_t default_heartbeat_interval = 30 * 60 * 1000; // 30 minute default heartbeat interval
int64_t heartbeat_interval = 30 * 60 * 1000; // current heartbeat interval
int64_t initial_grace_period = 20000; // do not send heartbeats this long after turning on the device
int64_t heartbeat_max_interval = 60 * 60 * 1000;
int64_t heartbeat_interval_increment = 20 * 1000;

void set_heartbeat_interval(int64_t new_interval)
{
  default_heartbeat_interval = new_interval;
  heartbeat_interval = new_interval;
  return;
}

void set_grace_period(int64_t new_grace)
{
  initial_grace_period = new_grace;
  return;
}

void rdcp_heartbeat_send(void)
{
  serial_writeln("INFO: Sending HEARTBEAT message for this device");

  struct rdcp_message rm;
  uint8_t data[256];

  uint16_t me = getMyRDCPAddress();
  rm.header.origin = me;
  rm.header.sender = me;
  rm.header.destination  = RDCP_HQ_MULTICAST_ADDRESS;
  rm.header.message_type = RDCP_MSGTYPE_HEARTBEAT;
  rm.header.rdcp_payload_length = 0;
  rm.header.counter = 0;
  rm.header.sequence_number = 0x0000; // special for Heartbeat messages
  rm.header.relay1 = 0xEE; // special for Heartbeat messages
  rm.header.relay2 = 0xEE;
  rm.header.relay3 = 0xEE;

  /* Finalize the RDCP Header by calculating the checksum */
  uint8_t data_for_crc[256];
  memcpy(&data_for_crc, &rm.header, RDCP_HEADER_SIZE - 2);
  uint16_t actual_crc = crc16(data_for_crc, RDCP_HEADER_SIZE - 2);
  rm.header.checksum = actual_crc;

  /* Schedule the crafted message for sending */
  memcpy(&data, &rm.header, RDCP_HEADER_SIZE);
  rdcp_txqueue_add(data, RDCP_HEADER_SIZE, NOTIMPORTANT, NOTFORCEDTX, TX_CALLBACK_NONE, 0);

  return;
}

void rdcp_heartbeat_check(void)
{
  int64_t now = my_millis();

  if (now <= initial_grace_period) return;

  if (cire_state != CIRE_STATE_NONE)
  { // Don't send Heartbeat messages while waiting for an ACK to a CIRE
    last_heartbeat_sent = now; // skip this heartbeat
    return;
  }

  uint8_t current_screen = gui_get_current_screen();
  if ((current_screen != SCREEN_OACRISIS) && (current_screen != SCREEN_OANONCRISIS) && (current_screen != SCREEN_EULA)) return; // last was SPLASH

  if ((last_heartbeat_sent == 0) || (now - last_heartbeat_sent > heartbeat_interval))
  {
    // Send if channel is free, re-schedule if it is currently busy
    if ((now >= rdcp_get_channel_free_estimation()) && (true)) // is_screensaver_on()))
    {
      heartbeat_interval = default_heartbeat_interval;
      last_heartbeat_sent = now;
      rdcp_heartbeat_send();
    }
    else
    {
      if (heartbeat_interval < heartbeat_max_interval) heartbeat_interval += heartbeat_interval_increment;
    }
  }

  return;
}

void rdcp_mg_process_incoming_private_oa(void)
{
  /* Official Announcements sent to a single device are symetrically encrypted. */
  uint8_t ciphertext[256];
  uint8_t plaintext[256];
  uint8_t iv[12];
  uint8_t gcmauthtag[16];
  uint8_t additional_data[8];
  uint8_t additional_data_size = 8;

  memset(iv, 0, sizeof(iv));
  iv[0] = rdcp_msg_in.header.origin % 256;
  iv[1] = rdcp_msg_in.header.origin / 256;
  iv[2] = rdcp_msg_in.header.sequence_number % 256;
  iv[3] = rdcp_msg_in.header.sequence_number / 256;
  iv[4] = rdcp_msg_in.header.destination % 256;
  iv[5] = rdcp_msg_in.header.destination / 256;
  iv[6] = rdcp_msg_in.header.message_type;
  iv[7] = rdcp_msg_in.header.rdcp_payload_length;
  for (int i=0; i<additional_data_size; i++) additional_data[i] = iv[i];

  for (int i=0; i<rdcp_msg_in.header.rdcp_payload_length - 16; i++) ciphertext[i] = rdcp_msg_in.payload.data[i];
  for (int i=0; i<16; i++) gcmauthtag[i] = rdcp_msg_in.payload.data[rdcp_msg_in.header.rdcp_payload_length - 16 + i];

  if (!decrypt_aes256gcm(ciphertext, rdcp_msg_in.header.rdcp_payload_length - 16,
      additional_data, additional_data_size, getHQsharedSecret(), 32, iv, 12, gcmauthtag, 16, plaintext))
  {
    serial_writeln("WARNING: Authenticated decryption failed - bad key oder data");
    return;
  }

  uint8_t subtype = plaintext[0];
  uint16_t refnr = plaintext[1] + 256 * plaintext[2];
  uint16_t lifetime = plaintext[3] + 256 * plaintext[4];
  uint8_t morefragments = plaintext[5];
  uint8_t uuContent[256];

  if (subtype != RDCP_MSGTYPE_OA_SUBTYPE_UPDATE)
  {
    for (int i=0; i<rdcp_msg_in.header.rdcp_payload_length - 16 - 6; i++) uuContent[i] = plaintext[i+6];
    uuContent[rdcp_msg_in.header.rdcp_payload_length - 16 - 6] = 0;

    char info[512];
    int msg_len = rdcp_msg_in.header.rdcp_payload_length - 16 - 6; // without GCM Tag and OA Subheader
    char oa[1024];
    unsigned int len = unishox2_decompress_simple((char*)uuContent, msg_len, oa);
    oa[len] = 0;

    snprintf(info, 512, "INFO: Received Private OA, type %02X, refnr %04X, content: %s", subtype, refnr, oa);
    serial_writeln(info);

    if (subtype == RDCP_MSGTYPE_OA_SUBTYPE_CRISIS_TXT)
    {
      mb_add_external_message(oa, get_current_rdcp_msg_base64(), rdcp_msg_in.header.origin, rdcp_msg_in.header.sequence_number,
       refnr, morefragments, lifetime, RELEVANT_FOR_DISPLAYING, RELEVANCE_FOR_THIS_DEVICE_ONLY, true, true, subtype);
      if (gui_get_current_screen() == SCREEN_OANONCRISIS) gui_transition_to_screen(SCREEN_OACRISIS);
    }
    else if (subtype == RDCP_MSGTYPE_OA_SUBTYPE_NONCRISIS)
    {
      mb_add_external_message(oa, get_current_rdcp_msg_base64(), rdcp_msg_in.header.origin, rdcp_msg_in.header.sequence_number,
       refnr, morefragments, lifetime, RELEVANT_FOR_DISPLAYING, RELEVANCE_FOR_THIS_DEVICE_ONLY, false, true, subtype);
      if (gui_get_current_screen() != SCREEN_OANONCRISIS) gui_transition_to_screen(SCREEN_OANONCRISIS);
    }
    else if (subtype == RDCP_MSGTYPE_OA_SUBTYPE_FEEDBACK)
    {
      mb_add_external_message(oa, get_current_rdcp_msg_base64(), rdcp_msg_in.header.origin, rdcp_msg_in.header.sequence_number,
       refnr, morefragments, lifetime, RELEVANT_FOR_DISPLAYING, RELEVANCE_FOR_THIS_DEVICE_ONLY, true, true, subtype);
      if (gui_get_current_screen() == SCREEN_OANONCRISIS) gui_transition_to_screen(SCREEN_OACRISIS);
    }
    else if (subtype == RDCP_MSGTYPE_OA_SUBTYPE_INQUIRY)
    {
      mb_add_external_message(oa, get_current_rdcp_msg_base64(), rdcp_msg_in.header.origin, rdcp_msg_in.header.sequence_number,
       refnr, morefragments, lifetime, RELEVANT_FOR_DISPLAYING, RELEVANCE_FOR_THIS_DEVICE_ONLY, true, true, subtype);

      lv_textarea_set_text(ui_TextAreaRESPoa, oa);
      lv_textarea_set_text(ui_TextAreaRESPFreetext, "");

      gui_switch_red_button_mode(RED_BUTTON_MODE_INQUIRY);
      set_current_inquiry_refnr(refnr);
      if (gui_get_current_screen() == SCREEN_OANONCRISIS) gui_transition_to_screen(SCREEN_OACRISIS);
    }
  }
  else
  { // Private OA lifetime update
    mb_update_lifetime(rdcp_msg_in.header.origin, refnr, lifetime);
  }

  return;
}

uint16_t current_inquiry_refnr = 0x0000;
uint16_t get_current_inquiry_refnr(void)
{
  return current_inquiry_refnr;
}
void set_current_inquiry_refnr(uint16_t value)
{
  current_inquiry_refnr = value;
  return;
}

void rdcp_mg_process_incoming_public_oa(void)
{
  /* Official Announcements to broadcast/multicast addresses are unencrypted, but have separate signatures. */
  uint8_t subtype = rdcp_msg_in.payload.data[0];
  uint16_t refnr = rdcp_msg_in.payload.data[1] + 256 * rdcp_msg_in.payload.data[2];
  uint16_t lifetime = rdcp_msg_in.payload.data[3] + 256 * rdcp_msg_in.payload.data[4];
  uint8_t morefragments = rdcp_msg_in.payload.data[5];
  uint8_t uuContent[256];

  if (subtype != RDCP_MSGTYPE_OA_SUBTYPE_UPDATE)
  {
    for (int i=0; i<rdcp_msg_in.header.rdcp_payload_length - 6; i++) uuContent[i] = rdcp_msg_in.payload.data[i+6];
    uuContent[rdcp_msg_in.header.rdcp_payload_length - 6] = 0;

    char info[512];
    int msg_len = rdcp_msg_in.header.rdcp_payload_length - 6; // without OA Subheader
    char oa[1024];
    unsigned int len = unishox2_decompress_simple((char*)uuContent, msg_len, oa);
    oa[len] = 0;

    snprintf(info, 512, "INFO: Received Public OA, type %02X, refnr %04X, content: %s", subtype, refnr, oa);
    serial_writeln(info);

    if (subtype == RDCP_MSGTYPE_OA_SUBTYPE_CRISIS_TXT)
    {
      mb_add_external_message(oa, get_current_rdcp_msg_base64(), rdcp_msg_in.header.origin, rdcp_msg_in.header.sequence_number,
       refnr, morefragments, lifetime, RELEVANT_FOR_DISPLAYING, RELEVANCE_FOR_EVERYONE, true, false, subtype);
      if (gui_get_current_screen() == SCREEN_OANONCRISIS) gui_transition_to_screen(SCREEN_OACRISIS);
    }
    else if (subtype == RDCP_MSGTYPE_OA_SUBTYPE_NONCRISIS)
    {
      mb_add_external_message(oa, get_current_rdcp_msg_base64(), rdcp_msg_in.header.origin, rdcp_msg_in.header.sequence_number,
       refnr, morefragments, lifetime, RELEVANT_FOR_DISPLAYING, RELEVANCE_FOR_EVERYONE, false, false, subtype);
      if (gui_get_current_screen() != SCREEN_OANONCRISIS) gui_transition_to_screen(SCREEN_OANONCRISIS);
    }
    else if (subtype == RDCP_MSGTYPE_OA_SUBTYPE_INQUIRY)
    {
      /*
          INQUIRY messages should only be sent to a single device and thus not be handled here.
          However, this might become relevant for later use-cases.
      */
      mb_add_external_message(oa, get_current_rdcp_msg_base64(), rdcp_msg_in.header.origin, rdcp_msg_in.header.sequence_number,
       refnr, morefragments, lifetime, RELEVANT_FOR_DISPLAYING, RELEVANCE_FOR_EVERYONE, true, false, subtype);

      lv_textarea_set_text(ui_TextAreaRESPoa, oa);
      lv_textarea_set_text(ui_TextAreaRESPFreetext, "");

      gui_switch_red_button_mode(RED_BUTTON_MODE_INQUIRY);
      set_current_inquiry_refnr(refnr);
      if (gui_get_current_screen() == SCREEN_OANONCRISIS) gui_transition_to_screen(SCREEN_OACRISIS);
    }
  }
  else
  { // Public OA lifetime update
    mb_update_lifetime(rdcp_msg_in.header.origin, refnr, lifetime);
  }

  return;
}

void rdcp_mg_process_signature(void)
{
  uint16_t refnr = rdcp_msg_in.payload.data[0] + 256 * rdcp_msg_in.payload.data[1];
  uint8_t schnorrsig[128];
  for (int i=0; i<rdcp_msg_in.header.rdcp_payload_length - 2; i++) schnorrsig[i] = rdcp_msg_in.payload.data[i+2];

  char info[128];
  snprintf(info, 128, "INFO: Received SIGNATURE for %04X-%04X", rdcp_msg_in.header.origin, refnr);
  serial_writeln(info);

  mb_add_signature(schnorrsig, rdcp_msg_in.header.origin, refnr);

  return;
}

void rdcp_mg_process_resetoa(void)
{
  bool valid = false;

  if (rdcp_msg_in.header.rdcp_payload_length != RDCP_SIGNATURE_LENGTH)
  {
    serial_writeln("WARNING: RESET OA message has invalid payload length");
    return;
  }

  uint8_t sha[32];
  get_inline_hash(&rdcp_msg_in, 0, sha);

  uint8_t sig[128];
  for (int i=0; i<65; i++) sig[i] = rdcp_msg_in.payload.data[0+i];

  valid = schnorr_verify_signature(sha, 32, sig);
  if (valid)
  {
    mb_clear_history();
    mb_count_and_show_last();
  }
  else
  {
    serial_writeln("WARNING: Invalid HQ Schnorr signature for RDCP RESET of OA - ignoring");
  }

  return;
}

void rdcp_mg_process_resetinfrastructure(void)
{
  bool valid = false;

  if (rdcp_msg_in.header.rdcp_payload_length != 2 + RDCP_SIGNATURE_LENGTH)
  {
    serial_writeln("WARNING: RESET INFRA message has invalid payload length");
    return;
  }

  uint8_t sha[32];
  get_inline_hash(&rdcp_msg_in, 2, sha);

  uint8_t sig[128];
  for (int i=0; i<65; i++) sig[i] = rdcp_msg_in.payload.data[2+i];

  valid = schnorr_verify_signature(sha, 32, sig);
  if (valid)
  {
    uint16_t nonce = rdcp_msg_in.payload.data[0] + 256 * rdcp_msg_in.payload.data[1];

    if (!persistence_checkset_nonce("rstinfra", nonce))
    {
      serial_writeln("WARNING: Invalid nonce received for signed RDCP RESET of INFRASTRUCTURE");
      return;
    }
    else
    {
      serial_writeln("INFO: Performing RESET OF INFRASTRUCTURE on this device");
    }

    set_next_rdcp_sequence_number(getMyRDCPAddress(), 1); // only on infrastructure, not device reset
    rdcp_reset_duplicate_message_table();
    mb_clear_history();
    ESP.restart();
  }
  else
  {
    serial_writeln("WARNING: Invalid HQ Schnorr signature for RDCP RESET of INFRASTRUCTURE - ignoring");
  }

  return;
}

void rdcp_mg_process_resetdevice(void)
{
  bool valid = false;

  if (rdcp_msg_in.header.rdcp_payload_length != 2 + RDCP_SIGNATURE_LENGTH)
  {
    serial_writeln("WARNING: RESET DEVICE message has invalid payload length");
    return;
  }

  uint8_t sha[32];
  get_inline_hash(&rdcp_msg_in, 2, sha);

  uint8_t sig[128];
  for (int i=0; i<65; i++) sig[i] = rdcp_msg_in.payload.data[2+i];

  valid = schnorr_verify_signature(sha, 32, sig);
  if (valid)
  {
    uint16_t nonce = rdcp_msg_in.payload.data[0] + 256 * rdcp_msg_in.payload.data[1];

    if (!persistence_checkset_nonce("rstdev", nonce))
    {
      serial_writeln("WARNING: Invalid nonce received for signed RDCP RESET of DEVICE");
      return;
    }
    else
    {
      serial_writeln("INFO: Performing RESET OF DEVICE");
    }

    rdcp_reset_duplicate_message_table();
    mb_clear_history();
    ESP.restart();
  }
  else
  {
    serial_writeln("WARNING: Invalid HQ Schnorr signature for RDCP RESET of DEVICE - ignoring");
  }

  return;
}

void rdcp_mg_process_rebootdevice(void)
{
  bool valid = false;

  if (rdcp_msg_in.header.rdcp_payload_length != 2 + RDCP_SIGNATURE_LENGTH)
  {
    serial_writeln("WARNING: REBOOT DEVICE message has invalid payload length");
    return;
  }

  uint8_t sha[32];
  get_inline_hash(&rdcp_msg_in, 2, sha);

  uint8_t sig[128];
  for (int i=0; i<65; i++) sig[i] = rdcp_msg_in.payload.data[2+i];

  valid = schnorr_verify_signature(sha, 32, sig);
  if (valid)
  {
    uint16_t nonce = rdcp_msg_in.payload.data[0] + 256 * rdcp_msg_in.payload.data[1];

    if (!persistence_checkset_nonce("rstdev", nonce))
    {
      serial_writeln("WARNING: Invalid nonce received for signed RDCP DEVICE REBOOT");
      return;
    }
    else
    {
      serial_writeln("INFO: Performing DEVICE REBOOT");
    }

    ESP.restart();
  }
  else
  {
    serial_writeln("WARNING: Invalid HQ Schnorr signature for RDCP DEVICE REBOOT - ignoring");
  }

  return;
}

void rdcp_mg_process_maintenance(void)
{
  bool valid = false;

  if (rdcp_msg_in.header.rdcp_payload_length != RDCP_SIGNATURE_LENGTH)
  {
    serial_writeln("WARNING: MAINTENANCE message has invalid payload length");
    return;
  }

  uint8_t sha[32];
  get_inline_hash(&rdcp_msg_in, 0, sha);

  uint8_t sig[128];
  for (int i=0; i<65; i++) sig[i] = rdcp_msg_in.payload.data[0+i];

  valid = schnorr_verify_signature(sha, 32, sig);
  if (valid)
  {
    gui_transition_to_screen(SCREEN_MAINTENANCE);
    serial_set_serial_mode(SERIAL_MODE_FULLACCESS);
  }
  else
  {
    serial_writeln("WARNING: Invalid HQ Schnorr signature for RDCP MAINTENANCE - ignoring");
  }

  return;
}

#define FILENAME_DEVLOCK "/devlock"
bool device_is_locked = false;

struct lockentry {
  uint16_t duration = 0;
  int64_t time_added = 0;
  time_t absexp = 0;
};

lockentry locke;

void rdcp_blockdevice_lock(uint16_t duration)
{
  gui_disable_cire_buttons();
  device_is_locked = true;

  locke.duration = duration;
  locke.time_added = my_millis();
  locke.absexp = tdeck_get_time() ? tdeck_get_time() + 60*duration : 0;

  if (hasStorage)
  {
    File f = FFat.open(FILENAME_DEVLOCK, FILE_WRITE);
    if (f)
    {
      f.write((uint8_t*)&locke, sizeof(lockentry));
      f.close();
    }
  }

  return;
}

void rdcp_blockdevice_unlock(void)
{
  gui_enable_cire_buttons();
  if (hasStorage)
  {
    FFat.remove(FILENAME_DEVLOCK);
  }
  device_is_locked = false;

  return;
}

void rdcp_blockdevice_check(void)
{
  if (!device_is_locked)
  {
    if (hasStorage)
    {
      File f = FFat.open(FILENAME_DEVLOCK, FILE_READ);
      if (f)
      {
        f.read((uint8_t*)&locke, sizeof(lockentry));
        f.close();
        device_is_locked = true;
        gui_disable_cire_buttons();
      }
    }
  }

  if (device_is_locked)
  {
    time_t now = tdeck_get_time();
    if (now > 0) // device has wallclock time
    {
      if (locke.absexp < now) rdcp_blockdevice_unlock(); // absolute timer expired
    }
    if (my_millis() < locke.time_added) locke.time_added = my_millis(); // reset monotonic counter after power cycle
    if (my_millis() > locke.time_added + 60 * locke.duration) rdcp_blockdevice_unlock(); // relative timer expired
  }

  return;
}

void rdcp_mg_process_blockalert(void)
{
  bool valid = false;

  if (rdcp_msg_in.header.rdcp_payload_length != 4 + RDCP_SIGNATURE_LENGTH)
  {
    serial_writeln("WARNING: BLOCK ALERT message has invalid payload length");
    return;
  }

  uint8_t sha[32];
  get_inline_hash(&rdcp_msg_in, 4, sha);

  uint8_t sig[128];
  for (int i=0; i<65; i++) sig[i] = rdcp_msg_in.payload.data[4+i];

  valid = schnorr_verify_signature(sha, 32, sig);
  if (valid)
  {
    uint16_t nonce = rdcp_msg_in.payload.data[0] + 256 * rdcp_msg_in.payload.data[1];

    if (!persistence_checkset_nonce("rstdev", nonce))
    {
      serial_writeln("WARNING: Invalid nonce received for signed RDCP BLOCK ALERT");
      return;
    }
    else
    {
      uint16_t targetdevice = rdcp_msg_in.payload.data[0] + 256 * rdcp_msg_in.payload.data[1];
      uint16_t duration = rdcp_msg_in.payload.data[2] + 256 * rdcp_msg_in.payload.data[3];

      if (targetdevice == getMyRDCPAddress())
      {
        if (duration > 0)
        {
          serial_writeln("INFO: BLOCK DEVICE ALERT affects this device, restricting usage");
          rdcp_blockdevice_lock(duration);
        }
        else
        {
          serial_writeln("INFO: BLOCK DEVICE lifted by HQ");
          rdcp_blockdevice_unlock();
        }
      }

    }
  }
  else
  {
    serial_writeln("WARNING: Invalid HQ Schnorr signature for RDCP BLOCK ALERT - ignoring");
  }

  return;
}

/* EOF */
