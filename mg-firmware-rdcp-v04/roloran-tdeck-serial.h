#ifndef _ROLORAN_TDECK_SERIAL
#define _ROLORAN_TDECK_SERIAL

#include <Arduino.h>
#include "Base64ren.h"
#include <unishox2.h>

#define SERIAL_MODE_SILENT     2
#define SERIAL_MODE_READONLY   1
#define SERIAL_MODE_FULLACCESS 0

/**
 * Open the Serial port for I/O with default settings.
 * To be called once after power-on. 
 */
void serial_begin(void);

/**
 * Print a text banner with some device settings on Serial
 */
void serial_banner(void);

/**
 * Set Pre Banner Mode
 * @param v New Banner Mode value
 */
void set_pre_banner_mode(bool v);

/**
 * Set serial note.
 * @param s New serial note
 */
void set_serial_note(String s);

/**
 * Print a String on the Serial port.
 * Adds device prefix (SERIAL_PREFIX) if second parameter is set to true.
 * Returns false if Serial port is in "silent mode", true otherwise.
 * @param s String to print on Serial/UART connection to PC
 * @param use_prefix true if the default device prefix is to be used, false for no prefix 
 * @return true if the string was printed on Serial, false if it was suppressed (e.g. due to SERIAL_MODE_SILENT)
 */
bool serial_write(String s, bool use_prefix=true);

/**
 * Print a String on the Serial port and add a newline symbol.
 * Same as serial_write(), but with a newline at the end and no return value.
 * @param s String to print on Serial/UART 
 * @param use_prefix true if the default device prefix is to be used, false for no prefix
 */
void serial_writeln(String s, bool use_prefix=true);

/**
 * Convert binary data to Base64 and print it on the Serial port.
 * @param data Binary data to print 
 * @param len Length of binary data in number of bytes 
 * @param add_newline true if a trailing newline should be printed 
 */
void serial_write_base64(char *data, uint8_t len, bool add_newline=false);

/**
 * Compress a String with Unishox2 and print the result Base64-encoded on the Serial port.
 * @param s Text string to be unishox2-compressed and base64-encoded 
 * @param add_newline true if a trailing newline should be printed
 */
void serial_string_to_unishox(String s, bool add_newline=true);

/**
 * Read a String from Serial port.
 * Input is expected to consist of one line of text (with a newline symbol at the end).
 * An empty returned String indicates that no input was available (timeout).
 * @return String read from Serial/UART port; empty String if no input is available 
 */
String serial_readln(void);

/**
 * Process a command received via Serial / UART.
 * @param s String with command to process 
 * @param processing_mode String with processing mode, such as "ECHO: " or "REPLAY: "
 * @param persist_selected_commands true if certain commands may be persisted, false otherwise (e.g., during replay of already persisted commands)
 */
void serial_process_command(String s, String processing_mode="ECHO: ", bool persist_selected_commands=true);

/**
 * Get the current Serial mode
 * @return Current serial mode, e.g., SERIAL_MODE_SILENT 
 */
uint8_t serial_get_serial_mode(void);

/**
 * Set a new Serial mode
 * @param newmode New Serial mode, e.g., SERIAL_MODE_FULLACCESS 
 */
void serial_set_serial_mode(uint8_t newmode);

/**
 * Check whether a new SIMRX line was received via Serial/UART meanwhile.
 * @return true if new Serial/UART input is available, false otherwise 
 */
bool serial_has_new_message(void);

/**
 * Copy the most recently SIMRX-received packet into the given buffer.
 * Returns the number of bytes copied.
 * @param destination Buffer where to store a SIMRX'd base64-encoded LoRa payload 
 * @return number of bytes copied to destination buffer 
 */
uint16_t serial_copy_received_message(uint8_t *destination);

/**
 * @return Timestamp of the most recently SIMRX'd LoRa packet payload
 */
int64_t serial_get_simrx_timestamp(void);

#endif
/* EOF */
