#ifndef _DEVICE_PERSISTENCE
#define _DEVICE_PERSISTENCE

#include <Arduino.h>

/**
 * Initialize persistent storage on the T-Deck.
 * To be called once after power-on.
 */
void persistence_setup(void);

/**
 * Store a Serial command to be automatically executed again on next device power-on.
 * @param s String with a Serial command to automatically replay on next power-on.
 */
void persist_serial_command_for_replay(String s);

/**
 * Replay persisted Serial commands (= update configuration during device power-on).
 * Usually called once after power-on and persistence_setup().
 */
void persistence_replay_serial(void);

/**
 * Reset/delete the list of Serial commands replayed on device power-on.
 */
void persistence_reset_replay_serial(void);

/**
 * Reset the device base configuration. Device needs reprovisioning afterwards.
 */
void persistence_delete_basecfg(void);

/**
 * Serial print the device base configuration file.
 */
void persistence_dump_basecfg(void);

/**
 * Add a line of text to the device base configuration file.
 * Used internally.
 */
void persistence_add_basecfg(String s);

/**
 * Remove all persistently stored settings. Device needs to be restarted afterwards.
 */
void factory_reset();

/**
 * Check whether the device has been initialled provisioned by an RDCP Infrastructure Operator
 * @return true if basecfg is available, false if device is yet unprovisioned
 */
bool persistence_device_is_provisioned(void);

/**
 * Configure the device settings based on provisioning.
 * @return true if device was provisioned, false if this operation failed and the device needs to be provisioned first.
 */
bool persistence_configure_device_settings(void);

/**
 * Get the next RDCP Sequence Number to use for a specific RDCP address as Origin. 
 * @param origin RDCP Address of the device that needs the SequenceNumber as Origin 
 * @return uint16_t SequenceNumber as used in RDCP Header
 */
uint16_t get_next_rdcp_sequence_number(uint16_t origin);

/**
 * Set the next RDCP Sequence NUmber to use for a specific RDCP address. Used internally 
 * as well as for testing purposes. 
 * @param origin RDCP Address or Origin 
 * @param seq Next SequenceNumber to use for this Otigin 
 * @return the sequence number that has been set
 */
uint16_t set_next_rdcp_sequence_number(uint16_t origin, uint16_t seq);

/**
 * Get a fresh Nonce for a CIRE. 
 * @param origin RDCP Address of the device that wants to send a new CIRE 
 * @return uint16_t Nonce for this Origin that can be used as ReferenceNumber in a CIRE
 */
uint16_t get_next_cire_nonce(uint16_t origin);

/**
 * @return true if the device has FFat available, false otherwise
 */
bool  hasStorage(void);

/**
 * Check for nonce validity on management RDCP messages. 
 * @param name Name of the nonce type 
 * @param nonce Received nonce 
 * @return true if nonce is valid, false otherwise
 */
bool persistence_checkset_nonce(char *name, uint16_t nonce);

/**
 * Reformat the FFat file system, carrying over some configuration data. 
 */
void persistence_reformat_filesystem(void);

#endif

/* EOF */