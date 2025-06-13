/*
    ROLORAN T-Deck as mobile device in the Neuhaus scenario

    Implementation for "scenario v2.0 (2025)" based on RDCP v0.4

    Developed in Arduino IDE / using Arduino CLI
    Board type: ESP32S3 Dev Module (Boards manager: esp32 by Espressif Systems, v2.0.17 - do not use v3.x.x!)
    Non-default settings: (cf. https://github.com/Xinyuan-LilyGO/T-Deck)
    - Enable USB CDC On Boot
    - Flash size 16 MB
    - Partition Scheme 16M Flash (3MB APP)

    UI-related files created with SquareLine Studio

    Used libraries and their configuration files to be taken from
       https://github.com/Xinyuan-LilyGO/T-Deck
       (includes: AceButton ESP32-audioI2S TFT_eSPI TinyGPSPlus TouchLib es7210 lv_conf.h lvgl)
       copy them to the directory where Arduino IDE installs libraries (on macOS: ~/Documents/Arduino/libraries/)
       (LVGL 8.4.0 is used)

       https://github.com/siara-cc/Unishox_Arduino_lib (not installable through Arduino IDE)
       https://github.com/roloran/SX126x-Arduino (based on Bernd Giesecke's SX126x-Arduino library)
       TODO (Schnorr signature library)

    Install libraries via Arduino IDE:
    - GFX Library for Arduino by Moon On Our Nation, v1.5.6
    - Crypto by Rhys Weatherley, v0.4.0

    Additional libraries included with this project, _not_ to be installed manually:
    - Base64 by Arturo Guadalupi, https://github.com/Xander-Electronics/Base64, v1.0.0,
      name adjusted to Base64ren and included in this project's directory
*/

#include <Arduino.h>
#include <esp_timer.h>
#include <lvgl.h>

#include "roloran-tdeck-serial.h"       // Serial/UART communication
#include "roloran-tdeck-hal.h"          // T-Deck hardware-related functions
#include "roloran-tdeck-persistence.h"  // File-system / persistence related functions
#include "settings-device.h"            // Device-specific settings
#include "settings-scenario.h"          // Scenario-specific settings
#include "roloran-lora.h"               // LoRa-specific functions
#include "roloran-rdcp.h"               // RDCP-related settings and functions
#include "ui.h"                         // GUI as exported from SquareLine Studio
#include "tdeck_gui.h"                  // GUI helper functions
#include "roloran-board.h"              // Message Board functions

#define BUFSIZE 512

SET_LOOP_TASK_STACK_SIZE(16*1024);      // default of 8 kb is not enough

/* Information about the most recently received LoRa packet */
uint8_t  lora_packet[BUFSIZE];
uint16_t lora_packet_length = 0;
int64_t  lora_packet_timestamp = 0;

bool provisioned = false; // has the device already been provisioned?
extern bool has_txed;

int64_t minutetimer = -1; // track milliseconds so we execute some tasks only about once per minute
int32_t old_free_heap = ESP.getFreeHeap();
int32_t old_min_free_heap = ESP.getMinFreeHeap();
int32_t free_heap = 0;
int32_t min_free_heap = 0;

/**
 * setup() function, called once after device power-on.
 * Initializes the T-Deck hardware components (order is relevant) and retrieves
 * persistenly stored configuration and data.
 */
void setup(void)
{
  tdeck_setup();                // Initialize the LilyGo hardware; must be first
  radio_setup();                // Initialize SX1262; must be second due to SPI quirks
  persistence_setup();          // Initialize persistence
  provisioned = persistence_configure_device_settings(); // Initialize device settings based on provisioning

  if (provisioned)
  {
    persistence_replay_serial();  // Update device configuration with stored Serial commands
    serial_writeln("READY");      // Signal LoRa Modem readiness over Serial
  }
  else
  {
    serial_set_serial_mode(SERIAL_MODE_FULLACCESS);
    set_pre_banner_mode(false);
    serial_writeln("WARNING: DEVICE NEEDS PROVISIONING. PLEASE START BASECFG PROCEDURE!");
  }

  gui_callback_setup();         // Set up the GUI callbacks missing in SquareLine Studio export

  return;
}

/**
 * main loop() function, executed over and over again.
 */
void loop(void)
{
  static bool first_loop = true;

  if (first_loop)
  { // Only perform the following actions once.
    // Separated from the setup() function as they are part of the "user experience", not the power-on process
    first_loop = false;
    if (provisioned)
    {
      delay(200);
      tdeck_loop();
      delay(200);
      set_gui_needs_screen_refresh(true);
      tdeck_loop();
      if (shall_show_eula())
      {
        gui_transition_to_screen(SCREEN_EULA); // Default screen on start-up
      }
      else
      {
        gui_transition_to_screen(SCREEN_OACRISIS); // Directly go to OA CRISIS screen if no EULA is shown
      }
      set_gui_needs_screen_refresh(true);
      tdeck_loop();
      serial_banner();  // Output current device configuration over Serial
      mb_count_and_show_last(true, true); // Display latest message board entry if available
      tdeck_loop();
      mb_check_lifetime_and_update_display(true, true); // Check expiration dates
    }
    else
    {
      gui_transition_to_screen(SCREEN_PROVISIONING);  // Default screen if device has not yet been provisioned
    }

    rdcp_check_cirefile(); // Send any persisted CIRE if we lost power before having it ACK'd or given up
  }

  tdeck_loop(); // Periodically let the GUI do its work
  radio_loop(); // Periodically let the Radio API do its work
  check_screensaver_activation(); // Turn on the screensaver if device is idle

  String s = serial_readln(); // Check for new commands received over Serial
  if (s.length() != 0)
  {
    serial_process_command(s); // Process any new Serial commands
  }

  tdeck_loop(); // Periodically let the GUI do its work (again)

  bool shall_process_new_message = false;
  if (lora_has_new_message() == true) // Pre-process new packets received over LoRa
  {
    lora_packet_length = lora_copy_received_message(lora_packet); // Copy the LoRa packet so we can receive a new one while processing this one
    lora_packet_timestamp = lora_get_rx_timestamp();
    shall_process_new_message = true;
  }
  if (serial_has_new_message() == true) // Pre-process new SIMRX messages if available
  {
    lora_packet_length = serial_copy_received_message(lora_packet); // The SIMRX interface mimics actually received LoRa packets
    lora_packet_timestamp = serial_get_simrx_timestamp();
    shall_process_new_message = true;
  }
  if (shall_process_new_message) // Process a fresh received / SIMRX'd LoRa packet
  {
    serial_writeln("INFO: New LoRa packet to process, length " + String(lora_packet_length) + " @" + String(my_millis()) + " ms");
    rdcp_mg_process_rxed_lora_packet(lora_packet, lora_packet_length, lora_packet_timestamp); // main LoRa packet processing function call
  }

  tdeck_loop();           // Periodically let the GUI do its work (yes, again)
  rdcp_txqueue_loop();    // Periodically let the TX scheduler do its work
  tdeck_loop();           // Periodically let the GUI do its work (once more...)
  rdcp_cire_check();      // Check for timeouts of sent CIRE messages, i.e., no ACKs received from DA or HQ
  rdcp_heartbeat_check(); // Check whether we should send a HEARTBEAT message

  if ((minutetimer == -1) || (my_millis() > minutetimer + 5 * MINUTES_TO_MILLISECONDS)) // execute initially and roughly once every five minutes
  {
    minutetimer = my_millis();
    if (is_screensaver_on()) mb_check_lifetime_and_update_display(true, true); // lifetime check blocks device for several seconds, only do it with screensaver on
    rdcp_blockdevice_check();

    rdcp_dump_queues();

    free_heap = ESP.getFreeHeap();
    min_free_heap = ESP.getMinFreeHeap();
    if ((free_heap < old_free_heap) || (min_free_heap < old_min_free_heap))
    {
      char info[INFOLEN];
      snprintf(info, INFOLEN, "WARNING: Free heap dropped from %d/%d to %d/%d",
        old_min_free_heap, old_free_heap, min_free_heap, free_heap);
      serial_writeln(info);
      old_min_free_heap = min_free_heap;
      old_free_heap = free_heap;
      if ((free_heap < 32768) && is_screensaver_on())
      {
        serial_writeln("ERROR: OUT OF MEMORY - restarting as countermeasure");
        delay(1000);
        ESP.restart();
      }
    }
  }

  return;
}

/* EOF */
