# ROLORAN Neuhaus Mobile Device Firmware for RDCP v0.4

This repository holds the LilyGo T-Deck firmware implementation for RDCP v0.4 mobile devices (MGs) in the Neuhaus scenario v2.0.

The ROLORAN Disaster Communication Protocol (RDCP) is specified in a separate [repository](https://github.com/roloran/RDCP-Specs).

In the Neuhaus scenario, RDCP uses three different kinds of RDCP-capable devices: HQs, DAs, and MGs. MGs are mobile devices for citizens and blue-light organizations to be used in regional crisis situations, including longer black-outs. MGs have been implemented as part of the dtec.bw (#NextGenerationEU) research project [ROLORAN](https://www.unibw.de/software-security/forschung/roloran). LilyGo T-Deck devices are used as commercial-off-the-shelf (COTS) LoRa devices which combine a Semtech SX1262 LoRa radio with a hardware keyboard and touch-screen display at a moderate price.

The RDCP firmware enables citizens to receive RDCP Official Announcements during crisis situations and also send RDCP Citizen Reports, including medical emergencies that trigger emergency aid via the HQ using the DAs in the RDCP Infrastructure as Relays.

The firmware in this repository is the prototype RDCP MG implementation for the Neuhaus scenario, initially deployed for field testing in the second phase of the ROLORAN-Neuhaus cooperation in 2025. It might serve as a blueprint basis for further development and deployments in comparable RDCP scenarios.

## Dependencies, Compilation, and Flashing

Based on compatibility notes and support provided by used libraries, our T-Deck firmware implementation uses the `Arduino CLI` toolchain.
It is recommended to first setup `Arduino IDE` for this project and then switch to the `Arduino CLI`-based workflow outlined here. Using a different environment, such as PlatformIO, should work in theory but is not actively supported.

The Arduino development environment needs to be able to compile code for ESP32-S3; the "ESP32S3 Dev Module" is a good choice within the "boards manager". Note that Espressif v2.0.17 must be used; the more recent v3.x.y versions will not work as expected.

When using the `Arduino IDE` for compilation (note that using `Arduino CLI` is recommended), the following settings need to be changed from their defaults:

- Enable USB CDC On Boot
- Flash size 16 MB
- Partition Scheme 16M Flash (3MB APP)

Several libraries related to T-Deck hardware components are used as provided in [LilyGo's T-Deck repository](https://github.com/Xinyuan-LilyGO/T-Deck), including:

- TouchLib
- lv_conf.h
- lvgl (8.4.0)

Copy them to the directory where Arduino IDE installs libraries. We keep a tested fork of LilyGo's repository [here](https://github.com/roloran/LilyGO-T-Deck).
The lv_conf.h file should be as indicated below, which is stored in the library lvgl as a .template file. Insert this file in the library directory of the Arduino IDE.

```
.
├── ...
├── libraries # Arduino library folder
│ ├── lvgl # lvgl library folder
│ ├── lv_conf.h (insert the template file from lvgl library folder and rename it)
└── ...
```
Via `Arduino IDE`, the following dependencies can be installed:

- GFX Library for Arduino by Moon On Our Nation, v1.5.6
- Crypto by Rhys Weatherley, v0.4.0
- ESP8266Audio, v2.0.0

Additionally, our implementation needs the [Unishox2 library](https://github.com/siara-cc/Unishox_Arduino_lib), which we forked [here](https://github.com/roloran/Unishox_Arduino_lib) and the SX126x-Arduino library, which we forked [here](https://github.com/roloran/SX126x-Arduino) including some SPI-handling patches.

Our implementation integrates the [Base64 library](https://github.com/Xander-Electronics/Base64) with some namespace adjustments so that it does not have to be installed manually.

When all dependencies are met, the firmware can be compiled using `Arduino CLI` as follows:

```bash
arduino-cli compile -v --fqbn "esp32:esp32:esp32s3:CDCOnBoot=cdc,FlashSize=16M,PartitionScheme=app3M_fat9M_16MB" --output-dir build ./
```

Flashing the firmware can be done afterward as follows:

```bash
DEVICE="/dev/your-t-deck-device-here"
arduino-cli upload --fqbn "esp32:esp32:esp32s3:CDCOnBoot=cdc,FlashSize=16M,PartitionScheme=app3M_fat9M_16MB" --port $DEVICE --input-dir build
```

Note that you need to identify your T-Deck device's name first (maybe using `Arduino IDE`). Initially flashing the T-Deck with its pre-installed factory firmware might require pressing the knob down during power-on. Please see LilyGo's documentation first if you have not worked with T-Deck devices before.


## Initial Device Configuration

When a T-Deck has been flashed with our firmware for the first time, it cannot be used without initial configuration via a Serial/UART connection first and will show the "requires maintenance" UI screen. It is recommended to use the [ROLORAN-Terminal](https://github.com/roloran/ROLORAN-Terminal) to access the T-Deck for this purpose.

Initial configuration is part of the "provisioning" process (see [RDCP specs](https://github.com/roloran/RDCP-Specs)) and consists of the "base configuration" workflow outlined below. It is recommended to use ROLORAN-Terminal's scripting capabilities to setup a larger number of devices.

Configuration settings are stored persistently on the T-Deck and restored automatically during the power-on process. Initial configuration needs to be performed again after a device has been wiped explicitly.

## Device Usage

T-Decks are primarily intended as end-user devices participating in RDCP infrastructures, but can also be used for testing and maintenance operations by authorized personnel. Both types of usage are outlined below.

### Interactive (GUI) Usage

As end-user devices, the primary purpose of T-Decks is to display Official Announcements (according to RDCP specs). RDCP infrastructures can be operated in `crisis` and `non-crisis` modes. The major difference is that in `crisis` mode, end-user devices can also be used to send Citizen Request (CIRE) messages according to RDCP specs, while `non-crisis` mode only allows for one-way communication, i.e., citizens cannot send messages back to the HQ.

Device usage typically runs along the following steps:

- An end-user turns the device on. As the optional built-in battery has a very limited capacity, typically external "power banks" are used for this purpose. The T-Decks can also be connected to a USB-C charging device; however, given the worst-case assumption of a "black-out crisis", permanent external power sources are considered to be unavailable.

- After power-on, provisioned devices optionally show some kind of "end-user agreement" screen to let the end-users know that other, still available means of communication (e.g., mobile phone network, landlines, ...) should be preferred, and sending emergencies via RDCP should only be considered as the "last resort" option. It is worth keeping in mind that we operate RDCP infrastructures as a part of an ongoing university research project, not for production use with any kind of legally binding guarantees.

- End-user devices always start in `crisis` mode to allow for sending CIREs quickly after turning the device on, especially in real-world emergency situations. Once the device receives a `non-crisis` message (e.g., a non-crisis official announcement or a status information as part of timestamp messages), it switches to `non-crisis` mode.

- In `non-crisis` mode, the end-user is shown a list of persistently stored `non-crisis` official announcements. New OAs, such as those periodically repeated by DAs or new messages sent by the HQ, are appended to this list. If the device has already received a timestamp message, the date and time of when a message was received are added to the displayed messages along with the information whether a valid cryptographic signature has been received for each message.

- In `crisis` mode, end-users have two additional buttons available (one in red and one in blue) to send emergency and other CIRE messages to the HQ. When using either of them, the UI screen changes to display form fields that have to be filled in (such as "where is the emergency?" and "what happened?"). The form fields and overall design are developed together with Neuhaus' HQ operators to make end-user devices somewhat intuitive to use while still providing all the information that HQ needs to react to new CIREs appropriately.

- Only one CIRE may be sent at a time. `Crisis`-mode buttons are greyed out until an Acknowledgment message has been received from the HQ or the end-user device determines that the message currently cannot be delivered. Similarly, when HQ sends an Inquiry OA, it must be answered before further CIREs can be sent.

Unfortunately, in our experience, sometimes T-Deck hardware components fail to initialize properly after power-on or may even fail at run-time. If the software detects that components are not working properly, it shows a UI screen that asks the end-user to power-cycle the device, which usually fixes the issue somewhat reliably (a software-side reboot, i.e., without a power-cycle, does not suffice). However, if, for example, the device's touch screen stops to work after some time, end-users need to perform the power-cycle after becoming aware of the problem.

To conserve energy, the display is turned off after an inactivity period. It is turned on automatically again when a new OA is received or when the device is used interactively. Unfortunately, the pinout of current T-Deck hardware versions does not allow for the use of ESP32 sleep modes; for example, the SX1262's DIO1 pin could not be used to wake the device from light or deep sleep. Thus, even with other energy-saving options in place (such as lowering CPU frequency), T-Decks continuously consume about 0.4 W even when idle, depleting typical power banks within a few days. We'd be happy to learn about how this situation can be improved.

### Serial Device Access (UART, USB-CDC)

The devices can be accessed over their USB-C port for a Serial/UART connection. Parameters are fixed to 115200/8N1 and it is recommended to use the [ROLORAN-Terminal](https://github.com/roloran/ROLORAN-Terminal) for this purpose.

Provisioned devices dump parts of their configuration (e.g., LoRa parameters, RDCP address) after power-on over this channel. Unprovisioned or unlocked devices accept the following commands:

- `RESET` commands:
  - `RESET CONFIG` removes persisted commands for replay outside basecfg.
  - `RESET RADIO` re-initalizes the LoRa radio with the current settings.
  - `RESET BASECFG` wipes the device. It will require a power-cycle and re-provisioning afterwards.
  - `RESET ENTRYPOINTS` deletes all stored RDCP Entry Points.
  - `RESET MULTICAST` deletes all stored RDCP multicast addresses.
  - `RESET MESSAGES` deletes all messages from both `crisis` and `non-crisis` message boards.
  - `RESET FILESYSTEM` formats the FFat file system while trying to keep the configuration.

- `SHOW` commands:
  - `SHOW CONFIG` outputs the current device configuration (same output as after device power-on).
  - `SHOW MESSAGES` lists the messages stored in the internal message boards.

- LoRa parameter commands:
  - `LORAFREQ` sets the LoRa channel frequency in MHz, e.g., `LORAFREQ 868.2`
  - `LORABW` sets the LoRa channel bandwidth in KHz, e.g., `LORABW 125`
  - `LORASF` sets the LoRa spreading factor, e.g., `LORASF 12` (accepted values: 7 to 12)
  - `LORACR` sets the LoRa coding rate to x/8, e.g., `LORACR 5` (accepted values: 5 to 8, as in RadioLib)
  - `LORASW` sets the LoRa syncword (hexadecimal), e.g., `LORASW 12` (accepted values: 12 and 34)
  - `LORAPW` sets the LoRa TX power, e.g., `LORAPW 14` (accepted values: up to 22 with T-Deck's SX1262)
  - `LORAPL` sets the LoRa preamble length, e.g., `LORAPL 15` (15 is the default value in our scenarios)

- `TX <base64>` queues an outgoing RDCP message provided as Base64-encoded string.

- `TXNOW <base64>` queues an outgoing RDCP message for immediate sending (use with care!).

- `TXSCHED <ms> <base64>` queues an outgoing RDCP message to be sent after the delay given in milliseconds.

- `SIMRX <base64>` simulates receiving a LoRa packet whose payload is given Base64-encoded. This is solely intended for the purpose of testing how the device reacts to a specific RDCP message without actually transmitting it "over the air" (e.g., under duty cycle considerations).

- `RESTART` triggers a reboot of the device (not a full power-cycle!).

- `BASECFG` starts the base configuration procedure for an unprovisioned device. After this command, any of the other commands can be sent and they will be stored persistently on the device to be executed after each power-on. Similar to how mails are sent through SMTP, a single `.` on a line of its own will end the `BASECFG` mode. The device should then be power-cycled and Serial/UART output should be checked for any error messages such a configuration setting may give. It is recommended to use configuration templates along with ROLORAN-Terminal's scripting capabilities for provisioning. Base configuration should include all of the `LORA*` commands if they deviate from the firmware's default settings in a scenario. `BASECFG` _must_ include `RDCPADDR`, setting at least one `ENTRYPOINT`, the `HQPUBKEY` and the `SHAREDSECRET`. If the device shall become part of at least one multicast group, the `MULTICAST` command must be used. It is recommended to set an `OWNER` if this information is available during provisioning. Other commands such as `HEARTBEAT`, `GRACETIME`, `CIRETIMEDA`, `CIRETIMEHQ`, and `EULA` _may_ be used to deviate from default settings. Please note that a faulty `BASECFG` can render the device useless and unconfigurable. If you brick it, you need to flash a new firmware that initially removes the base configuration. This is not a toy command.

- `RDCP` commands:
  - `RDCPIDUP` toggles whether the device ignores or filters duplicated RDCP messages (based on Origin and SequenceNumber). Use with care, as most of the firmware implementation relies on duplicate filtering quite heavily. You've been warned.
  - `RDCPADDR` sets the RDCP address the device should use as its own, e.g., `RDCPADDR AEFF`.
  - `ENTRYPOINT` adds an Entry Point for the device, usually the nearest DA, e.g., `RDCPADDR 0200`; can be used up to 5 times. Use `RESET ENTRYPOINTS` to remove the existing setting if you want to replace instead of to add.
  - `MULTICAST` adds the device to a multicast group, e.g., `MULTICAST AF00`; can be used up to 5 times. See also `RESET MULTICAST`.
  - `HEARTBEAT` sets the time interval between Heartbeat messages sent by the device in minutes; default is `HEARTBEAT 30`.
  - `GRACETIME` sets the grace period in seconds; default is `GRACETIME 60`. A device will not send a Heartbeat message for this duration after power-on in order to first listen to other RDCP messages and update its channel free estimator.
  - `CIRETIMEDA` sets the time to wait for an Acknowledgment message by the Entry Point after sending a CIRE in seconds. See the RDCP specs [section about CIREs](https://github.com/roloran/RDCP-Specs/blob/main/rdcp_v04_overview.md#rdcp-messages-for-citizen-reports).
  - `CIRETIMEHQ` sets the time to wait for an Acknowledgment message by the HQ after sending a CIRE in seconds.
  - `RDCPPING` sends an RDCP Echo Request message to another device, e.g., `RDCPPING 0200`.
  - `RDCPTEST` sends an RDCP Test message to a given RDCP address, e.g., `RDCPTEST FFFF Hello world`.
  - `RDCPCIRE` sends an RDCP Citizen Request message to the HQ. Subtype and ReferenceNumber must be given along with a text in the format that the HQ expects, e.g., `RDCPCIRE 00 0001 !#...`.
  - `RDCPACKS` sends a signed Acknowledgment message; this only works if a private key is stored on the device. The command expects Origin, Destination, acknowledged SequenceNumber, and Acknowledgment type as parameters. Note that usually only the HQ sends such messages and the HQ's private key must not be stored on end-user devices.
  - `RDCPACK` sends an (unsigned) Acknowledgment message; same as `RDCPACKS` but without the Schnorr signature. Note that usually only DAs send such acknowledgments.
  - `RDCPTIME` sends a signed Timestamp message to the broadcast address, e.g., `RDCPTIME 00 12 31 23 59 02` (see RDCP specs). Note that usually only the HQ sends such messages and other devices only accept it if properly signed.

- Other device settings:
  - `SCREENSAVER` sets the delay before the screen is turned off when the device is idle in milliseconds. Default is `SCREENSAVER 30000`.
  - `UISCREEN` switches to a different UI screen, e.g., `UISCREEN 1`. This is only useful for UI testing or when giving device demonstrations. See `tdeck_gui.h` for the UI screen numbers.
  - `EULA` toggles whether the "end user license" with the note to only use CIREs as a method of last resort should be shown on power-on.
  - `OWNER` sets the device owner's name. It is used to pre-fill the "who reports?" CIRE form fields.
  - `HQPUBKEY` sets the HQ's public key used to verify Schnorr signatures. Uses a sequence of hex digits as exported by the Schnorr stand-alone tool.
  - `MYPUBKEY` sets this device's public key. Only used for maintenance devices that may create signed RDCP messages. Not used for end-user devices.
  - `MYPRIVKEY` sets this device's private key, corresponding to `MYPUBKEY`. Not used for end-user devices.
  - `SHAREDSECRET` sets the shared secret with the HQ, a 256-bit AES key, given as 32 hex digits.
  - `SETTIME` uses the same syntax as `RDCPTIME` but only sets the local device wallclock. Useful for some tests if the next official Timestamp message is not expected anytime soon after device power-on.
  - `UNLOCK` changes the Serial/UART access mode based on a given password, e.g., `UNLOCK nicetry`. The higher the unlocked level the more information will be output via Serial/UART and the more commands can be used. Repeated failed unlock attempts may brick the device. Passwords can be set with the `UNLOCKHASH1` and `UNLOCKHASH2` commands.
  - `RINGTONE 0` disables the short melody when new messages are displayed.
  - `NOTE` can be used to add an infix to each Serial output text line.
  - `MISTAKESWEREMADE` bricks the device. 

Some of those commands are automatically persisted on the device and replayed after power-on, so they don't have to be repeated every time the device is used. Those replayed commands override even `BASECFG` settings. Using `!` as a prefix for the command prevents it from being echoed back.

Unlocked devices show received LoRa packets using the `RX <base64>` and `RXMETA` lines as the other ROLORAN LoRa modems do.

Sending an empty input line to an unlocked T-Deck outputs some current status information, e.g., channel free estimation and number of queued messages.

## Source code overview

For developers and contributors, the firmware's Arduino C source code is structured similar to previous T-Deck firmware versions and other LoRa device implementations in the ROLORAN project:

- `mg-firmware-rdcp-v04.ino` is the starting point with the typical setup / loop functions of Arduino programs.

- `roloran-tdeck-hal.h` serves as hardware abstraction layer for the components used in T-Decks.

- `roloran-tdeck-serial.h` provides functions for Serial/UART input and output.

- `roloran-tdeck-persistence.h` bundles the FFat-based data persistence functions.

- `roloran-lora.h` provides access to the T-Deck's SX1262 through the modified SX126x-Arduino library.

- `roloran-board.h` handles the messages to be displayed to the end-users on the crisis / non-crisis message board screens.

- `roloran-crypto.h` bundles the AES-256-GCM AEAD and Schnorr signature functions.

- `settings-device.h` provides any configuration and device settings.

- `tdeck_gui.h` handles content management and callback functions for the UI screens.

- `ui*` hold the resources for anything UI-related including fonts.

- `roloran-audio.h` plays a short melody through T-Deck's internal buzzer.

## Limitations and the way ahead

RDCP MGs in the Neuhaus scenario are currently intended to be used as a prototype by citizens and selected authority personnel. Its interactive user interface (GUI) will evolve based on experience gathered in the ROLORAN research project and the use of its Serial/UART/USB-CDC interface is limited to technical staff operating the RDCP infrastructure. Intuitive user experiences and reliable, stable operations are currently the primary goals. Secondary future objectives may involve additional functionality and interfaces (e.g., coupling with smartphones via Bluetooth) as well as cooperation with commercial vendors interested in creating more than a research prototype.

<!-- EOF -->
