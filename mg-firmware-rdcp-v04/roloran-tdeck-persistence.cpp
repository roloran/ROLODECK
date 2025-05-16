#include "roloran-tdeck-hal.h"
#include "settings-device.h"
#include "roloran-tdeck-serial.h"
#include "roloran-lora.h"
#include "roloran-tdeck-persistence.h"

#include <LittleFS.h>

#define FILENAME_SERIAL_REPLAY "/sercmds.txt"
#define FILENAME_DEVICE_SETTINGS "/devicesettings.txt"
#define FILENAME_PREFIX_SEQNR "/used_seqnr_"
#define FILENAME_PREFIX_CIRENONCE "/used_cirenonce_"

bool hasFFat = false;
bool device_is_provisioned = false;

bool  hasStorage(void)
{
  return hasFFat;
}

void persistence_setup(void)
{
  // FFat.format(); // unbrick device :-)
  if(LittleFS.begin(true, "/littlefs", 10U, "ffat")) // Format file system if necessary
  {
    hasFFat = true;
  }
  else
  {
    return;
  }
  return;
}

void persist_serial_command_for_replay(String s)
{
  if (hasFFat)
  {
    File f = LittleFS.open(FILENAME_SERIAL_REPLAY, FILE_APPEND);
    if (!f) return;
    char cline[256];
    s.toCharArray(cline, 256);
    f.printf("%s\n", cline);
    f.close();
  }
  return;
}

void persistence_add_basecfg(String s)
{
  if (hasFFat)
  {
    File f = LittleFS.open(FILENAME_DEVICE_SETTINGS, FILE_APPEND);
    if (!f) return;
    f.print(s + "\n");
    f.close();
  }
  return;
}

void persistence_replay_serial(void)
{
  if (hasFFat)
  {
    File f = LittleFS.open(FILENAME_SERIAL_REPLAY, FILE_READ);
    if (!f)
    {
      return;
    }
    while (f.available())
    {
      String line = f.readStringUntil('\n');
      serial_process_command(line, "REPLAY: ", false);
    }
    f.close();
  }
  return;
}

void persistence_dump_basecfg(void)
{
  if (hasFFat)
  {
    File f = LittleFS.open(FILENAME_DEVICE_SETTINGS, FILE_READ);
    serial_writeln("INFO: BASECFG Dump begin");
    if (!f)
    {
      serial_writeln("INFO: (not found)");
      return;
    }
    while (f.available())
    {
      String line = f.readStringUntil('\n');
      serial_writeln("INFO: BASECFG: " + line);
    }
    f.close();
    serial_writeln("INFO: BASECFG Dump end");
  }
  return;
}

void persistence_reset_replay_serial(void)
{
  if (hasFFat)
  {
    LittleFS.remove(FILENAME_SERIAL_REPLAY);
  }
  return;
}

void persistence_delete_basecfg(void)
{
  if (hasFFat)
  {
    LittleFS.remove(FILENAME_DEVICE_SETTINGS);
  }
  return;
}

bool persistence_device_is_provisioned(void)
{
  return device_is_provisioned;
}

bool persistence_configure_device_settings(void)
{
  if (hasFFat)
  {
    File f = LittleFS.open(FILENAME_DEVICE_SETTINGS, FILE_READ);
    if (!f) return false;
    int old_serial_mode = serial_get_serial_mode();
    serial_set_serial_mode(SERIAL_MODE_FULLACCESS);
    while (f.available())
    {
      String line = f.readStringUntil('\n');
      serial_process_command(line, "DEVICE SETTING: ", false);
    }
    f.close();
    serial_set_serial_mode(old_serial_mode);
    serial_writeln("Happily returning true");
    return true;
  }
  return false;
}

uint16_t get_next_rdcp_sequence_number(uint16_t origin)
{
  uint16_t seq = 1;
  if (hasFFat)
  {
    char fn[256];
    snprintf(fn, 256, "%s%04X", FILENAME_PREFIX_SEQNR, origin);
    File f = LittleFS.open(fn, FILE_READ);
    if (!f)
    {
      serial_writeln("WARNING: Missing sequence number file, starting with defaults");
      set_next_rdcp_sequence_number(origin, 2);
      return seq;
    }
    String line = "0";
    if (f.available())
    {
      line = f.readString();
    }
    seq = line.toInt();
    f.close();
    set_next_rdcp_sequence_number(origin, seq+1);
  }
  return seq;
}

uint16_t set_next_cire_nonce(uint16_t origin, uint16_t nonce)
{
  if (hasFFat)
  {
    char fn[256];
    snprintf(fn, 256, "INFO: Persisting next-up CIRE Nonce %u for %04X", nonce, origin);
    serial_writeln(fn);
    snprintf(fn, 256, "%s%04X", FILENAME_PREFIX_CIRENONCE, origin);
    File f = LittleFS.open(fn, FILE_WRITE);
    if (!f) return nonce;
    char content[256];
    snprintf(content, 256, "%" PRIu16 "\n", nonce);
    f.print(content);
    f.close();
  }
  return nonce;
}

uint16_t get_next_cire_nonce(uint16_t origin)
{
  uint16_t nonce = 1;
  if (hasFFat)
  {
    char fn[256];
    snprintf(fn, 256, "%s%04X", FILENAME_PREFIX_CIRENONCE, origin);
    File f = LittleFS.open(fn, FILE_READ);
    if (!f)
    {
      serial_writeln("WARNING: Missing CIRE Nonce file, starting with defaults");
      set_next_cire_nonce(origin, 2);
      return nonce;
    }
    String line = "0";
    if (f.available())
    {
      line = f.readString();
    }
    nonce = line.toInt();
    f.close();
    set_next_cire_nonce(origin, nonce+1);
  }
  return nonce;
}

uint16_t set_next_rdcp_sequence_number(uint16_t origin, uint16_t seq)
{
  if (hasFFat)
  {
    char fn[256];
    snprintf(fn, 256, "INFO: Persisting next-up seqnr %u for %04X", seq, origin);
    serial_writeln(fn);
    snprintf(fn, 256, "%s%04X", FILENAME_PREFIX_SEQNR, origin);
    File f = LittleFS.open(fn, FILE_WRITE);
    if (!f) return seq;
    char content[256];
    snprintf(content, 256, "%" PRIu16 "\n", seq); // persist previous serial number so we match it on next use
    f.print(content);
    f.close();
  }
  return seq;
}

bool persistence_checkset_nonce(char *name, uint16_t nonce)
{
  if (!hasFFat) return false;
  bool is_valid = false;

  char filename[64];
  snprintf(filename, 64, "%s.nce", name);

  File f = LittleFS.open(filename, FILE_READ);
  if (!f)
  {
    is_valid = true; // never seen a nonce for this type before
  }
  else
  {
    String line = "0";
    if (f.available())
    {
      line = f.readString();
    }
    uint16_t old_nonce = line.toInt();
    f.close();

    if (old_nonce < nonce)
    { 
      is_valid = true;
    }
    else 
    {
      char info[256];
      snprintf(info, 256, "WARNING: Old nonce == %" PRIu16 ", new nonce == %" PRIu16, old_nonce, nonce);
      serial_writeln(info);
    }
  }

  if (is_valid)
  {
    f = LittleFS.open(filename, FILE_WRITE);
    if (!f)
    {
      serial_writeln("ERROR: Cannot persist nonce");
      is_valid = false; // cannot persist nonce, don't trust it
    }

    char content[256];
    snprintf(content, 256, "%" PRIu16 "\n", nonce);
    f.print(content);
    f.close();
  }

  return is_valid;
}

uint8_t save_basecfg[4096];
uint8_t save_replay[4096];
uint8_t save_seqnr[256];
uint8_t save_nonce[256];

void persistence_reformat_filesystem(void)
{
  if (!hasFFat) return;
  char fn[256];
  File f;

  bool has_device_settings = true;
  bool has_serial_replay = true; 
  bool has_seqnr = true; 
  bool has_cirenonce = true;
  int size_basecfg = 0;
  int size_replay = 0;
  int size_seqnr = 0;
  int size_nonce = 0;

  snprintf(fn, 256, "%s", FILENAME_DEVICE_SETTINGS);
  f = LittleFS.open(fn, FILE_READ);
  if (!f)
  {
    has_device_settings = false;
  }
  else 
  {
    size_basecfg = f.read(save_basecfg, 4096);
  }
  f.close();

  snprintf(fn, 256, "%s", FILENAME_SERIAL_REPLAY);
  f = LittleFS.open(fn, FILE_READ);
  if (!f)
  {
    has_serial_replay = false;
  }
  else 
  {
    size_replay = f.read(save_replay, 4096);
  }
  f.close();

  snprintf(fn, 256, "%s%04X", FILENAME_PREFIX_SEQNR, getMyRDCPAddress());
  f = LittleFS.open(fn, FILE_READ);
  if (!f)
  {
    has_seqnr = false;
  }
  else 
  {
    size_seqnr = f.read(save_seqnr, 256);
  }
  f.close();

  snprintf(fn, 256, "%s%04X", FILENAME_PREFIX_CIRENONCE, getMyRDCPAddress());
  f = LittleFS.open(fn, FILE_READ);
  if (!f)
  {
    has_cirenonce = false;
  }
  else 
  {
    size_nonce = f.read(save_nonce, 256);
  }
  f.close();

  LittleFS.end();
  LittleFS.format();
  LittleFS.begin(true, "/littlefs", 10U, "ffat");
  LittleFS.format();

  if (has_device_settings)
  {
    snprintf(fn, 256, "%s", FILENAME_DEVICE_SETTINGS);
    f = LittleFS.open(fn, FILE_WRITE);
    if (f)
    {
      f.write(save_basecfg, size_basecfg);
      f.close();
      delay(500);
    }
  }

  if (has_serial_replay)
  {
    snprintf(fn, 256, "%s", FILENAME_SERIAL_REPLAY);
    f = LittleFS.open(fn, FILE_WRITE);
    if (f)
    {
      f.write(save_replay, size_replay);
      f.close();
      delay(500);
    }
  }

  if (has_seqnr)
  {
    snprintf(fn, 256, "%s%04X", FILENAME_PREFIX_SEQNR, getMyRDCPAddress());
    f = LittleFS.open(fn, FILE_WRITE);
    if (f)
    {
      f.write(save_seqnr, size_seqnr);
      f.close();
      delay(500);
    }
  }

  if (has_cirenonce)
  {
    snprintf(fn, 256, "%s%04X", FILENAME_PREFIX_CIRENONCE, getMyRDCPAddress());
    f = LittleFS.open(fn, FILE_WRITE);
    if (f)
    {
      f.write(save_nonce, size_nonce);
      f.close();
      delay(500);
    }
  }

  serial_writeln("INFO: Reformat completed. Prepare for reboot.");
  delay(2000);
  return;
}

/* EOF */
