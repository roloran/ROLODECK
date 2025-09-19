#include "settings-device.h"
#include "roloran-tdeck-serial.h"
#include "roloran-tdeck-serial.h"
#include "roloran-crypto.h"
#include "roloran-tdeck-hal.h"
#include <LittleFS.h> 
#include "settings-scenario.h"

/**
 * Settings that should be unique for each device
 */
uint16_t MY_RDCP_ADDRESS = 0xAEFF;   //< This device's RDCP address
String   DEVICE_OWNER = ""; //< Default display name for Citizen Reports

/**
 * Settings that should be the same for all devices in an area / group
 */
uint16_t MY_ENTRY_POINTS[] = {0,0,0,0,0};
uint16_t MY_MULTICAST_ADDRESSES[] = {0,0,0,0,0};

/**
 * Settings that should be the same for all devices in a scenario
 */
float    MY_LORA_FREQ = 868.2;         //< LoRa Frequency in MHz
float    MY_LORA_BW = 125.0;           //< LoRa Bandwidth
uint8_t  MY_LORA_SF = 12;              //< LoRa Spreading Factor
int      MY_LORA_CR = 5;               //< LoRa Coding Rate as per RadioLib
uint8_t  MY_LORA_SW = 0x12;            //< LoRa Short Sync Word
int      MY_LORA_POWER = 14;           //< LoRa Output Power in dBm, -17 to 22
uint16_t MY_LORA_PREAMBLE_LENGTH = 15; //< LoRa number of Preamble Symbols

uint16_t getMyRDCPAddress(void)                  { return MY_RDCP_ADDRESS; }
void     setMyRDCPAddress(uint16_t rdcp_address) { MY_RDCP_ADDRESS = rdcp_address; return; }

float    getMyLoRaFrequency(void)       { return MY_LORA_FREQ; }
float    getMyLoRaBandwidth(void)       { return MY_LORA_BW; }
uint8_t  getMyLoRaSpreadingFactor(void) { return MY_LORA_SF; }
int      getMyLoRaCodingRate(void)      { return MY_LORA_CR; }
uint8_t  getMyLoRaSyncWord(void)        { return MY_LORA_SW; }
int      getMyLoRaPower(void)           { return MY_LORA_POWER; }
uint16_t getMyLoRaPreambleLength(void)  { return MY_LORA_PREAMBLE_LENGTH; }

void     setMyLoRaFrequency(float freq_in_mhz)              { MY_LORA_FREQ = freq_in_mhz; return; }
void     setMyLoRaBandwidth(float bandwidth_in_khz)         { MY_LORA_BW = bandwidth_in_khz; return; }
void     setMyLoRaSpreadingFactor(uint8_t sf)               { MY_LORA_SF = sf; return; }
void     setMyLoRaCodingRate(int cr)                        { MY_LORA_CR = cr; return; }
void     setMyLoRaSyncWord(uint8_t syncword)                { MY_LORA_SW = syncword; return; }
void     setMyLoRaPower(int power)                          { MY_LORA_POWER = power; return; }
void     setMyLoRaPreambleLength(uint16_t preamble_length)  { MY_LORA_PREAMBLE_LENGTH = preamble_length; return; }

uint32_t MY_SCREENSAVER_DELAY = 30000; //< After how many milliseconds should the screensaver turn on?
uint32_t getScreensaverDelay(void)           { return MY_SCREENSAVER_DELAY; }
void     setScreensaverDelay(uint32_t delay) { MY_SCREENSAVER_DELAY = delay; return; }

void addMyEntryPoint(uint16_t ep)
{
  bool added = false;
  for (int i=0; i<5; i++)
  {
    if (MY_ENTRY_POINTS[i] == RDCP_NO_ADDRESS)
    {
      MY_ENTRY_POINTS[i] = ep;
      added = true;
      break;
    }
  }
  if (!added)
  {
    serial_writeln("WARNING: Maximum reached - cannot add more Entry Points");
  }
  return;
}

void addMyMulticastAddress(uint16_t ma)
{
  bool added = false;
  for (int i=0; i<5; i++)
  {
    if (MY_MULTICAST_ADDRESSES[i] == RDCP_NO_ADDRESS)
    {
      MY_MULTICAST_ADDRESSES[i] = ma;
      added = true;
      break;
    }
  }
  if (!added)
  {
    serial_writeln("WARNING: Maximum reached - cannot add more Group Memberships");
  }
  return;
}

bool matchesMyMulticastAddress(uint16_t ma)
{
  for (int i=0; i<5; i++)
  {
    if (MY_MULTICAST_ADDRESSES[i] == ma) return true;
  }
  return false;
}

uint16_t getEntryPoint(uint8_t num)
{
  return MY_ENTRY_POINTS[num];
}

uint16_t getSuggestedRelay(uint8_t retry=0)
{
  uint16_t roam = getRoamingRecommendation(MINUTES_TO_MILLISECONDS*15);
  if ((retry == 0) && (roam != RDCP_NO_ADDRESS)) return roam;
  uint8_t index = 0;
  uint16_t da = getEntryPoint(index);
  for (int i=0; i<retry; i++)
  {
    uint16_t new_da = getEntryPoint(index%5);
    if (new_da != RDCP_NO_ADDRESS) da = new_da;
  }
  return da;
}

uint16_t getMulticastAddress(uint8_t num)
{
  return MY_MULTICAST_ADDRESSES[num];
}
bool matchesMyDeviceAddress(uint16_t rdcpa)
{
  return rdcpa == MY_RDCP_ADDRESS;
}

bool matchesAnyOfMyAddresses(uint16_t rdcpa)
{
  return ((rdcpa == RDCP_ADDRESS_BROADCAST) || matchesMyDeviceAddress(rdcpa) || matchesMyMulticastAddress(rdcpa));
}

void clearMyMulticastAddresses(void)
{
  for (int i=0; i<5; i++) MY_MULTICAST_ADDRESSES[i] = 0;
  return;
}

void clearMyEntryPoints(void)
{
  for (int i=0; i<5; i++) MY_ENTRY_POINTS[i] = 0;
  return;
}

String OwnerDisplayName = "";

void setOwnerDisplayName(String s)
{
  OwnerDisplayName = s;
  return;
}

String getOwnerDisplayName(void)
{
  return OwnerDisplayName;
}

uint8_t HQsharedSecret[ASCIIKEYLEN];

void setHQsharedSecret(uint8_t *secret)
{
  for (int i=0; i<ASCIIKEYLEN; i++) HQsharedSecret[i] = secret[i];
  return;
}

uint8_t* getHQsharedSecret(void)
{
  return HQsharedSecret;
}

char HQschnorrPublicKey[DATABUFLEN];
char MYschnorrPublicKey[DATABUFLEN];
char MYschnorrPrivateKey[DATABUFLEN];

void  setHQpublicKey(char *pubkey)
{ 
  snprintf(HQschnorrPublicKey, DATABUFLEN, "%s\0", pubkey);
  return;
}

char* getHQpublicKey(void)
{
  return HQschnorrPublicKey;
}

void setMyPublicKey(char *pubkey)
{
  snprintf(MYschnorrPublicKey, DATABUFLEN, "%s\0", pubkey); 
  return;
}

char* getMyPublicKey(void)
{
  return MYschnorrPublicKey;
}

void setMyPrivateKey(char *privkey)
{
  snprintf(MYschnorrPrivateKey, DATABUFLEN, "%s\0", privkey); 
  return;
}

char* getMyPrivateKey(void)
{
  return MYschnorrPrivateKey;
}

uint8_t unlockHash1[SHABUFLEN];
uint8_t unlockHash2[SHABUFLEN];
bool unlockHash1set = false;
bool unlockHash2set = false;

void setUnlockHash1(uint8_t *s)
{
  unlockHash1set = true;
  for (int i=0; i<SHABUFLEN; i++) unlockHash1[i] = s[i];
  return;
}

void setUnlockHash2(uint8_t *s)
{
  unlockHash2set = true;
  for (int i=0; i<SHABUFLEN; i++) unlockHash2[i] = s[i];
  return;
}

uint8_t checkUnlockHash(String s)
{
  uint8_t result = SERIAL_MODE_SILENT;

  char input[INFOLEN];
  s.toCharArray(input, INFOLEN);

  SHA256 h = SHA256();
  h.reset();
  h.update(input, strlen(input));
  uint8_t sha[SHABUFLEN];
  h.finalize(sha, SHABUFLEN);

  if (unlockHash1set)
  {
    bool match = true;
    for (int i=0; i<SHABUFLEN; i++) if (sha[i] != unlockHash1[i]) match = false;
    if (match) result = SERIAL_MODE_READONLY;
  }

  if (unlockHash2set)
  {
    bool match = true;
    for (int i=0; i<SHABUFLEN; i++) if (sha[i] != unlockHash2[i]) match = false;
    if (match) result = SERIAL_MODE_FULLACCESS;
  }

  return result;
}

struct roamingTableEntry {
  uint16_t da = RDCP_NO_ADDRESS;
  int16_t rssi = -200;
  int64_t timestamp = NO_TIMESTAMP;
};

struct roamingTableEntry roamingTable[16];

void updateRoamingTable(uint16_t da, int16_t rssi)
{
  for (int i=0; i<16; i++)
  {
    if ((roamingTable[i].da == da) || (roamingTable[i].da == RDCP_NO_ADDRESS))
    {
      roamingTable[i].da = da;
      roamingTable[i].rssi = rssi;
      roamingTable[i].timestamp = my_millis();
      break;
    }
  }
  return;
}

uint16_t getRoamingRecommendation(uint32_t timeframe = MINUTES_TO_MILLISECONDS*15)
{
  int index = -1;
  int64_t now = my_millis();

  for (int i=0; i<16; i++)
  {
    if ((roamingTable[i].timestamp + timeframe > now) && (roamingTable[i].da != RDCP_NO_ADDRESS))
    {
      if (index == -1)
      {
        index = i;
      }
      else
      {
        if (roamingTable[i].rssi > roamingTable[index].rssi) index = i;
      }
    }
  }

  if (index > -1) return roamingTable[index].da;
  return 0;
}

bool do_show_eula = true;
bool checked_poweron_counter = false;

#define FILENAME_COUNT "/usagectr"
#define SKIP_EULA_AFTER 20

bool shall_show_eula(void)
{
  if (!checked_poweron_counter)
  {
    checked_poweron_counter = true;
    uint64_t count = 1;

    File f = LittleFS.open(FILENAME_COUNT, FILE_READ);
    if (f)
    {
      f.read((uint8_t*)&count, sizeof(uint64_t));
      f.close();
      count++;
    }
    
    f = LittleFS.open(FILENAME_COUNT, FILE_WRITE);
    f.write((uint8_t*)&count, sizeof(uint64_t));
    f.close();
    
    if (count > SKIP_EULA_AFTER) do_show_eula = false;
  }
  return do_show_eula;
}

void set_show_eula(bool newvalue)
{
  do_show_eula = newvalue;
  return;
}

/* EOF */