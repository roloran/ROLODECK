#ifndef _ROLORAN_BOARD
#define _ROLORAN_BOARD

#include "roloran-tdeck-persistence.h"
#include "roloran-rdcp.h"
#include "tdeck_gui.h"
#include "roloran-tdeck-hal.h"
#include <time.h>

/**
 * Delete the complete message history in order to start fresh. 
 * Removes the history file as well as any message files linked in it. 
 * To be used with "reset of official announcements / infrastructure"-type 
 * events. 
 */
void mb_clear_history(void);

/**
 * Add a locally generated text message (i.e., not received via LoRa or SIMRX)
 * to the message board. It is automatically tagged as relevant for displaying 
 * and relevant for this device only.
 * @param text Textual content of the message 
 * @param refnr A reference number, e.g., as used in a CIRE 
 * @param seqnr A sequence number, e.g., as used in the RDCP Header 
 * @param lifetime 16-bit lifetime specification as used in OAs 
 * @param crisis true if crisis-mode text, false if non-crisis mode text
 */
void mb_add_local_message(char *text, uint16_t refnr, uint16_t seqnr, uint16_t lifetime, bool crisis);

#define RELEVANCE_FOR_THIS_DEVICE_ONLY false
#define RELEVANCE_FOR_EVERYONE true
#define NOT_RELEVANT_FOR_DISPLAYING false
#define RELEVANT_FOR_DISPLAYING true
#define GENERATED_EXTERNALLY false
#define GENERATED_LOCALLY true

/**
 * Add a received message to the message board. 
 * @param text Human-readable text of the message 
 * @param rdcpmsg Base64-encoded RDCP Message 
 * @param origin RDCP Address of message's Origin 
 * @param seqnr RDCP Header SequenceNumber used by Origin for the message 
 * @param refnr Reference number as used in an OA 
 * @param morefrags More Fragments number taken from the message 
 * @param lifetime 16-bit lifetime specification as used in OAs 
 * @param displayrelevance Indicates whether this message should be shown on the display (e.g., false for signature messages)
 * @param deviceonlyrelevance Indicates whether this message is only relevant for this device or for everyone (e.g., INQUIRY vs Public OA)
 * @param crisis true if crisis-mode text, false if non-crisis mode text
 * @param signedprivate true if the RDCP Message is an OA to a unicast address and thus AES-GCM-encrypted/authenticated
 * @param subtype RDCP Message Type "Official Announcement" Subtype (e.g., crisis text, inquiry, ...)
 * @return true if message was added to message board, false if it was not (e.g., already known)
 */
bool mb_add_external_message(char *text, char *rdcpmsg, uint16_t origin, uint16_t seqnr, 
    uint16_t refnr, uint8_t morefrags, uint16_t lifetime, bool displayrelevance, 
    bool deviceonlyrelevance, bool crisis, bool signedprivate, uint8_t subtype);

/**
 * Adds a cryptographic signature from a separate RDCP Signature Message. 
 * The signature is expected to be a 65-byte representation of a Schnorr signature. 
 * The corresponding (multi-fragment) OA is identified by the given reference number. 
 * @param signature 65-byte Schnorr signature as received in an RDCP Message 
 * @param origin 16-bit RDCP Header Origin from which the OA/Signature was sent
 * @param refnr 16-bit OA reference number 
 * @return void
 */
void mb_add_signature(uint8_t *signature, uint16_t origin, uint16_t refnr);

/**
 * Checks the lifetime of all currently stored messages and removes the expired ones.
 * Should be called periodically (e.g., once per minute) in combination with a refresh of 
 * what is shown to the end-user on a display. 
 * @return true if at least one message has expired meanwhile, false if nothing changed
 */
bool mb_check_lifetime(void);

/**
 * Clears the GUI textareas for the OA CRISIS and NONCRISIS screens and add all currently 
 * relevant messages again.
 */
void mb_refresh_display(void);

//< hq_timestamp is a data structure to store RDCP Timestamp message content and metadata
struct hq_timestamp {
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int64_t received = 0;    
};

/**
 * Set this device's wallclock time, e.g., based on received Timestamp messages or Serial commands. 
 * @param year Year to set, e.g., 2025, i.e. 4 digits
 * @param month Month to set, 1--12
 * @param day Day of month, 1--31
 * @param hour Hour (24h format), 0--23
 * @param minute Minute to set, 0--59
 * @return void
 */
void tdeck_set_time(int year, int month, int day, int hour, int minute);

/**
 * Retrieve the currently set wallclock time as time_t. 
 * @return time_t Current wallclock time, or 0 if none is set (e.g., no timestamp received yet). 
 */
time_t tdeck_get_time(void);

/**
 * Check all currently persisted messages for their expiration and update the GUI textareas accordingly. 
 * @param check Set to true if an expiration check should be performed. 
 * @param update_only_if_necessary Re-fill the GUI textareas only if at least one message has meanwhile expired. 
 * @return void 
 * Setting update_only_if_necessary to false forces a re-fill of the GUI textareas with persisted content
 * and may, for example, be used after device power-on. Periodic expiration checks should only update the
 * GUI textareas if necessary for performance and user experience reasons. 
 */
void mb_check_lifetime_and_update_display(bool check, bool update_only_if_neccessary);

/**
 * Update the lifetime of a message identified by its Origin and ReferenceNumber. 
 * If it is a multi-fragment message, all fragments are updated. 
 * @param origin uint16_t RDCP Address of the message's Origin 
 * @param refnr uint16_t ReferenceNumber as used, e.g., in OA subheaders
 * @param lifetime uint16_t New lifetime with magic values according to RDCP specs. 0 to delete the message.
 * @return void
 */
void mb_update_lifetime(uint16_t origin, uint16_t refnr, uint16_t lifetime);

/**
 * Dump the stored messages on Serial
 */
void mb_serial_show_messages(void);

/**
 * Count the message board and display the most recent message on-screen. 
 * @param forced_mode true to force crisis/non-crisis mode; false for "most recent message decides". 
 * @param crisis in forced_mode, set to true for Crisis mode, or false for Non-crisis mode
 */
void mb_count_and_show_last(bool forced_mode = false, bool crisis=false);

/**
 * Update the counters for the persisted message board.
 */
void mb_count(void);

/**
 * Show the n'th crisis or non-crisis message on-screen. 
 * @param msgno Number of crisis or non-crisis message 
 * @param crisis true for crisis mode, false for non-crisis mode
 */
void mb_show_by_number(int msgno, bool crisis);

/**
 * Show the currently loaded message on-screen. 
 * @param as_number Number to show on-screen (for navigation buttons)
 */
void mb_show_current(int as_number);

/**
 * Jump to the first crisis message.
 */
void mb_show_crisis_first(void);

/**
 * Go to the previous crisis message.
 */
void mb_show_crisis_previous(void);

/**
 * Go to the next crisis message.
 */
void mb_show_crisis_next(void);

/**
 * Jump to the last crisis message.
 */
void mb_show_crisis_last(void);

/**
 * Jump to the first non-crisis message.
 */
void mb_show_noncrisis_first(void);

/**
 * Go to the previous non-crisis message.
 */
void mb_show_noncrisis_previous(void);

/**
 * Go to the next non-crisis message.
 */
void mb_show_noncrisis_next(void);

/**
 * Jump to the last non-crisis message.
 */
void mb_show_noncrisis_last(void);

#endif

/* EOF */