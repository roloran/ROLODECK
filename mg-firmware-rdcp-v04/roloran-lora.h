#ifndef _ROLORAN_LORA
#define _ROLORAN_LORA

#define TX_QUEUE_LENGTH       32

#include "roloran-rdcp.h"

/**
 * Start listening to incoming LoRa packets.
 * The radio can be on standby, transmitting, or listening. 
 * This function starts listening for incoming LoRa packets. 
 */
void startReceiveMode(void);

/**
 * Fully initialize the LoRa radio with the current settings.
 * To be used during device power-on, usually from the main setup() function. 
 */
void radio_setup(void);

/**
 * Report whether RadioLib initialized the SX1262 and entered receive mode.
 * @return true when the radio is ready, false after an initialization error
 */
bool radio_is_initialized(void);

/**
 * Fully re-initialize the LoRa radio after configuration change
 * and go back into receiving mode.
 * Should be called after LoRa configuration changes or in system states
 * indicating that a radio re-initialization should be performed; for example,
 * when CAD is started but the CAD callback is not executed within reasonable time. 
 */
void radio_apply_new_configuration(void);

/**
 * The radio_loop() function should be executed periodically as part of the main loop()
 * function. It handles everything related to actual transmitting and receiving outside
 * the hardware-triggered interrupt routines. 
 */
void radio_loop();

/**
 * Check whether a new LoRa packet was received meanwhile.
 * @return true if a new LoRa packet is available, false otherwise
 */
bool lora_has_new_message(void);

/**
 * Copy the most recently received LoRa packet into the given buffer.
 * Returns the number of bytes copied.
 * @param destination Buffer where to store the copied received LoRa packet
 * @return uint16_t length of copied LoRa packet in bytes
 */
uint16_t lora_copy_received_message(uint8_t *destination);

/**
 * Get the timestamp of the most recently received LoRa packet. 
 * @return Timestamp as in my_millis() of when the DIO1 RX interrupt occurred
 */
int64_t lora_get_rx_timestamp(void);

/**
 * Start sending a LoRa packet right now (without Channel Activity Detection). 
 * @param data LoRa packet payload 
 * @param len Length of LoRa packet payload in bytes
 * @return true if LoRa packet was accepted for sending, false otherwise 
 */
bool lora_radio_send(uint8_t *data, uint8_t len);

/**
 * Start Channel Activity Detection (CAD, listen-before-talk). 
 * The CAD callback is executed when a result is available. 
 */
void lora_radio_startcad(uint8_t channel=CHANNEL868MG);

/**
 * Switches the LoRa radio into receive mode. 
 * Currently the same as using startReceiveMode(). 
 * Provided for compatibility.
 */
void lora_radio_receivemode(void);

/**
 * Return the RSSI value for the most recently received LoRa packet
 * as provided by RadioLib.
 * @return int16_t RSSI value
 */
int16_t getReceiveRSSI(void);
int8_t getReceiveSNR(void);

/**
 * Return the most recently received LoRa packet / RDCP message as Base64-encoded string. 
 * @return (char *) Base64-encoded LoRa packet payload, usually RDCP message
 */

char *get_current_rdcp_msg_base64(void);

/**
 * Set the most recently received LoRa packet as Base64-encoded string. 
 * Typically used in combination with SIMRX-like functions. 
 * @param m64 (char *) Base64-encoded LoRa packet payload
 * @return void 
 */
void set_current_rdcp_msg_base64(char *m64);

/**
 * Switch channel on T-Deck radio.
 * @param channel Channel to switch to, either CHANNEL868DA or CHANNEL868MG
 */
void radio_switch_channel(uint8_t channel);

#endif

/* EOF */
