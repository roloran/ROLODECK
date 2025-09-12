#ifndef _ROLORAN_RDCP
#define _ROLORAN_RDCP

#include <Arduino.h>
#include "roloran-crypto.h"
#include "settings-scenario.h"

/// RDCP v0.4 fixed header size
#define RDCP_HEADER_SIZE 16
/// RDCP v0.4 fixed signature size
#define RDCP_SIGNATURE_LENGTH 65

/// RDCP v0.4 fixed broadcast address
#define RDCP_BROADCAST_ADDRESS    0xFFFF
/// RDCP v0.4 fixed HQ multicast address
#define RDCP_HQ_MULTICAST_ADDRESS 0x00FF

/// RDCP v0.4 fixed maximum LoRa Payload size
#define RDCP_MAX_PAYLOAD_SIZE 200

/// Constants for both LoRa channels
#define CHANNEL433 0
#define CHANNEL868 1

/// Magic values for TX scheduling; > 0 means delay in milliseconds
#define TX_IMMEDIATELY -1
#define TX_WHEN_CF 0

/// RDCP v0.4 uses a 1.000 ms pause between retransmission of the same message. Depending on average hardware performance, we have to sleep much shorter.
#define PAUSE_BETWEEN_RETRANSMISSIONS_ON_THIS_DEVICE 750

/**
  * Data structure for an RDCP v0.4 Header
  */
struct rdcp_header {
  uint16_t sender;              //< current sender of the message (may be a relay)
  uint16_t origin;              //< original sender of the message (who created it)
  uint16_t sequence_number;     //< for detecting duplicates (specific for "origin")
  uint16_t destination;         //< intended destination (may be the broadcast address)
  uint8_t  message_type;        //< type of RDCP message according to specification
  uint8_t  rdcp_payload_length; //< the length of the inner RDCP payload
  uint8_t  counter;             //< retransmission counter
  uint8_t  relay1;              //< relay 1 designation, delay 1 assignment
  uint8_t  relay2;              //< relay 2 designation, delay 2 assignment
  uint8_t  relay3;              //< relay 3 designation, delay 3 assignment
  uint16_t checksum;            //< CRC-16 (CCITT) checksum
};

/**
  * Data structure for storing the RDCP v0.4 Payload of an RDCP Message
  */
struct rdcp_payload {
  uint8_t data[RDCP_MAX_PAYLOAD_SIZE];            // RDCP payload must not exceed 200 bytes
};

/**
  * Data structure for an RDCP v0.4 Message, consisting of RDCP Header and RDCP Payload
  */
struct rdcp_message {
  struct rdcp_header  header;
  struct rdcp_payload payload;
};

/*
 * RDCP v0.4 Message Type definitions
 */
#define RDCP_MSGTYPE_TEST                    0x00
#define RDCP_MSGTYPE_ECHO_REQUEST            0x01
#define RDCP_MSGTYPE_ECHO_RESPONSE           0x02
#define RDCP_MSGTYPE_BBK_STATUS_REQUEST      0x03
#define RDCP_MSGTYPE_BBK_STATUS_RESPONSE     0x04
#define RDCP_MSGTYPE_DA_STATUS_REQUEST       0x05
#define RDCP_MSGTYPE_DA_STATUS_RESPONSE      0x06
#define RDCP_MSGTYPE_TRACEROUTE_REQUEST      0x07
#define RDCP_MSGTYPE_TRACEROUTE_RESPONSE     0x08
#define RDCP_MSGTYPE_DEVICE_BLOCK_ALERT      0x09
#define RDCP_MSGTYPE_TIMESTAMP               0x0a
#define RDCP_MSGTYPE_DEVICE_RESET            0x0b
#define RDCP_MSGTYPE_DEVICE_REBOOT           0x0c
#define RDCP_MSGTYPE_MAINTENANCE             0x0d
#define RDCP_MSGTYPE_INFRASTRUCTURE_RESET    0x0e
#define RDCP_MSGTYPE_ACK                     0x0f

#define RDCP_MSGTYPE_OFFICIAL_ANNOUNCEMENT   0x10
#define RDCP_MSGTYPE_RESET_ALL_ANNOUNCEMENTS 0x11

#define RDCP_MSGTYPE_CITIZEN_REPORT          0x1a
#define RDCP_MSGTYPE_PRIVILEGED_REPORT       0x1c

#define RDCP_MSGTYPE_FETCH_ALL_NEW_MESSAGES  0x20
#define RDCP_MSGTYPE_FETCH_MESSAGE           0x21
#define RDCP_MSGTYPE_DELIVERY_RECEIPT        0x2a
#define RDCP_MSGTYPE_SCHEDULE_RCPT           0x2b

#define RDCP_MSGTYPE_SIGNATURE               0x30
#define RDCP_MSGTYPE_HEARTBEAT               0x31

/*
 * Subtypes for OFFICIAL ANNOUNCEMENTs
 */
#define RDCP_MSGTYPE_OA_SUBTYPE_RESERVED     0x00
#define RDCP_MSGTYPE_OA_SUBTYPE_NONCRISIS    0x10
#define RDCP_MSGTYPE_OA_SUBTYPE_CRISIS_TXT   0x20
#define RDCP_MSGTYPE_OA_SUBTYPE_CRISIS_GFX   0x21
#define RDCP_MSGTYPE_OA_SUBTYPE_UPDATE       0x22
#define RDCP_MSGTYPE_OA_SUBTYPE_FEEDBACK     0x30
#define RDCP_MSGTYPE_OA_SUBTYPE_INQUIRY      0x31

/*
 * Subtypes for CITIZEN REPORTs
 */
#define RDCP_MSGTYPE_CIRE_SUBTYPE_EMERGENCY  0x00
#define RDCP_MSGTYPE_CIRE_SUBTYPE_REQUEST    0x01
#define RDCP_MSGTYPE_CIRE_SUBTYPE_RESPONSE   0x02

/*
 * Subtypes for ACKNOWLEDGMENTs
 */
#define RDCP_ACKNOWLEDGMENT_POSITIVE         0x00
#define RDCP_ACKNOWLEDGMENT_NEGATIVE         0x01
#define RDCP_ACKNOWLEDGMENT_POSNEG           0x02

/**
  * Toggle whether RDCP Messages are checked for duplicates based on Origin and SequenceNumber.
  * @return true if duplicates are now ignored; false otherwise.
  */
bool rdcp_toggle_ignore_duplicates(void);

/**
  * Calculate the CRC-16 (CCITT) checksum of a byte array with given length
  * @param data The data to calculate the checksum for 
  * @param len length of the data 
  * @return uint16_t CRC-16 checksum
  */
uint16_t crc16(uint8_t *data, uint16_t len);

/**
  * Verify that the CRC-16 checksum of the most recently received RDCP Message is correct.
  * Needs the actual received packet length as parameter because the corresponding RDCP Header field might be corrupted.
  * @param real_packet_length Received LoRa packet payload length; may differ from RDCP header size + RDCP payload size
  * @return true if the CRC-16 in the RDCP Message is valid; false otherwise
  */
bool rdcp_check_crc_in(uint8_t real_packet_length);

/**
  * Process a received LoRa packet. Needs the byte array of the received LoRa packet payload, its length, and the
  * timestamp when the packet was received as parameters.
  * @param lora_packet The received LoRa packet payload 
  * @param lora_packet_length Length of the payload of the received LoRa packet 
  * @param lora_packet_timestamp Timestamp of when the LoRa packet was received (monotonic clock)
  * @return true if the RDCP message was successfully processed, false otherwise (e.g., bad CRC, too short to be RDCP, ...)
  */
bool rdcp_mg_process_rxed_lora_packet(uint8_t *lora_packet, uint16_t lora_packet_length, int64_t lora_packet_timestamp);

/**
  * Data structure for Duplicate Table entries
  */
struct rdcp_dup_table_entry {
  uint16_t origin = RDCP_NO_ADDRESS; //< Origin from the RDCP Header
  uint16_t sequence_number = RDCP_NO_SEQUENCE_NUMBER; //< SequenceNumber from the RDCP Header
  int64_t last_seen = NO_TIMESTAMP;  //< Timestamp of when the entry was last updated
};

/**
  * Data structure for the overall Duplicate Table
  */
struct rdcp_dup_table {
  unsigned short num_entries = 0;              //< Number of currently stored entries
  struct rdcp_dup_table_entry tableentry[256]; //< Array of Duplicate Table entries
};

/**
  * Resets the Duplicate Table by clearing all entries
  */
void rdcp_reset_duplicate_message_table(void);

/**
  * Checks whether an RDCP Message with Origin and SequenceNumber given as parameters
  * should be treated as duplicate (returns true) or new (returns false).
  * @param origin RDCP Origin address of the RDCP Message to check for duplicate 
  * @param sequence_number RDCP Header SequenceNumber of the RDCP Message to check for duplicate 
  * @return true if the message is a duplicate, false if it was not seen before
  */
bool rdcp_check_duplicate_message(uint16_t origin, uint16_t sequence_number);

/**
  * Updates the Channel Free Estimator (CFEst) timestamp value based on a received
  * RDCP Message, especially its RetransmissionCounter / Relay1-3 settings.
  * Used internally.
  */
void rdcp_update_channel_free_estimator_rx(void);

/**
  * Processes an incoming RDCP Message based on what RDCP mobile devices have to do.
  * Actually a subset of rdcp_mg_process_rxed_lora_packet(...), which handles previous steps such as
  * checksum verification and duplicate elimination and then calls this function.
  * Used internally.
  * @param is_duplicate true if message is a duplicate based on Origin and SequenceNumber, false otherwise
  */
void rdcp_mg_process_incoming_message(bool is_duplicate);

/**
  * Returns the current timestamp value of CFEst (Channel Free Estimator).
  * @return int64_t timestamp (monotonic clock) of when the channel is expected be free again
  */
int64_t rdcp_get_channel_free_estimation(void);

/**
  * Sets CFEst (Channel Free Estimator) to a new absolute timestamp.
  * Returns the result of the implicit rdcp_txqueue_reschedule() call.
  * @param new_value New absolute timestamp (monotonic clock) of when to expect channel to be free again. 
  * @return true if scheduled TX entries had to be postponed/deleted due to a longer busy channel; false if no changes to schedule happened. 
  */
bool rdcp_set_channel_free_estimation(int64_t new_value);

/**
  * Updates CFEst (Channel Free Estimator) to a new timestamp, but only if the new value is further in the future.
  * Returns true if change was made, false otherwise.
  * @param new_value New absolute timestamp (monotonic clock) of when to expect channel to be free again. 
  * @return true if the new value was set; false if the channel was known to be busy even longer before.
  */
bool rdcp_update_channel_free_estimation(int64_t new_value);

/**
  * Calculate the airtime (in milliseconds) of sending a LoRa packet with the payload size
  * given as parameter under consideration of the currently used LoRa settings, such as
  * bandwidth, coding rate, and preamble length.
  * @param payload_size Number of bytes for the LoRa packet payload (e.g., RDCP Message including Header and Payload)
  * @return Calculated airtime in milliseconds based on current LoRa radio parameters (e.g., SF, bandwidth)
  */
uint16_t airtime_in_ms(uint8_t payload_size);

/**
  * Add an outgoing RDCP Message to the TX Queue (TXQ).
  * TXQ is used for messages that are up for transmission very soon, unlike the
  * TX Ahead Queue, which is used for intermediate-term scheduling. 
  * The LoRa payload is given as data along with its length. Parameter `important`
  * is set to true if the message should not be dropped even if re-scheduled too often
  * or delayed too long. `force_tx` prohibits re-scheduling. The `callback_selector`
  * determines which function is called when the message has been sent including all of
  * its retransmissions. If `force_tx` is used, the `forced_time` should be given. 
  * @param data Complete RDCP Message (header+payload) to schedule
  * @param len Length of data (RDCP Message) in bytes
  * @param important True if the message is so important that it must not be dropped even after multiple re-schedules
  * @param force_tx True if the message must be sent at its scheduled time, i.e., must not be re-scheduled
  * @param callback_selector Number of the callback to trigger after TX, e.g. TX_CALLBACK_NONE
  * @param forced_time Used in combination with force_tx to specify the TX start time
  * @return true if message was accepted, false otherwise (e.g., queue full)
  */
bool rdcp_txqueue_add(uint8_t *data, uint8_t len, bool important, bool force_tx, uint8_t callback_selector, int64_t forced_time);

/**
  * Re-schedule the entries in the TX Queue because CFEst has changed meanwhile (offset=0) or by a given offset.
  * Returns true if entries were dropped due to resecheduling, or false if all entries are still there.
  * Calling this function over and over again implicitly drops messages not marked as important and can
  * free up space in the TX Queue.
  * @param offset Relative time offset to re-schedule all entries by, or 0 if CFEst has changed and serves as baseline
  * @return true if it at least one scheduled message was dropped due to excessive re-scheduling or postponing 
  */
bool rdcp_txqueue_reschedule(int64_t offset);

/**
  * RDCP TX Queue Loop to be called periodically to start sending outgoing
  * messages when the channel is free. This should be called as a part of an overall
  * Arduino-style loop() for the whole device; otherwise, no TX ever starts. 
  * @return true If a message is prepared to be sent now; false if no TX is up ahead.
  */
bool rdcp_txqueue_loop(void);

/// A magic timestamp of NO_FORCED_TIME is used to signal that TX should happen when channel is free again
#define NO_FORCED_TIME 0

/// IMPORTANT and NONIMPORTANT are just parameter aliases for true and false
#define IMPORTANT true
#define NOTIMPORTANT false 

/// FORCEDTX and NOFORCEDTX are just parameter aliases for true and false
#define FORCEDTX true 
#define NOTFORCEDTX false

/// Numeric identifiers for available callback functions
#define TX_CALLBACK_NONE 0
#define TX_CALLBACK_CIRE 1

/**
  * Data structure for a TX Queue entry.
  */
struct txqueue_entry {
  uint8_t payload[RDCP_HEADER_SIZE + RDCP_MAX_PAYLOAD_SIZE];       //< data of the outgoing message
  uint8_t payload_length = 0;                   //< length of the outgoing message
  int64_t currently_scheduled_time = NO_TIMESTAMP;         //< timestamp when to transmit
  int64_t originally_scheduled_time = NO_TIMESTAMP;        //< timestamp when originally planned to transmit
  uint8_t num_of_reschedules = 0;               //< how often the entry has already been rescheduled
  bool important = false;                       //< message is important and should not be dropped even it if takes longer
  bool force_tx = false;                        //< indicator whether message should be sent independend of CAD status
  uint8_t callback_selector = TX_CALLBACK_NONE; //< which callback function to use when TX is finished
  int64_t timeslot_duration =  NO_DURATION;               //< timeslot duration in milliseconds including retransmissions
  uint8_t cad_retry = 0;                        //< CAD retry attempt number
  bool waiting = false;                         //< message is still waiting to be sent
  bool in_process = false;                      //< this message is currently being processed
};

/// Keep the TX Queue small on purpose. We don't want single devices to block the channel for too long.
#define MAX_TXQUEUE_ENTRIES 8

/**
  * Data structure for the overall TX Queue.
  */
struct txqueue {
  uint8_t num_entries = 0;
  struct txqueue_entry entries[MAX_TXQUEUE_ENTRIES];
};

/**
  * Schedule an outgoing RDCP Message in the "ahead of time" queue.
  * This is intended for intermediate-term scheduling, like sending a SIGNATURE after a corresponding RDCP Message. 
  * Periodic retransmissions should not be added to any queue unless the LoRa channel is considered free currently. 
  * @param data LoRa packet payload (i.e., RDCP Header + RDCP Payload)
  * @param len Length of data in bytes 
  * @param important IMPORTANT if the message must not be dropped, otherwise NOTIMPORTANT 
  * @param forcetx FORCEDTX if the message has to be sent at its scheduled time, NOTFORCEDTX if it may be re-scheduled 
  * @param callback_selector Number of the callback to use, e.g., TX_CALLBACK_NONE
  * @param delay_in_ms Relative monotonic clock timestamp of when to move the message to the TX queue (in milliseconds)
  * @return true if the message was accepted in the queue, false otherwise (e.g., queue full)
  */
bool rdcp_txaheadqueue_add(uint8_t *data, uint8_t len, bool important, bool force_tx, uint8_t callback_selector, int64_t delay_in_ms);

/**
  * Data structure for a TX Ahead Queue entry.
  */
struct txaheadqueue_entry {
  uint8_t payload[RDCP_MAX_PAYLOAD_SIZE];       //< data of the outgoing message
  uint8_t payload_length = 0;                   //< length of the outgoing message
  int64_t scheduled_time = NO_TIMESTAMP;        //< timestamp when to move to the TX Queue
  bool important = false;                       //< message is important and should not be dropped even it if takes longer
  bool force_tx = false;                        //< indicator whether message should be sent independend of CAD status
  uint8_t callback_selector = TX_CALLBACK_NONE; //< which callback function to use when TX is finished
  bool waiting = false;                         //< message is still waiting to be sent
};

/// The length of the TX Ahead Queue is larger than the length of the TX Queue. Still, less queued messages is better.
#define MAX_TXAHEADQUEUE_ENTRIES 16

/**
  * Data structure for the overall TX Ahead Queue.
  */
struct txaheadqueue {
  uint8_t num_entries = 0;
  struct txaheadqueue_entry entries[MAX_TXAHEADQUEUE_ENTRIES];
};

/**
  * RDCP TX Ahead Queue Loop to be called implicitly by the TX Queue Loop.
  * Calling this manually should be used scarcely. 
  * @return true if a messages was moved to the TX queue, false otherwise
  */
bool rdcp_txaheadqueue_loop(void);

/**
  * Return the duration of a full timeslot given an RDCP Message based
  * on its Message Type (i.e. initial RetransmissionCounter) and size.
  * @param data RDCP Message to analyse. Must at least be an RDCP Header with rdcp_payload_size field. 
  * @return Timeslot duration in milliseconds (when applying the number of retransmissions according to RDCP specs)
  */
int64_t rdcp_get_timeslot_duration(uint8_t *data);

/**
 * Hand over an RDCP Message for transmission (TX). 
 * 
 * The message is passed as Base64-encoded String. 
 * 
 * The timestamp is given as my_millis()-style value for scheduling,
 * or TX_IMMEDIATELY for immediate sending, 
 * or TX_WHEN_CF to send when the channel is free.
 * 
 * The channel must be either CHANNEL433 or CHANNEL868.
 * 
 * This interface is intended for Serial/UART usage, as it only gives
 * limited control over the used TX (Ahead) Queue settings.
 * @param b64rdcpmsg The RDCP Message to send, Base64-encoded
 * @param time When to send: TX_IMMEDIATELY, TX_WHEN_CF (channel free), or delay in milliseconds 
 * @param channel Either CHANNEL433 or CHANNEL868 
 * @return true if the message was accepted, false otherwise (e.g., too short to be RDCP)
 */
bool rdcp_tx_interface(String b64rdcpmsg, int64_t time, uint8_t channel);

/**
 * Start gracefully transmitting the currently `tx_ongoing` RDCP Message
 * by applying Channel Activity Detection first, which might seriously
 * delay the start of the transmission.
 */
void rdcp_send_message_cad(void);

/**
 * Send the `tx_ongoing` RDCP Message from TX Queue now. 
 * This is implicitly used for `force_tx`-scheduled messages or when
 * Channel Activity Detection has delayed for too long without
 * non-important messages getting dropped in the process.
 * @return true if the message was accepted for sending, false otherwise
 */
bool rdcp_send_message_force(void);

/**
 * Callback when a LoRa TX event has finished. This is an additional
 * RDCP-specific callback function to be called by the underlying
 * LoRa library. It automatically re-transmits an RDCP Message if its
 * previous retransmission counter was > 0 (after adjusting its Checksum)
 * or removes it from the TX Queue at the end of a timeslot. 
 */
void rdcp_callback_txfin(void);

/**
 * Callback when a LoRa CAD event has results. If the channel is free,
 * transmission of the `tx_ongoing` RDCP Message starts. Otherwise,
 * repeated CAD attempts and re-scheduling processes are triggered
 * according to RDCP specifications.
 * @return true if the message is being sent now, false if CAD delays
 */
bool rdcp_callback_cad(bool cad_busy);

/**
 * Craft and schedule an ECHO REQUEST message to the given Destination address.
 * @param desta RDCP Address of Destination, i.e., whom to ping 
 */
void rdcp_mg_send_echo_request(uint16_t desta);

/**
 * Craft and schedule a TEST message to the given Destination address 
 * with an arbitrary message given as String. 
 * @param desta RDCP Address of Destination, i.e., whom to send the Test message to (may be broadcast)
 * @param m String with the text to include in the Test message payload
 */
void rdcp_mg_send_test_message(uint16_t desta, String m);

/**
 * Craft and schedule a CITIZEN REPORT message. Will be sent to the HQ multicast address. 
 * The subtype can, e.g., be RDCP_MSGTYPE_CIRE_SUBTYPE_EMERGENCY, and the ReferenceNumber 
 * must be given according to RDCP Specs. The content, given as char *text, should be 
 * formatted in a way that is understood by the HQ. Its Unishox2 representation must 
 * not exceed 168 bytes (+16 bytes for the AES-GCM AuthTag + 16 bytes for RDCP Header = 
 * 200 bytes maximum LoRa packet payload size). 
 * @param subtype CIRE subtype, e.g., RDCP_MSGTYPE_CIRE_SUBTYPE_EMERGENCY 
 * @param refnr Reference Number for this CIRE; HQ might use it for FEEDBACK and INQUIRY
 * @param text Textual content of the CIRE. Usually structured text data, depending on what HQ expects. 
 */

void rdcp_send_cire(uint8_t subtype, uint16_t refnr, char *text);

/**
 * Callback to be executed when a CIRE message has been sent by this device. 
 * Usually starts a timer to wait for received ACKs and disables sending more 
 * CIREs while the current one is on its way. 
 */
void rdcp_cire_callback(void);

/**
 * Craft and schedule a signed ACK message from Origin to Destination, giving
 * the acknowledged sequence number and the type of the acknowledgment. 
 * @param origin RDCP Address of who signed ACK Origin
 * @param destination Whom to send the ACK to 
 * @param seqnr Acknowledged SequenceNumber (! - not a ReferenceNumber)
 * @param acktype Acknowledgment type, e.g., RDCP_ACKNOWLEDGMENT_POSITIVE
 */
void rdcp_send_ack_signed(uint16_t origin, uint16_t destination, uint16_t seqnr, uint8_t acktype);

/**
 * Craft and send an ACK message without signature from Origin to Destination,
 * referring to the acknowledged SequenceNumber and the given type of Acknowledgment. 
 * @param origin RDCP Address of ACK Message Origin (e.g., a DA)
 * @param destination RDCP Address of ACK Message Destination (e.g., a MG)
 * @param seqnr Sequence Number of Destination's acknowledged previous message 
 * @param acktype Type of acknowledgment, e.g., RDCP_ACKNOWLEDGMENT_POSITIVE
 * @returns void 
 */
void rdcp_send_ack_unsigned(uint16_t origin, uint16_t destination, uint16_t seqnr, uint8_t acktype);

/**
 * @return Number of messages currently in the TX Queue. 
 */
int get_num_txq_entries(void);

/**
 * @return Number of messages currently in the TX Ahead Queue. 
 */
int get_num_txaq_entries(void);

/**
 * Send a TIMESTAMP message to the broadcast address (spoofing an HQ). 
 * @param year Years since 2025
 * @param month Current month (1--12)
 * @param day Current day (1--31)
 * @param hour Current hour (0--23)
 * @param minute Current minute (0--59)
 * @param status 0 if we are in non-crisis, 1 in crisis, 2 in unmanned-crisis mode
 */
void rdcp_send_timestamp(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t status);

/**
 * Must be periodically called to check whether we received ACKs for CIREs.
 */
void rdcp_cire_check(void);

/**
 * Handle incoming ACKs for CIREs. 
 * @param origin ACK Origin 
 * @param confirmedseqnr SequenceNumber confirmed in the ACK 
 * @param acktype Type of acknowledment
 */
void rdcp_cire_ack(uint16_t origin, uint16_t confirmedseqnr, uint8_t acktype);

/**
 * Needs to be called periodically to determine whether this device should send a HEARTBEAT message.
 */
void rdcp_heartbeat_check(void);

/**
 * Set the interval in which to send HEARTBEAT messages. 
 * @param new_interval Time interval in milliseconds
 */
void set_heartbeat_interval(int64_t new_interval);

/**
 * Set how long to wait after power-on before the first HEARTBEAT message is sent.
 * @param new_grace Grace period in milliseconds
 */
void set_grace_period(int64_t new_grace);

/**
 * Set timeout when waiting for an ACK after a CIRE from the Entry Point DA in seconds. RDCP Specs recommend 60 seconds.
 * @param timeout Timeout waiting for DA ACK in seconds
 */
void set_ciretime_da(uint16_t timeout);

/**
 * Set timeout when waiting for an ACK after a CIRE from the HQ in seconds. RDCP Specs recommend 900 seconds.
 * @param timeout Timeout waiting for HQ ACK in seconds
 */
void set_ciretime_hq(uint16_t timeout);

/**
 * Process an incoming private official announcement (sent to this device only). Used internally.
 * @param is_duplicate true if message is a duplicate based on Origin and SequenceNumber, false otherwise
 */
void rdcp_mg_process_incoming_private_oa(bool is_duplicate);

/**
 * Process an incoming public official announcement. Used internally.
 */
void rdcp_mg_process_incoming_public_oa(void);

/**
 * Get the current inquiry reference number. Used internally.
 * @return ReferenceNumber of the currently active INQUIRY
 */
uint16_t get_current_inquiry_refnr(void);

/**
 * Set the current inquiry reference number. Used internally.
 * @param value ReferenceNumber of the currently active INQUIRY
 */
void set_current_inquiry_refnr(uint16_t value);

/**
 * Handle incoming RDCP SIGNATURE messages. Used internally.
 */
void rdcp_mg_process_signature(void);

/**
 * Handle an RDCP RESET OF OFFICIAL ANNOUNCEMENTS message. Used internally.
 */
void rdcp_mg_process_resetoa(void);

/**
 * Handle an RDCP RESET OF INFRASTRUCTURE message. Used internally.
 */
void rdcp_mg_process_resetinfrastructure(void);

/**
 * Handle an RDCP RESET OF DEVICE message. Used internally.
 */
void rdcp_mg_process_resetdevice(void);

/**
 * Handle an RDCP DEVICE REBOOT message. Used internally.
 */
void rdcp_mg_process_rebootdevice(void);

/**
 * Handle an RDCP MAINTENANCE message. Used internally.
 */
void rdcp_mg_process_maintenance(void);

/**
 * Handle an RDCP BLOCK ALERT message. Used internally.
 */
void rdcp_mg_process_blockalert(void);

/**
 * Periodically called to check whether this device is currently locked.
 */
void rdcp_blockdevice_check(void);

/**
 * Check whether a cirefile is stored. A cirefile is persisted when a 
 * CIRE is sent by the user but no ACK has been received back. If the
 * device is turned on after power loss, the non-ACKed CIRE will be
 * sent again.
 */
void rdcp_check_cirefile(void);

/**
 * Get the current CIRE state. 
 * @return CIRE state
 */
int get_cire_state(void);

/**
 * Dump TXQ und TXAQ over Serial.
 */

void rdcp_dump_queues(void);

void rdcpcsv_logfile_set_status(bool enabled);
void rdcpcsv_logfile_delete(void);
void rdcpcsv_logfile_dump(void);

#endif
/* EOF */
