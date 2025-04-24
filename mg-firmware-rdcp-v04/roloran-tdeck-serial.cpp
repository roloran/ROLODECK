/*
 * roloran-tdeck-serial.c
 */

#include "roloran-tdeck-hal.h"
#include "settings-device.h"
#include "roloran-tdeck-serial.h"
#include "roloran-lora.h"
#include "roloran-tdeck-persistence.h"
#include "roloran-rdcp.h"
#include "tdeck_gui.h"
#include "roloran-board.h"
#include "roloran-audio.h"

uint16_t LEN = 512;
uint8_t  MY_SERIAL_MODE = SERIAL_MODE_SILENT;
bool     new_simrx_available = false;
#define  BUFLEN 512
uint8_t  simrx_buffer[BUFLEN];
uint16_t simrx_buffer_length = 0;
int64_t  simrx_timestamp = 0;

bool pre_banner_mode = true;
String SERIAL_NOTE = "";

void set_serial_note(String s)
{
  SERIAL_NOTE = "{" + s + "} ";
  if (s.startsWith("none")) SERIAL_NOTE = "";
  return;
}

int64_t serial_get_simrx_timestamp(void)
{
  return simrx_timestamp;
}

uint8_t serial_get_serial_mode(void)
{
  return MY_SERIAL_MODE;
}

void serial_set_serial_mode(uint8_t newmode)
{
  MY_SERIAL_MODE = newmode;
  return;
}

void serial_begin(void)
{
  Serial.begin(TDECK_SERIAL_SPEED);
  Serial.setTimeout(10); // don't wait too long when reading from Serial
  return;
}

void set_pre_banner_mode(bool v)
{
  pre_banner_mode = v;
  return;
}

void serial_banner(void)
{
  pre_banner_mode = false;
  Serial.println(SERIAL_PREFIX "INFO: Firmware for scenario " FW_SCENARIO ", RDCP " FW_RDCP ", build " FW_VERSION ", " __DATE__ " " __TIME__);
  char buf[LEN];
  snprintf(buf, LEN, "%sINFO: Device RDCP AD: %04X\0",        SERIAL_PREFIX, getMyRDCPAddress()); Serial.println(buf);
  snprintf(buf, LEN, "%sINFO: Device RDCP EP: %04X %04X %04X %04X %04X\0",         SERIAL_PREFIX, getEntryPoint(0), getEntryPoint(1), getEntryPoint(2), getEntryPoint(3), getEntryPoint(4)); Serial.println(buf);
  snprintf(buf, LEN, "%sINFO: Device RDCP MC: %04X %04X %04X %04X %04X\0",         SERIAL_PREFIX, getMulticastAddress(0), getMulticastAddress(1), getMulticastAddress(2), getMulticastAddress(3), getMulticastAddress(4)); Serial.println(buf);
  snprintf(buf, LEN, "%sINFO: Device LoRa FQ: %.3f MHz\0",    SERIAL_PREFIX, getMyLoRaFrequency()); Serial.println(buf);
  snprintf(buf, LEN, "%sINFO: Device LoRa BW: %3.0f kHz\0",   SERIAL_PREFIX, getMyLoRaBandwidth()); Serial.println(buf);
  snprintf(buf, LEN, "%sINFO: Device LoRa SF: %2d\0",         SERIAL_PREFIX, getMyLoRaSpreadingFactor()); Serial.println(buf);
  snprintf(buf, LEN, "%sINFO: Device LoRa CR: 4/%d\0",        SERIAL_PREFIX, getMyLoRaCodingRate()); Serial.println(buf);
  snprintf(buf, LEN, "%sINFO: Device LoRa SW: 0x%02X\0",      SERIAL_PREFIX, getMyLoRaSyncWord()); Serial.println(buf);
  snprintf(buf, LEN, "%sINFO: Device LoRa PW: %2d dBm\0",     SERIAL_PREFIX, getMyLoRaPower()); Serial.println(buf);
  snprintf(buf, LEN, "%sINFO: Device LoRa PL: %2d symbols\0", SERIAL_PREFIX, getMyLoRaPreambleLength()); Serial.println(buf);
  snprintf(buf, LEN, "%sINFO: Device SerMode: %d\0",          SERIAL_PREFIX, MY_SERIAL_MODE); Serial.println(buf);
  return;
}

extern SemaphoreHandle_t highlander;

bool serial_write(String s, bool use_prefix)
{
  if (pre_banner_mode) return false;
  if (MY_SERIAL_MODE != SERIAL_MODE_SILENT)
  {
    if (highlander) xSemaphoreTake(highlander, portMAX_DELAY); // ! may be nested call !
    digitalWrite(TDECK_TFT_CS, HIGH);
    delay(1);
    if (use_prefix == true)
    {
      Serial.print(SERIAL_NOTE + SERIAL_PREFIX + s);
    }
    else
    {
      Serial.print(s);
    }
    Serial.flush();
    xSemaphoreGive(highlander);
    return true;
  }
  return false;
}

void serial_writeln(String s, bool use_prefix)
{
  serial_write(s + "\n", use_prefix);
  return;
}

void serial_write_base64(char *data, uint8_t len, bool add_newline)
{
  int encodedLength = Base64ren.encodedLength(len);
  char encodedString[encodedLength + 1];
  Base64ren.encode(encodedString, (char *) data, len);
  if (add_newline == true)
  {
    serial_writeln(encodedString, false);
  }
  else
  {
    serial_write(encodedString, false);
  }
  return;
}

void serial_string_to_unishox(String s, bool add_newline)
{
  char buf[LEN];
  const char *sc = s.c_str();
  unsigned int len = strlen(sc);
  memset(buf, 0, sizeof(buf));
  int c_total = unishox2_compress_simple(sc, len, buf);
  serial_write_base64(buf, c_total, add_newline);
  return;
}

String serial_readln(void)
{
  return Serial.readString();
}

bool serial_has_new_message(void)
{
  return new_simrx_available;
}

uint16_t serial_copy_received_message(uint8_t *destination)
{
  new_simrx_available = false;
  memcpy(destination, simrx_buffer, simrx_buffer_length);
  return simrx_buffer_length;
}

#define MODE_REGULAR 0
#define MODE_BASECFG 1
int serial_processing_mode = MODE_REGULAR;

void serial_process_command(String s, String processing_mode, bool persist_selected_commands)
{
  s.trim();
  String s_uppercase = String(s);
  s_uppercase.toUpperCase();

  /* Handle BASECFG mode, end it with a single '.' on a new line. */
  if (serial_processing_mode == MODE_BASECFG)
  {
    if (s_uppercase.startsWith("."))
    {
      serial_processing_mode = MODE_REGULAR;
      persistence_dump_basecfg();
      serial_writeln("INFO: END OF BASECFG MODE");
    }
    else
    {
      persistence_add_basecfg(s);
    }
    return;
  }

  /* Don't echo the Serial command if it starts with '!' */
  if (s.startsWith("!"))
  {
    s = s.substring(1);
    s_uppercase = s_uppercase.substring(1);
  }
  else
  {
    if (s.length() > 1)
    {
      serial_writeln(processing_mode + s);
    }
    else
    {
      char status[256];
      int64_t now = my_millis();
      int64_t cfdelta = rdcp_get_channel_free_estimation() - now;
      int txqe = get_num_txq_entries();
      int txaqe = get_num_txaq_entries();
      uint16_t roam = getRoamingRecommendation(1000*60*15);
      time_t wallclock = tdeck_get_time();
      char timestamp[32];
      struct tm ti;
      snprintf(timestamp, 32, "n/a");
      if (wallclock)
      {
        if (localtime_r(&wallclock, &ti)) 
        {
          snprintf(timestamp, 256, "%02d.%02d.%04d %02d:%02d", ti.tm_mday, ti.tm_mon, ti.tm_year + 1900, ti.tm_hour, ti.tm_min);
        }
      }
      int cirestate = get_cire_state();

      snprintf(status, 256, "STATUS: T %" PRId64 " ms, rCF %" PRId64 " ms, Q %d/%d, R %04X, CPU %d MHz, clk %s, CIREstate %d", now, cfdelta, txqe, txaqe, roam, get_cpufreq(), timestamp, cirestate);
      serial_writeln(status);
    }
  }

  /* The following Serial commands are only available in 'full access' mode. */
  if (MY_SERIAL_MODE == SERIAL_MODE_FULLACCESS)
  {
    if (s_uppercase.startsWith("RESET "))
    {
      String p1= s_uppercase.substring(6);
      if (p1.equals(String("CONFIG")))
      {
        serial_writeln("INFO: Removing persisted Serial commands for replay");
        persistence_reset_replay_serial();
      }
      else if (p1.equals(String("RADIO")))
      {
        serial_writeln("INFO: Re-initalizing the radio with current configuration");
        radio_apply_new_configuration(); // Re-initalize the radio
      }
      else if (p1.equals(String("BASECFG")))
      {
        serial_writeln("INFO: Wiping device");
        persistence_reset_replay_serial();
        persistence_delete_basecfg();
      }
      else if (p1.equals(String("ENTRYPOINTS")) || p1.equals(String("RELAYS")))
      {
        serial_writeln("INFO: Clearing Entry Point Settings");
        clearMyEntryPoints();
      }
      else if (p1.equals(String("MULTICAST")))
      {
        serial_writeln("INFO: Clearing Multicast Group Settings");
        clearMyMulticastAddresses();
      }
      else if (p1.equals(String("MESSAGES")))
      {
        mb_clear_history();
      }
      else if (p1.equals(String("FILESYSTEM")))
      {
        serial_writeln("INFO: Reformatting file system and rebooting");
        persistence_reformat_filesystem();
        ESP.restart();
      }
      // Further RESET commands can be added here
    } // ^ RESET
    else if (s_uppercase.startsWith("SHOW "))
    {
      String p1= s_uppercase.substring(5);
      if (p1.equals(String("CONFIG")))
      {
        serial_banner();
      }
      else if (p1.equals(String("MESSAGES")))
      {
        mb_serial_show_messages();
      }
      else if (p1.equals(String("QUEUES")))
      {
        rdcp_dump_queues();
      }
      // Further SHOW commands can be added here
    } // ^ SHOW
    else if (s_uppercase.startsWith("LORAFREQ "))
    {
      String p1 = s.substring(9);
      float new_freq = p1.toFloat();
      setMyLoRaFrequency(new_freq);
      radio_apply_new_configuration();
      serial_writeln("INFO: Changed this device's LoRa frequency to " + String(getMyLoRaFrequency()) + " MHz");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("LORABW "))
    {
      String p1 = s.substring(7);
      float new_bw = p1.toFloat();
      setMyLoRaBandwidth(new_bw);
  	  radio_apply_new_configuration();
      serial_writeln("INFO: Changed this device's LoRa bandwidth setting to " + String(getMyLoRaBandwidth()) + " kHz");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("LORASF "))
    {
      String p1 = s.substring(7);
      int new_sf = p1.toInt();
      setMyLoRaSpreadingFactor(new_sf);
      radio_apply_new_configuration();
	    serial_writeln("INFO: Changed this device's LoRa spreading factor to " + String(getMyLoRaSpreadingFactor()));
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("LORACR "))
    {
      String p1 = s.substring(7);
      int new_cr = p1.toInt();
      setMyLoRaCodingRate(new_cr);
  	  radio_apply_new_configuration();
	    serial_writeln("INFO: Changed this device's LoRa coding rate to 4/" + String(getMyLoRaCodingRate()));
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("LORASW "))
    {
      char buffer[32];
      String p1 = s.substring(7);
      p1.toCharArray(buffer, 32);
      uint8_t target_sws = strtol(buffer, NULL, 16);
      if ((target_sws != 0x12) && (target_sws != 0x34))
      {
        serial_writeln("ERROR: This device's firmware currently only supports 0x12 or 0x34 as LoRa syncword.");
        return;
      }
      setMyLoRaSyncWord(target_sws);
  	  radio_apply_new_configuration();
      snprintf(buffer, 31, "%x\0", getMyLoRaSyncWord());
	    serial_writeln("INFO: Changed this device's LoRa syncword to 0x" + String(buffer));
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("LORAPW "))
    {
      String p1 = s.substring(7);
      int new_power = p1.toInt();
      setMyLoRaPower(new_power);
  	  radio_apply_new_configuration();
	    serial_writeln("INFO: Changed this device's LoRa output power to " + String(getMyLoRaPower()) + " dBm");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("LORAPL "))
    {
      String p1 = s.substring(7);
      int new_pl = p1.toInt();
      setMyLoRaPreambleLength(new_pl);
  	  radio_apply_new_configuration();
	    serial_writeln("INFO: Changed this device's LoRa preamble length to " + String(getMyLoRaPreambleLength()));
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("TX "))
    {
      String p1 = s.substring(3);
      rdcp_tx_interface(p1, TX_WHEN_CF, CHANNEL868);
    }
    else if (s_uppercase.startsWith("TXNOW "))
    {
      String p1 = s.substring(6);
      rdcp_tx_interface(p1, TX_IMMEDIATELY, CHANNEL868);
    }
    else if (s_uppercase.startsWith("TXSCHED "))
    {
      String both = s.substring(8);
      String p1 = both.substring(0, both.indexOf(' '));
      String p2 = both.substring(both.indexOf(' '));
      p1.trim();
      p2.trim();

      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint64_t delay = strtol(buffer, NULL, 10);

      rdcp_tx_interface(p2, delay, CHANNEL868);
    }
    else if (s_uppercase.startsWith("BASECFG"))
    {
      serial_writeln("INFO: BEGIN OF BASECFG MODE. You brick it, you fix it. ;-)");
      serial_processing_mode = MODE_BASECFG;
    }
    else if (s_uppercase.startsWith("RESTART") || s_uppercase.startsWith("REBOOT"))
    {
      serial_writeln("INFO: Restarting device");
      ESP.restart();
    }
    else if (s_uppercase.startsWith("RDCPIDUP"))
    {
      bool new_state = rdcp_toggle_ignore_duplicates();
      if (new_state == true)
      {
        serial_writeln("INFO: RDCP duplicate message filter deactivated");
      }
      else
      {
        serial_writeln("INFO: RDCP duplicate message filter activated");
      }
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("RDCPADDR "))
    {
      String p1 = s.substring(9);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t new_rdcp_address = strtol(buffer, NULL, 16);
      setMyRDCPAddress(new_rdcp_address);
      serial_writeln("INFO: Changed this device's RDCP address to " + p1);
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("ENTRYPOINT "))
    {
      String p1 = s.substring(11);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t new_entrypoint_address = strtol(buffer, NULL, 16);
      addMyEntryPoint(new_entrypoint_address);
      serial_writeln("INFO: Added Entry Point " + p1);
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("MULTICAST "))
    {
      String p1 = s.substring(10);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t new_multicast_address = strtol(buffer, NULL, 16);
      addMyMulticastAddress(new_multicast_address);
      serial_writeln("INFO: Added Multicast Address " + p1);
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("SCREENSAVER "))
    {
      String p1 = s.substring(12);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t new_screensaver_delay = strtol(buffer, NULL, 10);
      setScreensaverDelay(new_screensaver_delay);
      serial_writeln("INFO: Changed screensaver delay to " + p1 + " ms");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("HEARTBEAT "))
    {
      String p1 = s.substring(10);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t new_heartbeat_interval = strtol(buffer, NULL, 10);
      set_heartbeat_interval((int64_t) new_heartbeat_interval * 1000 * 60);
      serial_writeln("INFO: Changed heartbeat interval to " + p1 + " minutes");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("GRACETIME "))
    {
      String p1 = s.substring(10);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t new_grace_period = strtol(buffer, NULL, 10);
      set_grace_period((int64_t) new_grace_period * 1000);
      serial_writeln("INFO: Changed grace period to " + p1 + " seconds");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("CIRETIMEDA "))
    {
      String p1 = s.substring(11);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t new_ciretimeda = strtol(buffer, NULL, 10);
      set_ciretime_da(new_ciretimeda);
      serial_writeln("INFO: Changed CIRE ACK timeout for Entry Point to " + p1 + " seconds");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("CIRETIMEHQ "))
    {
      String p1 = s.substring(11);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t new_ciretimehq = strtol(buffer, NULL, 10);
      set_ciretime_hq(new_ciretimehq);
      serial_writeln("INFO: Changed CIRE ACK timeout for HQ to " + p1 + " seconds");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("UISCREEN "))
    {
      String p1 = s.substring(9);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t new_screen = strtol(buffer, NULL, 10);
      gui_transition_to_screen(new_screen);
    }
    else if (s_uppercase.startsWith("EULA "))
    {
      String p1 = s.substring(5);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t eula = strtol(buffer, NULL, 10);
      if (eula == 0)
      {
        serial_writeln("INFO: Not showing EULA on device power-on");
        set_show_eula(false);
      }
      else
      {
        serial_writeln("INFO: Showing EULA on device power-on");
        set_show_eula(true);
      }

      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("SIMRX "))
    {
      String p1 = s.substring(6);

      char buffer[BUFLEN];
      p1.toCharArray(buffer, BUFLEN);
      int b64msg_len = strlen(buffer);
      int decoded_length = Base64ren.decodedLength(buffer, b64msg_len);
      char decoded_string[decoded_length + 1];
      Base64ren.decode(decoded_string, buffer, b64msg_len);

      if (decoded_length > 0)
      {
        serial_writeln("INFO: LoRa Radio received packet.");
        simrx_buffer_length = decoded_length;
        for (int i=0; i != decoded_length; i++) simrx_buffer[i] = decoded_string[i];

        char serialtext[255];
        simrx_timestamp = my_millis();
        sprintf(serialtext, "RXMETA %d %d %d %.3f\0", simrx_buffer_length, 0, 100, getMyLoRaFrequency());
        serial_writeln(serialtext);
        sprintf(serialtext, "RX %s\0", buffer);
        serial_writeln(serialtext);
        new_simrx_available = true;
        set_current_rdcp_msg_base64(buffer);
      }
      else
      {
        serial_writeln("INFO: LoRa Radio received empty packet. RSSI: 0 dBm, SNR: 0 dB");
      }
    }
    else if (s_uppercase.startsWith("RDCPPING "))
    {
      String p1 = s.substring(9);
      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t target = strtol(buffer, NULL, 16);
      rdcp_mg_send_echo_request(target);
    }
    else if (s_uppercase.startsWith("RDCPTEST "))
    {
      String p1 = s.substring(9, 13);
      String p2 = s.substring(14);

      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t target = strtol(buffer, NULL, 16);
      rdcp_mg_send_test_message(target, p2);
    }
    else if (s_uppercase.startsWith("RDCPCIRE "))
    {
      String p1 = s.substring(9, 11);  // Subtype
      String p2 = s.substring(12, 16); // Reference number
      String p3 = s.substring(17);     // Text

      char b1[32], b2[32], b3[256];
      p1.toCharArray(b1, 32);
      p2.toCharArray(b2, 32);
      p3.toCharArray(b3, 256);

      uint8_t subtype = (uint8_t) strtol(b1, NULL, 16);
      uint16_t refnum = (uint16_t) strtol(b2, NULL, 16);

      char b[256];
      snprintf(b, 256, "INFO: Preparing to send CITIZEN REPORT (subtype %d, refnr %d)", subtype, refnum);
      serial_writeln(b);
      rdcp_send_cire(subtype, refnum, b3);
    }
    else if (s_uppercase.startsWith("OWNER "))
    {
      String p1 = s.substring(6);
      setOwnerDisplayName(p1);
      char oname[128];
      p1.toCharArray(oname, 128);
      char buffer[256];
      snprintf(buffer, 256, "INFO: Device owner displayname set to %s", oname);
      serial_writeln(buffer);
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("HQPUBKEY "))
    {
      String p1 = s.substring(9);
      char buffer[256];
      p1.toCharArray(buffer, 256);
      setHQpublicKey(buffer);
      snprintf(buffer, 256, "INFO: HQ Public Key set");
      serial_writeln(buffer);
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("MYPUBKEY "))
    {
      String p1 = s.substring(9);
      char buffer[256];
      p1.toCharArray(buffer, 256);
      setMyPublicKey(buffer);
      snprintf(buffer, 256, "INFO: Device Public Key set");
      serial_writeln(buffer);
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("MYPRIVKEY "))
    {
      String p1 = s.substring(10);
      char buffer[256];
      p1.toCharArray(buffer, 256);
      setMyPrivateKey(buffer);
      snprintf(buffer, 256, "INFO: Device Private Key set");
      serial_writeln(buffer);
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("NOTE "))
    {
      String p1 = s.substring(5);
      set_serial_note(p1);
      serial_writeln("INFO: New note set");
    }
    else if (s_uppercase.startsWith("SHAREDSECRET "))
    {
      String p1 = s.substring(13);
      uint8_t secret[32];
      for (int i=0; i<32; i++) secret[i] = 0;

      char input[128];
      p1.toCharArray(input, 128);

      char hexbyte[3];
      hexbyte[2] = 0;
      for (int i=0; i<32; i++)
      {
        hexbyte[0] = toupper(input[2*i]);
        hexbyte[1] = toupper(input[2*i+1]);
        secret[i] = (uint8_t) strtol(hexbyte, NULL, 16);
      }
      setHQsharedSecret(secret);
      serial_writeln("INFO: Shared secret with HQ established");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("UNLOCKHASH1 "))
    {
      String p1 = s.substring(12);
      uint8_t secret[32];
      for (int i=0; i<32; i++) secret[i] = 0;

      char input[128];
      p1.toCharArray(input, 128);

      char hexbyte[3];
      hexbyte[2] = 0;
      for (int i=0; i<32; i++)
      {
        hexbyte[0] = toupper(input[2*i]);
        hexbyte[1] = toupper(input[2*i+1]);
        secret[i] = (uint8_t) strtol(hexbyte, NULL, 16);
      }

      setUnlockHash1(secret);

      serial_writeln("INFO: Serial mode 1 configured");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("UNLOCKHASH2 "))
    {
      String p1 = s.substring(12);
      uint8_t secret[32];
      for (int i=0; i<32; i++) secret[i] = 0;

      char input[128];
      p1.toCharArray(input, 128);

      char hexbyte[3];
      hexbyte[2] = 0;
      for (int i=0; i<32; i++)
      {
        hexbyte[0] = toupper(input[2*i]);
        hexbyte[1] = toupper(input[2*i+1]);
        secret[i] = (uint8_t) strtol(hexbyte, NULL, 16);
      }

      setUnlockHash2(secret);

      serial_writeln("INFO: Serial mode 0 configured");
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else if (s_uppercase.startsWith("RDCPACKS "))
    {
      String p1 = s.substring(9, 13);  // origin
      String p2 = s.substring(14, 18); // destination
      String p3 = s.substring(19, 23); // seqnr
      String p4 = s.substring(24, 25); // acktype

      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t origin = strtol(buffer, NULL, 16);
      p2.toCharArray(buffer, 32);
      uint16_t destination = strtol(buffer, NULL, 16);
      p3.toCharArray(buffer, 32);
      uint16_t seqnr = strtol(buffer, NULL, 16);
      p4.toCharArray(buffer, 32);
      uint8_t acktype = strtol(buffer, NULL, 16);

      char b[256];
      snprintf(b, 256, "INFO: Preparing to send a signed ACKNOWLEDGMENT from %s to %s for seqnr %s (type %s)", p1, p2, p3, p4);
      serial_writeln(b);
      rdcp_send_ack_signed(origin, destination, seqnr, acktype);
    }
    else if (s_uppercase.startsWith("RDCPACK "))
    {
      String p1 = s.substring(8, 12);  // origin
      String p2 = s.substring(13, 17); // destination
      String p3 = s.substring(18, 22); // seqnr
      String p4 = s.substring(23, 24); // acktype

      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint16_t origin = strtol(buffer, NULL, 16);
      p2.toCharArray(buffer, 32);
      uint16_t destination = strtol(buffer, NULL, 16);
      p3.toCharArray(buffer, 32);
      uint16_t seqnr = strtol(buffer, NULL, 16);
      p4.toCharArray(buffer, 32);
      uint8_t acktype = strtol(buffer, NULL, 16);

      char b[256];
      snprintf(b, 256, "INFO: Preparing to send an unsigned ACKNOWLEDGMENT from %s to %s for seqnr %s (type %s)", p1, p2, p3, p4);
      serial_writeln(b);
      rdcp_send_ack_unsigned(origin, destination, seqnr, acktype);
    }
    else if (s_uppercase.startsWith("RDCPTIME "))
    {
      String p1 = s.substring(9, 11);  // year since 2025
      String p2 = s.substring(12, 14); // month
      String p3 = s.substring(15, 17); // day in month
      String p4 = s.substring(18, 20); // hour
      String p5 = s.substring(21, 23); // minute
      String p6 = s.substring(24); // RDCP infrastructure status

      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint8_t year = strtol(buffer, NULL, 10);
      p2.toCharArray(buffer, 32);
      uint8_t month = strtol(buffer, NULL, 10);
      p3.toCharArray(buffer, 32);
      uint8_t day = strtol(buffer, NULL, 10);
      p4.toCharArray(buffer, 32);
      uint8_t hour = strtol(buffer, NULL, 10);
      p5.toCharArray(buffer, 32);
      uint8_t minute = strtol(buffer, NULL, 10);
      p6.toCharArray(buffer, 32);
      uint8_t status = strtol(buffer, NULL, 10);

      char b[256];
      snprintf(b, 256, "INFO: Preparing to send a signed TIMESTAMP from HQ to Broadcast");
      serial_writeln(b);
      rdcp_send_timestamp(year, month, day, hour, minute, status);
    }
    else if (s_uppercase.startsWith("SETTIME "))
    {
      String p1 = s.substring(8, 10);  // year since 2025
      String p2 = s.substring(11, 13); // month
      String p3 = s.substring(14, 16); // day in month
      String p4 = s.substring(17, 19); // hour
      String p5 = s.substring(20, 22); // minute
      String p6 = s.substring(23); // RDCP infrastructure status

      char buffer[32];
      p1.toCharArray(buffer, 32);
      uint8_t year = strtol(buffer, NULL, 10);
      p2.toCharArray(buffer, 32);
      uint8_t month = strtol(buffer, NULL, 10);
      p3.toCharArray(buffer, 32);
      uint8_t day = strtol(buffer, NULL, 10);
      p4.toCharArray(buffer, 32);
      uint8_t hour = strtol(buffer, NULL, 10);
      p5.toCharArray(buffer, 32);
      uint8_t minute = strtol(buffer, NULL, 10);
      p6.toCharArray(buffer, 32);
      uint8_t status = strtol(buffer, NULL, 10);

      char b[256];
      snprintf(b, 256, "INFO: Setting this device's wallclock time");
      serial_writeln(b);
      tdeck_set_time(2025 + year, month, day, hour, minute);
    }
    else if (s_uppercase.startsWith("UNLOCK ") || s_uppercase.startsWith("RINGTONE "))
    {
      // handled below
    }
    // Further serial commands in full access mode can be added here.
    else
     {
      if (s_uppercase.length() > 0)
        serial_writeln("WARNING: Unknown command");
    }
  } // ^ Full access mode

  if (true)
  { /* The following commands are available in all modes */
    if (s_uppercase.startsWith("UNLOCK "))
    {
      String p1 = s.substring(7);

      uint8_t result = checkUnlockHash(p1);

      if (result == SERIAL_MODE_READONLY)
      {
        MY_SERIAL_MODE = SERIAL_MODE_READONLY;
        serial_writeln("INFO: Switching to serial mode " + String(MY_SERIAL_MODE));
        if (persist_selected_commands) persist_serial_command_for_replay(s);
      }
      else if (result == SERIAL_MODE_FULLACCESS)
      {
        MY_SERIAL_MODE = SERIAL_MODE_FULLACCESS;
        serial_writeln("INFO: Switching to serial mode " + String(MY_SERIAL_MODE));
        if (persist_selected_commands) persist_serial_command_for_replay(s);
      }
      else
      {
        MY_SERIAL_MODE = SERIAL_MODE_SILENT;
      }
    } // ^ UNLOCK
    else if (s_uppercase.startsWith("MISTAKESWEREMADE"))
    {
      serial_writeln("INFO: Wiping device");
      persistence_reset_replay_serial();
      persistence_delete_basecfg();
    }
    else if (s_uppercase.startsWith("RINGTONE "))
    {
      String p1 = s.substring(9);
      int num = p1.toInt();
      if (num == 0)
      {
        serial_writeln("INFO: Disabling new message sound alert");
        set_audio_play(false);
      }
      else
      {
        serial_writeln("INFO: Enabling new message sound alert");
        set_audio_play(true);
      }
      if (persist_selected_commands) persist_serial_command_for_replay(s);
    }
    else
    {
      if (MY_SERIAL_MODE != SERIAL_MODE_FULLACCESS) Serial.println("Device usage restricted.");
    }
  }

  return;
}

// EOF
