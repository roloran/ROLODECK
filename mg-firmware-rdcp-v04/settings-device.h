#ifndef _SETTINGS_DEVICE
#define _SETTINGS_DEVICE

#include <Arduino.h>

/*
 * Firmware version information and other constant data
 */

#define FW_SCENARIO   "Neuhaus v2.0"
#define FW_RDCP       "v0.4"
#define FW_VERSION    "001"

#define SERIAL_PREFIX "T-DECK: "

/**
 * @return This device's primary RDCP Address
 */
uint16_t getMyRDCPAddress(void);

/**
 * @param rdcp_address Primary RDCP Address to set for this device
 */
void     setMyRDCPAddress(uint16_t rdcp_address);

/**
 * @return This device's LoRa channel frequency in MHz
 */
float    getMyLoRaFrequency(void);

/**
 * @return This device's LoRa channel bandwidth in KHz 
 */
float    getMyLoRaBandwidth(void);

/**
 * @return This device's currently used LoRa spreading factor 
 */
uint8_t  getMyLoRaSpreadingFactor(void);

/**
 * @return This device's currently used LoRa coding rate (5--8 as in RadioLib)
 */
int      getMyLoRaCodingRate(void);

/**
 * @return This device's currently used LoRa SyncWord
 */
uint8_t  getMyLoRaSyncWord(void);

/**
 * @return This device's currently used TX power in dBm
 */
int      getMyLoRaPower(void);

/**
 * @return This device's currently used LoRa preamble length
 */
uint16_t getMyLoRaPreambleLength(void);

/**
 * @param freq_in_mhz LoRa channel frequency to use in MHz 
 */
void     setMyLoRaFrequency(float freq_in_mhz);

/**
 * @param bandwidth_in_khz LoRa channel bandwidth to use in KHz 
 */
void     setMyLoRaBandwidth(float bandwidth_in_khz);

/**
 * @param sf LoRa spreading factor to use (7--12)
 */
void     setMyLoRaSpreadingFactor(uint8_t sf);

/**
 * @param cr LoRa coding rate to use (5--8, as in RadioLib)
 */
void     setMyLoRaCodingRate(int cr);

/**
 * @param syncword LoRa syncword to use (0x12 or 0x34)
 */
void     setMyLoRaSyncWord(uint8_t syncword);

/**
 * @param power LoRa TX power to use in dBm (usually 0--22)
 */
void     setMyLoRaPower(int power);

/**
 * @param preamble_length LoRa preamble length to use (usually 15 in our scenarios)
 */
void     setMyLoRaPreambleLength(uint16_t preamble_length);

/**
 * @return Number of milliseconds until the screensaver should activate when idle 
 */
uint32_t getScreensaverDelay(void);

/**
 * @param delay Number of milliseconds until the screensaver should activate when idle 
 */
void     setScreensaverDelay(uint32_t delay);

/**
 * @param ep RDCP Address of an EntryPoint DA to use. Up to 5 may be set. 
 */
void addMyEntryPoint(uint16_t ep);

/**
 * @param ma Set an RDCP Multicast Address for this device. up to 5 may be set. 
 */
void addMyMulticastAddress(uint16_t ma);

/**
 * @param ma An RDCP multicast address 
 * @return true if the given multicast address matches one of this device's 
 */
bool matchesMyMulticastAddress(uint16_t ma);

/**
 * @param num Index of entry point to get 
 * @return RDCP Address of stored Entry Point identified by index 
 */
uint16_t getEntryPoint(uint8_t num);

/**
 * @param num Index of Multicast address to get 
 * @return RDCP Multicast address of this device, identified by index 
 */
uint16_t getMulticastAddress(uint8_t num);

/**
 * @param rdcpa An RDCP address 
 * @return true if the given RDCP address matches this device's primary RDCP address 
 */
bool matchesMyDeviceAddress(uint16_t rdcpa);

/**
 * @param rdcpa An RDCP address 
 * @return true if the given RDCP address matches any of this device's (including broadcast and multicast)
 */
bool matchesAnyOfMyAddresses(uint16_t rdcpa);

/**
 * Reset currently set multicast addresses 
 */
void clearMyMulticastAddresses(void);

/**
 * Reset currently set entry points
 */
void clearMyEntryPoints(void);

/**
 * @param s Display name of device owner to set (default in CIRE's "who reports?")
 */
void     setOwnerDisplayName(String s);

/**
 * @return String with the name of the device owner
 */
String   getOwnerDisplayName(void);

/**
 * @param secret AES-256 key to be used between this device and HQ 
 */
void     setHQsharedSecret(uint8_t *secret);

/**
 * @return AES-256 key to be used between this device and HQ 
 */
uint8_t* getHQsharedSecret(void);

/**
 * @param pubkey Schnorr public key of the HQ 
 */
void     setHQpublicKey(char *pubkey);

/**
 * @return Schnorr public key of the HQ 
 */
char*    getHQpublicKey(void);

/**
 * @param pubkey Schnorr public key to use when signing messages on this device 
 */
void     setMyPublicKey(char *pubkey);

/**
 * @return Schnorr public key to use when signing messages on this device 
 */
char*    getMyPublicKey(void);

/**
 * @param privkey Schnorr private key to use when signing messages on this device 
 */
void     setMyPrivateKey(char *privkey);

/**
 * @return Schnorr private key to use when signing messages on this device 
 */
char*    getMyPrivateKey(void);

/**
 * @param s Password hash for unlocking read-only Serial mode 
 */
void     setUnlockHash1(uint8_t *s);

/**
 * @param s Password hash for unlocking full access to Serial
 */
void     setUnlockHash2(uint8_t *s);

/**
 * @param s Check a password against the stored password hashes 
 * @return Resulting Serial/UART access mode 
 */
uint8_t  checkUnlockHash(String s);

/**
 * Update the roaming table with RSSI value after receiving from a DA 
 * @param da RDCP address of the DA the valid RDCP message was received from 
 * @param rssi RRSI value for the received RDCP message as provided by radio libary 
 */
void updateRoamingTable(uint16_t da, int16_t rssi);

/**
 * @param timeframe Number of milliseconds of the oldest relevant roaming table entry 
 * @return RDCP Address of the recommended RDCP Entry Point when roaming 
 */
uint16_t getRoamingRecommendation(uint16_t timeframe);

/**
 * Get a recommendation for an entry point to use.
 * The recommendation is based on current roaming settings as well as persisted Entry Points.
 * @param retry Number of retries, i.e., how often this function has already been called without getting a working Entry Point 
 * @return RDCP Address of the recommended RDCP Entry Point 
 */
uint16_t getSuggestedRelay(uint8_t retry);

/**
 * @return true if the device should show the EULA screen after power-on 
 */
bool shall_show_eula(void);

/**
 * @param newvalue true if the EULA screen should be shown, false otherwise 
 */
void set_show_eula(bool newvalue);

#endif

/* EOF */