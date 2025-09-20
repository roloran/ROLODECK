#include <LittleFS.h>
#include "roloran-board.h"
#include "roloran-tdeck-persistence.h"
#include "roloran-rdcp.h"
#include "tdeck_gui.h"
#include "roloran-tdeck-hal.h"
#include "roloran-tdeck-serial.h"
#include "settings-device.h"
#include <time.h>
#include "roloran-board.h"
#include "ui.h"
#include "roloran-lora.h"
#include "roloran-crypto.h"
#include <SHA256.h>
#include "roloran-audio.h"
#include "settings-scenario.h"

// Which file to persist message board entries in
#define FILENAME_HISTORY "/history.dat"
#define FILENAME_HISTTMP "/history.new"
#define MAX_SHOWN_ENTRIES 64
#define CONTENT_RDCP_SIZE 270 
#define CONTENT_TEXT_SIZE 512

//< Data structure for a single history entry in the history file
struct history_entry {
    bool local = true;                //< locally created message (vs. received via RDCP message)
    bool display = false;             //< relevant for displaying on-screen (e.g., false for signatures)
    bool forall = false;              //< relevant for everyone (true) or only this device (false)
    bool crisis = true;               //< crisis vs. non-crisis message (separate GUI textareas)
    bool signature_available = false; //< True if a signature for the message is available or message is locally generated
    bool signature_verified = false;  //< True if the signature has been verified or message is locally generated
    uint16_t origin = RDCP_LOCAL_ORIGIN;       //< RDCP Origin of the message or 0 for locally created messages
    uint16_t seqnr = RDCP_NO_SEQUENCE_NUMBER;  //< RDCP Header SequenceNumber or number of multi-step local message
    uint16_t refnr = RDCP_NO_REFERENCE_NUMBER; //< ReferenceNumber as used, e.g., in OA and CIRE subheaders
    uint16_t lifetime = NO_DURATION;             //< lifetime setting for message (can have magic values according to RDCP specs)
    uint8_t morefragments = 0;        //< Number of subsequently following/expected fragments of the same message
    char content_rdcp[CONTENT_RDCP_SIZE];      //< Base64-encoded RDCP message (LoRa packet payload) for received messages
    char content_text[CONTENT_TEXT_SIZE];          //< Textual content of the message (decrypted and de-unishox2'd for received messages)
    uint8_t subtype = 0;              //< Subtype of the message, e.g., inquiry or crisis txt message
    int64_t timestamp_added = NO_TIMESTAMP;     //< my_millis() timestamp of when the message was added to the message board
    time_t timestamp_added_abs = NO_TIMESTAMP;  //< time_t of wallclock time of when message was added (if available)
    time_t expiry_absolute = NO_TIMESTAMP;      //< Wallclock expiration timestamp (if available)
    char footer[8];                   // must be "RDCP"; used to detect history file corruption, e.g. due to power-off during writing
};

history_entry cur_he;  // single currently processed history entry (not enough memory to hold them all at once)
history_entry dupe_he; // for duplicate checking
File histfile;         // File handle for accessing the history file

hq_timestamp hqts;    // Timestamp provided by HQ
bool has_external_time = false; // true if we received a Timestamp since power-on

int msg_crisis_current = 0;
int msg_crisis_total = 0;
int msg_crisis_last = 0;
int msg_noncrisis_current = 0;
int msg_noncrisis_total = 0;
int msg_noncrisis_last = 0;
extern uint16_t highest_oa_refnr;

extern bool infrastructure_in_crisis;

void tdeck_set_time(int year, int month, int day, int hour, int minute)
{
    hqts.year = year;
    hqts.month = month;
    hqts.day = day;
    hqts.hour = hour;
    hqts.minute = minute;
    hqts.received = my_millis();
    has_external_time = true;

    char info[INFOLEN];
    snprintf(info, INFOLEN, "INFO: T-Deck Wallclock Time set to %02d.%02d.%04d %02d:%02d @%lu", day, month, year, hour, minute, (unsigned long) hqts.received);
    serial_writeln(info);

    return;
}

time_t tdeck_get_time(void)
{
    if (!has_external_time) return 0; // magic value if no wallclock time is available

    struct tm t = {0};
    t.tm_year = hqts.year - 1900; // needs year - 1900
    t.tm_mon = hqts.month;
    t.tm_mday = hqts.day;
    t.tm_hour = hqts.hour;
    t.tm_min = hqts.minute;
    t.tm_sec = 0;
    t.tm_isdst = -1;
    time_t e = mktime(&t);
    time_t now = (time_t) my_millis() / 1000;
    time_t diff = now - (time_t) (hqts.received / 1000);
    e += diff;

    return e;
}

struct refnr_tracker {
    int nextpos = 0;
    uint16_t refnrs[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

refnr_tracker deleted_oas;

void mb_refnr_add_blocked(uint16_t refnr)
{
    bool found = false;
    for (int i=0; i<10; i++) if (deleted_oas.refnrs[i] == refnr) found = true;
    if (!found)
    {
        deleted_oas.refnrs[deleted_oas.nextpos] = refnr;
        deleted_oas.nextpos = (deleted_oas.nextpos + 1) % 10;
    }
    return;
}

bool mb_refnr_is_blocked(uint16_t refnr)
{
    for (int i=0; i<10; i++) if (deleted_oas.refnrs[i] == refnr) return true;
    return false;
}

/**
 * Convert the lifetime given in an Official Announcement RDCP Message
 * to seconds according to RDCP specs.
 * Returns 0 for unlimited lifetime.
 * Note that a lifetime of 0 in the RDCP Message indicates deletion.
 * @param lt Lifetime specification
 * @return time_t Converted lifetime in number of wallclock seconds
 */
time_t lifetime_to_seconds(uint16_t lt)
{
    time_t lifetime = 0;

    if (lt == 65535) lifetime = 0; // magic value for unlimited lifetime
    else if (lt > 0)
    {
      lifetime = lt * 60; // convert from minutes to seconds
      if (lt > 60000) // magic value range for whole days
      {
        // lifetime must be converted to "full days in seconds"
        lifetime = (lt - 60000) * 60 * 60 * 24;
      }
    }

    return lifetime;
}

/**
 * @return int Number of history entries currently stored in history file or -1 if no history file available
 */
int histfile_numentries(void)
{
    if (!hasStorage) return -1;
    if (histfile) histfile.close();
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    if (!histfile) return -1 ; // { xSemaphoreGive(highlander); return -1; }
    size_t filesize = histfile.size();
    histfile.close();
    return filesize / sizeof(history_entry);
}

void mb_zap_counters(void)
{
    msg_crisis_current = 0;
    msg_crisis_last = 0;
    msg_crisis_total = 0;
    msg_noncrisis_current = 0;
    msg_noncrisis_last = 0;
    msg_noncrisis_total = 0;
    highest_oa_refnr = RDCP_NO_REFERENCE_NUMBER;
    return;
}

void mb_clear_history(void)
{
    if (!hasStorage) return;
    if (histfile) histfile.close();
    serial_writeln("INFO: Clearing Message Board persistent history file");
    LittleFS.remove(FILENAME_HISTORY);
    mb_zap_counters();
    mb_count_and_show_last(true, infrastructure_in_crisis);
    return;
}

void mb_add_local_message(char *text, uint16_t refnr, uint16_t seqnr, uint16_t lifetime, bool crisis)
{
    if (!hasStorage) return;

    cur_he.local = GENERATED_LOCALLY;
    cur_he.display = RELEVANT_FOR_DISPLAYING;
    cur_he.forall = RELEVANCE_FOR_THIS_DEVICE_ONLY;
    cur_he.crisis = crisis;
    cur_he.origin = getMyRDCPAddress();
    cur_he.seqnr = seqnr;
    cur_he.refnr = refnr;
    cur_he.morefragments = 0;
    snprintf(cur_he.content_text, CONTENT_TEXT_SIZE, "%s\0", text);
    snprintf(cur_he.content_rdcp, CONTENT_RDCP_SIZE, "\0");
    cur_he.signature_available = false;
    cur_he.signature_verified = false;
    cur_he.timestamp_added = my_millis();
    cur_he.lifetime = lifetime;

    time_t absexp = tdeck_get_time();
    cur_he.timestamp_added_abs = absexp;

    if (absexp > NO_TIMESTAMP)
    {
        absexp += lifetime_to_seconds(lifetime);
        cur_he.expiry_absolute = absexp;
    }
    else
    {
        cur_he.expiry_absolute = NO_TIMESTAMP;
    }
    snprintf(cur_he.footer, 8, "RDCP\0");

    char info[INFOLEN];
    snprintf(info, INFOLEN, "INFO: Written Message Board local history entry, timestamp: %lu", cur_he.timestamp_added_abs);
    serial_writeln(info);

    if (histfile) histfile.close();
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_APPEND);
    if (!histfile) return;
    histfile.write((uint8_t*) &cur_he, sizeof(history_entry));
    histfile.close();

    if (cur_he.display == RELEVANT_FOR_DISPLAYING)
    {
        mb_count_and_show_last();
        if (is_screensaver_on()) screensaver_off();
        audio_play_melody();
    }

    return;
}

/**
 * Check whether we have received all fragments and a signature for the RDCP Message identified by Origin and RefNr.
 * If so, verify the signature and update the MB accordingly.
 */
void mb_check_for_signature(uint16_t origin, uint16_t refnr)
{
    if (!hasStorage) return;

    bool has_signature = false;
    bool has_all_fragments = true;
    bool fragsavail[256];

    for (int i=0; i<256; i++) fragsavail[i] = false;

    int num_entries = histfile_numentries();
    if (num_entries == -1) return;

    uint8_t schnorrsig[SIGBUFLEN];
    int highest_fragment = 0;
    bool first_fragment = true;

    if (histfile) histfile.close();
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    if (!histfile) return;

    SHA256 h = SHA256();
    h.reset();
    uint8_t data_to_sign[DATABUFLEN];
    uint8_t data_to_sign_length = 0;

    bool has_at_least_one_fragment = false;

    for (int i=0; i<num_entries; i++)
    {
        if (histfile.read((uint8_t*)&cur_he, sizeof(cur_he)) != sizeof(cur_he))
        {
            histfile.close();
            return;
        }
        if (strcmp(cur_he.footer, "RDCP"))
        {
            serial_writeln("ERROR: Bad Message Board history entry, deleting history file");
            histfile.close();
            mb_clear_history();
            return;
        }

        if ((cur_he.origin == origin) && (cur_he.refnr == refnr))
        {
            if ((cur_he.display == NOT_RELEVANT_FOR_DISPLAYING) && (cur_he.seqnr == RDCP_NO_SEQUENCE_NUMBER) && (cur_he.local == GENERATED_EXTERNALLY))
            {
                has_signature = true;
                for (int i=0; i<65; i++) schnorrsig[i] = cur_he.content_text[i];
            }
            else if (cur_he.display == RELEVANT_FOR_DISPLAYING)
            {
                uint8_t cf = cur_he.morefragments;
                if (cf > highest_fragment) highest_fragment = cf;
                fragsavail[cf] = true;
                has_at_least_one_fragment = true;

                uint8_t rm[DATABUFLEN];
                int b64msg_len = strlen(cur_he.content_rdcp);
                int decoded_length = Base64ren.decodedLength(cur_he.content_rdcp, b64msg_len);
                Base64ren.decode((char *)rm, cur_he.content_rdcp, b64msg_len);

                if (first_fragment)
                { // hash static fields taken from first fragment (in-order delivery guaranteed by specs :-))
                    first_fragment = false;

                    data_to_sign[0] = rm[2]; // Origin
                    data_to_sign[1] = rm[3]; // Origin
                    data_to_sign[2] = rm[6]; // Destination
                    data_to_sign[3] = rm[7]; // Destination
                    data_to_sign[4] = rm[8]; // Type
                    data_to_sign[5] = rm[16]; // Subtype
                    data_to_sign[6] = rm[17]; // RefNr
                    data_to_sign[7] = rm[18]; // RefNr
                    data_to_sign[8] = rm[19]; // Lifetime
                    data_to_sign[9] = rm[20]; // Lifetime
                    data_to_sign[10] = rm[21]; // MoreFrags
                    data_to_sign_length = 11;
                    h.update(data_to_sign, data_to_sign_length);
                }

                // hash unishox2-compressed content
                for (int i=22; i<decoded_length; i++) data_to_sign[i-22] = rm[i];
                data_to_sign_length = decoded_length - 22;
                h.update(data_to_sign, data_to_sign_length);
            }
        }
    } // read whole file
    histfile.close();

    char info[INFOLEN];

    for (int i=0; i<highest_fragment; i++)
    {
      if (fragsavail[i] == false) has_all_fragments = false;
    }

    bool signature_valid = false;

    uint8_t sha[SHABUFLEN];
    if (!has_at_least_one_fragment) has_all_fragments = false;
    if (has_signature && has_all_fragments)
    {
      h.finalize(sha, SHABUFLEN);
      signature_valid = schnorr_verify_signature(sha, SHABUFLEN, schnorrsig);
    }

    snprintf(info, INFOLEN, "INFO: Signature check for %04X-%04X yields [%d highfrag, %s avail; sig%savail; sig%svalid]",
        origin, refnr, highest_fragment, has_all_fragments?"all":"not all", has_signature?" ":" not ", signature_valid?" ":" not ");
    serial_writeln(info);

    if (signature_valid)
    {
        if (histfile) histfile.close();
        histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
        if (!histfile) return;

        File nf = LittleFS.open(FILENAME_HISTTMP, FILE_WRITE);
        if (!nf)
        {
            histfile.close();
            return;
        }

        int i=0;

        while (histfile.read((uint8_t*)&cur_he, sizeof(history_entry)) == sizeof(history_entry))
        {
            if (strcmp(cur_he.footer, "RDCP"))
            {
                serial_writeln("ERROR: Bad MB history file entry, deleting history file");
                histfile.close(); nf.close();
                LittleFS.remove(FILENAME_HISTORY);
                LittleFS.remove(FILENAME_HISTTMP);
                mb_zap_counters();
                return;
            }

            bool drop_entry = false;

            if ((cur_he.origin == origin) && (cur_he.refnr == refnr) && (cur_he.display == RELEVANT_FOR_DISPLAYING) && (cur_he.signature_verified == false))
            {
                cur_he.signature_available = true;
                cur_he.signature_verified = true;
                snprintf(info, INFOLEN, "INFO: Rewriting Message Board history entry %d to reflect verified signature", i);
                serial_writeln(info);
            }

            if ((cur_he.origin == origin) && (cur_he.refnr == refnr) && (cur_he.display == NOT_RELEVANT_FOR_DISPLAYING))
            {
                drop_entry = true;
                snprintf(info, INFOLEN, "INFO: Dropping Message Board history entry %d as signature has been verified", i);
                serial_writeln(info);
            }

            if (!drop_entry) nf.write((uint8_t*)&cur_he, sizeof(history_entry));

            i += 1;
        }
        histfile.close();
        nf.close();
        LittleFS.remove(FILENAME_HISTORY);
        LittleFS.rename(FILENAME_HISTTMP, FILENAME_HISTORY);
        serial_writeln("INFO: Finished rewriting Message Board history file after new valid signature");

        mb_count_and_show_last();
    }

    return;
}

bool mb_add_external_message(char *text, char *rdcpmsg, uint16_t origin, uint16_t seqnr, uint16_t refnr, uint8_t morefrags, uint16_t lifetime, bool displayrelevance, bool deviceonlyrelevance, bool crisis, bool signedprivate, uint8_t subtype)
{
    if (!hasStorage) return false;

    if (refnr > highest_oa_refnr) highest_oa_refnr = refnr;

    if (mb_refnr_is_blocked(refnr))
    { 
        serial_writeln("INFO: Message board refuses to add external OA with blocked reference number");  
        return false;
    }

    bool play_ringtone = true;
    if (strstr(text, "***") != NULL) play_ringtone = false; // disable ringtone based on message content

    cur_he.local = GENERATED_EXTERNALLY;
    cur_he.display = displayrelevance;
    cur_he.forall = deviceonlyrelevance;
    cur_he.crisis = crisis;
    cur_he.origin = origin;
    cur_he.seqnr = seqnr;
    cur_he.refnr = refnr;
    cur_he.morefragments = morefrags;
    snprintf(cur_he.content_rdcp, CONTENT_RDCP_SIZE, "%s\0", rdcpmsg);
    snprintf(cur_he.content_text, CONTENT_TEXT_SIZE, "%s\0", text);
    cur_he.signature_available = false;
    cur_he.signature_verified = false;
    cur_he.timestamp_added = my_millis();
    cur_he.lifetime = lifetime;
    cur_he.subtype = subtype;

    if (signedprivate)
    {
        cur_he.signature_available = true;
        cur_he.signature_verified = true;
    }

    time_t absexp = tdeck_get_time();
    cur_he.timestamp_added_abs = absexp;

    if (absexp > NO_TIMESTAMP)
    {
        absexp += lifetime_to_seconds(lifetime);
        cur_he.expiry_absolute = absexp;
    }
    else
    {
        cur_he.expiry_absolute = NO_TIMESTAMP;
    }
    snprintf(cur_he.footer, 8, "RDCP\0");

    if (histfile)
    {
        serial_writeln("WARNING: Histfile already open");
        histfile.close();
    }

    // check for duplicate entries
    bool is_dupe = false;
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    while (histfile.read((uint8_t*)&dupe_he, sizeof(history_entry)) == sizeof(history_entry))
    {
        if ( (dupe_he.origin == cur_he.origin) &&
             (dupe_he.seqnr  == cur_he.seqnr)  &&
             (dupe_he.refnr  == cur_he.refnr)  &&
             (dupe_he.morefragments == cur_he.morefragments))
             {
              is_dupe = true;
              break;
             }

        /* Treat as duplicate if it is a signature for an already verified message */
        if ( (dupe_he.origin == cur_he.origin) &&
             (dupe_he.refnr  == cur_he.refnr)  &&
             (cur_he.display == NOT_RELEVANT_FOR_DISPLAYING) && 
             (cur_he.local == GENERATED_EXTERNALLY) && 
             (dupe_he.signature_verified == true))
             {
              is_dupe = true;
              break;
             }
    }
    histfile.close();
    if (is_dupe) return false; // already added and processed, do nothing

    histfile = LittleFS.open(FILENAME_HISTORY, FILE_APPEND);
    if (!histfile) return false;
    histfile.write((uint8_t*) &cur_he, sizeof(history_entry));
    histfile.close();

    mb_check_for_signature(origin, refnr);
    if (cur_he.display == RELEVANT_FOR_DISPLAYING)
    {
        mb_count_and_show_last(); // mb_refresh_display();
        if (is_screensaver_on()) screensaver_off();
        if ((cur_he.crisis) && play_ringtone) audio_play_melody(); // play ringtone for crisis OAs only
    }

    return true;
}

#define CHUNKSIZE 1024
char chunk[CHUNKSIZE];

void mb_refresh_display(void)
{
    if (!hasStorage) return;
    bool last_msg_is_crisis = true;

    char info[INFOLEN];

    snprintf(info, INFOLEN, "WARNING: Obsolete mb_refresh_display() called");
    serial_writeln(info);

    gui_crisis_clear();
    gui_noncrisis_clear();

    int num_entries = histfile_numentries();
    if (num_entries == -1) return;
    int skipfirst = 0;
    if (num_entries > MAX_SHOWN_ENTRIES) skipfirst = MAX_SHOWN_ENTRIES; // maximum number of entries to show

    snprintf(info, INFOLEN, "INFO: Message Board History currently has %d entries", num_entries);
    serial_writeln(info);

    if (histfile)
    {
        serial_writeln("WARNING: Histfile already open");
        histfile.close();
    }
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    if (!histfile) return;

    size_t allocSize = 64 * 1024; // 64 kB ought to be enough for everymessage
    uint8_t* buffer_crisis = (uint8_t*) malloc(allocSize);
    uint8_t* buffer_noncrisis = (uint8_t*) malloc(allocSize);

    if (!buffer_noncrisis)
    {
        Serial.println("ERROR: Message Board RAM allocation failed!");
    }

    buffer_crisis[0] = 0;
    buffer_noncrisis[0] = 0;

    for (int i=0; i<num_entries; i++)
    {
        delay(1);

        if (histfile.read((uint8_t*)&cur_he, sizeof(cur_he)) != sizeof(cur_he))
        {
            histfile.close();
            free(buffer_crisis);
            free(buffer_noncrisis);
            return;
        }

        if (strcmp(cur_he.footer, "RDCP"))
        {
            serial_writeln("ERROR: Bad Message Board history entry, deleting history file");
            histfile.close();
            LittleFS.remove(FILENAME_HISTORY);
            return;
        }

        if (i < skipfirst) continue; // only show the n latest messages for performance and memory reasons
        if (cur_he.display != RELEVANT_FOR_DISPLAYING) continue;

        if (cur_he.timestamp_added_abs > NO_TIMESTAMP)
        {
            struct tm ti;
            if (localtime_r(&cur_he.timestamp_added_abs, &ti))
            {
                snprintf(info, INFOLEN, "\n[Empfangen %02d.%02d.%04d %02d:%02d]\n", ti.tm_mday, ti.tm_mon, ti.tm_year + 1900, ti.tm_hour, ti.tm_min);
            }
        }
        else
        {
            snprintf(info, INFOLEN, "\n");
        }
        if (cur_he.crisis) { strcat((char *)buffer_crisis, info); } else { strcat((char *)buffer_noncrisis, info); }

        if (cur_he.local == GENERATED_LOCALLY)
        {
            if (cur_he.crisis)
            {
                strcat((char *)buffer_crisis, cur_he.content_text);
                strcat((char *)buffer_crisis, "\n");
                last_msg_is_crisis = true;
            }
            else
            {
                strcat((char *)buffer_noncrisis, cur_he.content_text);
                strcat((char *)buffer_noncrisis, "\n");
                last_msg_is_crisis = false;
            }
        }
        else
        { // Handle externally received messages
            if (cur_he.crisis)
            {
                if (cur_he.subtype == RDCP_MSGTYPE_OA_SUBTYPE_CRISIS_TXT)
                    snprintf(info, INFOLEN, "[Nachricht des Krisenstabs, Referenznummer %04X, %s digitale Unterschrift]\n", cur_he.refnr, cur_he.signature_verified ? "hat" : "noch ohne");
                else if (cur_he.subtype == RDCP_MSGTYPE_OA_SUBTYPE_FEEDBACK)
                    snprintf(info, INFOLEN, "[Antwort des Krisenstabs auf Ihre Nachricht %04X-%04X]\n", getMyRDCPAddress(), cur_he.refnr);
                else if (cur_he.subtype == RDCP_MSGTYPE_OA_SUBTYPE_INQUIRY)
                    snprintf(info, INFOLEN, "[Frage des Krisenstabs zu Ihrer Nachricht %04X-%04X]\n", getMyRDCPAddress(), cur_he.refnr);
                else
                    snprintf(info, INFOLEN, "[Nachricht %04X-%04X]\n", cur_he.origin, cur_he.refnr);
                strcat((char *)buffer_crisis, info);
                if (cur_he.morefragments > 0)
                {
                    snprintf(info, INFOLEN, "[Mehrteilig. %d %s.]\n", cur_he.morefragments, cur_he.morefragments > 1 ? "Teile folgen" : "Teil folgt");
                    strcat((char *)buffer_crisis, info);
                }

                strcat((char *)buffer_crisis, cur_he.content_text);
                strcat((char *)buffer_crisis, "\n\n");
                last_msg_is_crisis = true;
            }
            else
            {
                snprintf(info, INFOLEN, "[Nachricht der Gemeinde, Referenznummer %04X, %s digitale Unterschrift]\n", cur_he.refnr, cur_he.signature_verified ? "hat" : "noch ohne");
                strcat((char *)buffer_noncrisis, info);
                if (cur_he.morefragments > 0)
                {
                    snprintf(info, INFOLEN, "[Mehrteilig. %d %s.]\n", cur_he.morefragments, cur_he.morefragments > 1 ? "Teile folgen" : "Teil folgt");
                    strcat((char *)buffer_noncrisis, info);
                }

                strcat((char *)buffer_noncrisis, cur_he.content_text);
                strcat((char *)buffer_noncrisis, "\n\n");
                last_msg_is_crisis = false;
            }

            /* To prevent buffer overruns, we truncate when exceeding a threshold */
            size_t lenc = strlen((const char*)buffer_crisis);
            size_t lennc = strlen((const char*)buffer_noncrisis);
            if (lenc > 60000)
            {
                for (int i=20000; i<65535; i++) buffer_crisis[i-20000] = buffer_crisis[i];
            }
            if (lennc > 60000)
            {
                for (int i=20000; i<65535; i++) buffer_noncrisis[i-20000] = buffer_noncrisis[i];
            }

        }
    }
    histfile.close();

    size_t sc = strlen((const char*)buffer_crisis);
    size_t snc = strlen((const char*)buffer_noncrisis);
    snprintf(info, INFOLEN, "INFO: Message Board updated with %d OAc, %d OAnc, last: %s", sc, snc, last_msg_is_crisis ? "crisis" : "noncrisis");
    serial_writeln(info);

    if (last_msg_is_crisis)
    {
        gui_transition_to_screen(SCREEN_OACRISIS);

        lv_textarea_set_accepted_chars(ui_TextAreaOACrisis, NULL);
        lv_textarea_set_max_length(ui_TextAreaOACrisis, 0);
        lv_textarea_set_text(ui_TextAreaOACrisis, (const char*) buffer_crisis);
        lv_textarea_set_accepted_chars(ui_TextAreaOACrisis, "*");

        lv_textarea_set_cursor_pos(ui_TextAreaOACrisis, LV_TEXTAREA_CURSOR_LAST);
        lv_textarea_add_text(ui_TextAreaOACrisis, "\n"); // along with a linebreak
    }
    else
    {
        gui_transition_to_screen(SCREEN_OANONCRISIS);

        lv_textarea_set_accepted_chars(ui_TextAreaOANonCrisis, NULL);
        lv_textarea_set_max_length(ui_TextAreaOANonCrisis, 0);
        lv_textarea_set_text(ui_TextAreaOANonCrisis, (const char*) buffer_noncrisis);
        lv_textarea_set_accepted_chars(ui_TextAreaOANonCrisis, "*");

        lv_textarea_set_cursor_pos(ui_TextAreaOANonCrisis, LV_TEXTAREA_CURSOR_LAST);
        lv_textarea_add_text(ui_TextAreaOANonCrisis, "\n"); // along with a linebreak
    }

    screensaver_off(); // Activate display so users can see that something has changed

    free(buffer_crisis);
    free(buffer_noncrisis);

    return;
}

void mb_serial_show_messages(void)
{
    if (!hasStorage) return;
    char info[INFOLEN];

    int num_entries = histfile_numentries();
    if (num_entries == -1) return;
    int skipfirst = 0;
    if (num_entries > MAX_SHOWN_ENTRIES) skipfirst = MAX_SHOWN_ENTRIES; // maximum number of entries to show

    snprintf(info, INFOLEN, "INFO: Message Board History currently has %d entries", num_entries);
    serial_writeln(info);

    if (histfile)
    {
        serial_writeln("WARNING: Histfile already open");
        histfile.close();
    }
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    if (!histfile) return;

    for (int i=0; i<num_entries; i++)
    {
        if (histfile.read((uint8_t*)&cur_he, sizeof(cur_he)) != sizeof(cur_he))
        {
            histfile.close();
            return;
        }
        if (strcmp(cur_he.footer, "RDCP"))
        {
            serial_writeln("ERROR: Bad Message Board history entry, deleting history file");
            histfile.close();
            LittleFS.remove(FILENAME_HISTORY);
            return;
        }

        if (i < skipfirst) continue; // only show the n latest messages for performance and memory reasons
        if (cur_he.display != RELEVANT_FOR_DISPLAYING) continue;

        if (cur_he.timestamp_added_abs > 0)
        {
            struct tm ti;
            if (localtime_r(&cur_he.timestamp_added_abs, &ti))
            {
                snprintf(info, INFOLEN, "INFO: MBTIMESTAMP %d %02d.%02d.%04d %02d:%02d", i, ti.tm_mday, ti.tm_mon, ti.tm_year + 1900, ti.tm_hour, ti.tm_min);
                serial_writeln(info);
            }
        }

        if (cur_he.local == GENERATED_LOCALLY)
        {
            snprintf(info, INFOLEN, "INFO: MBENTRY %d (local) - %s", i, cur_he.content_text);
            serial_writeln(info);
        }
        else
        { // Handle externally received messages
            snprintf(info, INFOLEN, "INFO: MBENTRY %d (remote) - R%04X - %s", i, cur_he.refnr, cur_he.content_text);
            serial_writeln(info);
        }
    }
    histfile.close();

    return;
}

/**
 * OBSOLETE
 * Re-write the complete MB history file and make a change to the entry identified
 * by changeidx. If 'deletion' is set to true, the entry is removed. Otherwise, it
 * is replaced by cur_he.
 */
void mb_rewrite_history_obs(int changeidx, bool deletion)
{
    history_entry he;

    if (histfile)
    {
        serial_writeln("WARNING: Histfile already open");
        histfile.close();
    }

    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ); if (!histfile) return;
    File nf = LittleFS.open(FILENAME_HISTTMP, FILE_WRITE); if (!nf) { histfile.close(); return; }

    int i = 0;
    while (histfile.read((uint8_t*)&he, sizeof(history_entry)) == sizeof(history_entry))
    {
        if (i != changeidx)
        {
            nf.write((uint8_t*)&he, sizeof(history_entry));
        }
        else
        { // do nothing if we delete
            if (deletion == false) nf.write((uint8_t*)&cur_he, sizeof(history_entry));
        }
        i++;
    }

    histfile.close(); nf.close();
    LittleFS.remove(FILENAME_HISTORY);
    LittleFS.rename(FILENAME_HISTTMP, FILENAME_HISTORY);

    char info[INFOLEN];
    snprintf(info, INFOLEN, "INFO: MB History re-written, %d entries, %s entry %d", i, deletion ? "deleted" : "changed", changeidx);
    serial_writeln(info);

    return;
}

void mb_add_signature(uint8_t *signature, uint16_t origin, uint16_t refnr)
{
    if (!hasStorage) return;

    if (refnr > highest_oa_refnr) highest_oa_refnr = refnr;

    if (mb_refnr_is_blocked(refnr))
    { 
        serial_writeln("INFO: Message board refuses to add Signature with blocked reference number");  
        return;
    }

    cur_he.local = GENERATED_EXTERNALLY;
    cur_he.display = NOT_RELEVANT_FOR_DISPLAYING;
    cur_he.forall = RELEVANCE_FOR_EVERYONE;
    cur_he.crisis = false;
    cur_he.origin = origin;
    cur_he.seqnr = RDCP_NO_SEQUENCE_NUMBER;
    cur_he.refnr = refnr;
    cur_he.morefragments = 0;
    for (int i=0; i<65; i++) cur_he.content_text[i] = signature[i];
    cur_he.content_text[65] = 0;
    snprintf(cur_he.content_rdcp, CONTENT_RDCP_SIZE, "%s", get_current_rdcp_msg_base64());
    cur_he.signature_available = true;
    cur_he.signature_verified = false;
    cur_he.timestamp_added = my_millis();
    cur_he.lifetime = 60002;
    cur_he.subtype = 0;
    time_t absexp = tdeck_get_time();
    cur_he.timestamp_added_abs = absexp;
    cur_he.expiry_absolute = NO_TIMESTAMP;
    snprintf(cur_he.footer, 8, "RDCP\0");

    if (histfile)
    {
        serial_writeln("WARNING: Histfile already open");
        histfile.close();
    }

    // check for duplicate entries
    bool is_dupe = false;
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    while (histfile.read((uint8_t*)&dupe_he, sizeof(history_entry)) == sizeof(history_entry))
    {
        if ( (dupe_he.origin   == cur_he.origin)    &&
             (dupe_he.lifetime == cur_he.lifetime)  &&
             (dupe_he.refnr    == cur_he.refnr)     &&
             (dupe_he.subtype  == cur_he.subtype))
             {
              is_dupe = true;
              break;
             }
    }
    histfile.close();
    if (is_dupe) return; // already added and processed, do nothing

    histfile = LittleFS.open(FILENAME_HISTORY, FILE_APPEND);
    if (!histfile) return; 
    histfile.write((uint8_t*) &cur_he, sizeof(history_entry));
    histfile.close();

    mb_check_for_signature(origin, refnr);
    return;
}

bool mb_check_lifetime(void)
{
    if (!hasStorage) return false;

    bool needs_repeat = true;
    bool deleted_something = false;
    history_entry he;

    time_t now_abs = tdeck_get_time();
    int64_t now = my_millis();

    if (histfile)
    {
        serial_writeln("WARNING: mb_chk_lt History file already open");
        histfile.close();
    }
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    if (!histfile) return false;

    File nf = LittleFS.open(FILENAME_HISTTMP, FILE_WRITE);
    if (!nf)
    {
        histfile.close();
        return false;
    }

    int i = 0;

    while (histfile.read((uint8_t*)&he, sizeof(history_entry)) == sizeof(history_entry))
    {
        if (strcmp(he.footer, "RDCP"))
        {
            serial_writeln("ERROR: Bad MB history file entry, deleting history file");
            histfile.close();
            nf.close();
            LittleFS.remove(FILENAME_HISTORY);
            LittleFS.remove(FILENAME_HISTTMP);
            mb_zap_counters();
            return true;
        }
    
        bool has_expired = false;

        if (he.lifetime == 65535)
        {
            nf.write((uint8_t*)&he, sizeof(history_entry));
            if ((he.local == GENERATED_EXTERNALLY) && (he.refnr > highest_oa_refnr)) highest_oa_refnr = he.refnr;
            continue; // 65535 is magic value for infinite lifetime
        }

        if (now_abs > NO_TIMESTAMP)
        { // Device currently has wallclock time available
            if ((he.expiry_absolute > NO_TIMESTAMP) && (now_abs > he.expiry_absolute)) has_expired = true;
        }

        if ((now_abs == NO_TIMESTAMP) || (he.expiry_absolute == NO_TIMESTAMP))
        {   // We need to work with a monotonic clock only
            // If the entry was added "in the future", assume that the device had a power cycle
            // and treat the entry as if it was added at power-on.
            int64_t basetime = he.timestamp_added;
            if (basetime > now) basetime = NO_TIMESTAMP;

            // Device timestamps are in milliseconds, so convert lifetime to milliseconds
            if (now > (basetime + SECONDS_TO_MILLISECONDS * lifetime_to_seconds(he.lifetime))) has_expired = true;
        }

        if (!has_expired)
        {
            nf.write((uint8_t*)&he, sizeof(history_entry));
            if ((he.local == GENERATED_EXTERNALLY) && (he.refnr > highest_oa_refnr)) highest_oa_refnr = he.refnr;
        }
        else
        {
            deleted_something = true;
            char info[INFOLEN];
            snprintf(info, INFOLEN, "INFO: Dropped MB entry o%04X r%04X - expired, lifetime %d", he.origin, he.refnr, he.lifetime);
            serial_writeln(info);
        }
        i++;
    }

    histfile.close();
    nf.close();
    LittleFS.remove(FILENAME_HISTORY);
    LittleFS.rename(FILENAME_HISTTMP, FILENAME_HISTORY);

    return deleted_something;
}

void mb_check_lifetime_and_update_display(bool check, bool update_only_if_neccessary)
{
    bool necessary = false;
    if (check) necessary = mb_check_lifetime();
    if (!update_only_if_neccessary)
    {
        mb_count_and_show_last();
    }
    else
    {
        if (necessary) mb_count_and_show_last();
    }
    return;
}

void mb_update_lifetime(uint16_t origin, uint16_t refnr, uint16_t lifetime)
{
    if (!hasStorage) return;

    bool needs_repeat = true;
    bool deleted_something = false;
    history_entry he;

    time_t now_abs = tdeck_get_time();
    int64_t now = my_millis();

    if (histfile) histfile.close();
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    if (!histfile) return;

    File nf = LittleFS.open(FILENAME_HISTTMP, FILE_WRITE);
    if (!nf)
    {
        histfile.close();
        return;
    }

    int i = 0;

    if (lifetime == NO_DURATION) mb_refnr_add_blocked(refnr); // track deleted OAs

    while (histfile.read((uint8_t*)&he, sizeof(history_entry)) == sizeof(history_entry))
    {
        if (strcmp(he.footer, "RDCP"))
        {
            serial_writeln("ERROR: Bad MB history file entry, deleting history file");
            histfile.close();
            nf.close();
            LittleFS.remove(FILENAME_HISTORY);
            LittleFS.remove(FILENAME_HISTTMP);
            mb_zap_counters();
            return;
        }

        bool delete_this = false;

        if ((origin == he.origin) && (refnr == he.refnr))
        { // found an entry that needs to be updated
            if (lifetime == NO_DURATION)
            { // delete this entry because new lifetime is 0
                deleted_something = true;
                delete_this = true;
            }

            time_t absexp = tdeck_get_time();
            he.timestamp_added_abs = absexp;
            if (absexp > NO_TIMESTAMP)
            {
                absexp += lifetime_to_seconds(lifetime);
                he.expiry_absolute = absexp;
            }
            else
            {
               he.expiry_absolute = NO_TIMESTAMP;
            }
            he.lifetime = lifetime;
            he.timestamp_added = my_millis();

            if (!delete_this) 
            { 
                nf.write((uint8_t*)&he, sizeof(history_entry));
                if ((he.local == GENERATED_EXTERNALLY) && (he.refnr > highest_oa_refnr)) highest_oa_refnr = he.refnr;
            }
            else 
            {
                char info[INFOLEN];
                snprintf(info, INFOLEN, "INFO: Dropped MB entry o%04X r%04X - lifetime set to zero", he.origin, he.refnr);
                serial_writeln(info);
            }
        }
        else 
        { // entry does not need update
            nf.write((uint8_t*)&he, sizeof(history_entry));
            if ((he.local == GENERATED_EXTERNALLY) && (he.refnr > highest_oa_refnr)) highest_oa_refnr = he.refnr;
        }
        i++;
    }

    histfile.close();
    nf.close();
    LittleFS.remove(FILENAME_HISTORY);
    LittleFS.rename(FILENAME_HISTTMP, FILENAME_HISTORY);
    if (deleted_something)
    {
        mb_zap_counters();
        mb_count_and_show_last(true, infrastructure_in_crisis);
    } 
    return;
}

void mb_load_entry(int num)
{
    if (!hasStorage) return;
    if (histfile) histfile.close();
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    if (!histfile) return;

    for (int i=0; i<num; i++)
    {
        histfile.read((uint8_t*)&cur_he, sizeof(cur_he));
    }
    histfile.close();

    return;
}

void mb_count_and_show_last(bool forced_mode, bool crisis)
{
    mb_count();
    if ((msg_crisis_total == 0) && (msg_noncrisis_total == 0))
    {
        cur_he.crisis = true;
        if (forced_mode && (crisis==false)) cur_he.crisis = false;
    }
    else
    {
        if (forced_mode) // forced mode
        {
            if (crisis)
            { // forced crisis
                mb_load_entry(msg_crisis_last);
                cur_he.crisis = true;
                msg_crisis_current = msg_crisis_total;
            }
            else // forced non-crisis
            {
                mb_load_entry(msg_noncrisis_last);
                cur_he.crisis = false;
                msg_noncrisis_current = msg_noncrisis_total;
            }
        }
        else // non-forced mode
        {
            if (msg_crisis_last > msg_noncrisis_last) // most recent OA is crisis
            {
                mb_load_entry(msg_crisis_last);
                cur_he.crisis = true;
                msg_crisis_current = msg_crisis_total;
            }
            else // most recent OA is non-crisis 
            {
                mb_load_entry(msg_noncrisis_last);
                cur_he.crisis = false;
                msg_noncrisis_current = msg_noncrisis_total;
            }
        }
    }
    mb_show_current(-1);
    return;
}

void mb_count(void)
{
    msg_crisis_total = 0;
    msg_noncrisis_total = 0;
    int msg_ignored = 0;
    int curpos = 0;
    msg_crisis_last = 0;
    msg_noncrisis_last = 0;

    if (!hasStorage) return;
    if (histfile) histfile.close();
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    if (!histfile) return;

    while (true)
    {
        if (histfile.read((uint8_t*)&cur_he, sizeof(cur_he)) != sizeof(cur_he)) break;
        curpos++;
        if (strcmp(cur_he.footer, "RDCP"))
        {
            serial_writeln("ERROR: Bad Message Board history entry, deleting history file");
            histfile.close();
            LittleFS.remove(FILENAME_HISTORY);
            msg_crisis_total = 0; msg_noncrisis_total = 0;
            msg_crisis_last = 0; msg_noncrisis_last = 0;
            msg_crisis_current = 0; msg_noncrisis_current = 0;
            return;
        }
        if ((cur_he.display == NOT_RELEVANT_FOR_DISPLAYING) &&
            (cur_he.seqnr == RDCP_NO_SEQUENCE_NUMBER) && (cur_he.local == GENERATED_EXTERNALLY))
            { // cryptographic signature
                msg_ignored += 1;
                continue;
            }
        if (cur_he.crisis) 
        { 
            msg_crisis_total += 1; 
            msg_crisis_last = curpos;
            if ((cur_he.local == GENERATED_EXTERNALLY) && (cur_he.refnr > highest_oa_refnr)) highest_oa_refnr = cur_he.refnr;
        }
        if (!cur_he.crisis) 
        { 
            msg_noncrisis_total += 1; 
            msg_noncrisis_last = curpos; 
            if ((cur_he.local == GENERATED_EXTERNALLY) && (cur_he.refnr > highest_oa_refnr)) highest_oa_refnr = cur_he.refnr;
        }
    }
    histfile.close();

    char info[INFOLEN];
    snprintf(info, INFOLEN, "INFO: MB count yields %d crisis, %d non-crisis, %d ignored, last: %s",
        msg_crisis_total, msg_noncrisis_total, msg_ignored, msg_crisis_last > msg_noncrisis_last ? "crisis" : "non-crisis");
    serial_writeln(info);

    return;
}

void mb_show_by_number(int msgno, bool crisis)
{
    if (!hasStorage) return;
    if (histfile) histfile.close();
    histfile = LittleFS.open(FILENAME_HISTORY, FILE_READ);
    if (!histfile) return;

    int counter = 0;

    while (true)
    {
        if (histfile.read((uint8_t*)&cur_he, sizeof(cur_he)) != sizeof(cur_he)) break;
        if (strcmp(cur_he.footer, "RDCP"))
        {
            serial_writeln("ERROR: Bad Message Board history entry, deleting history file");
            histfile.close();
            LittleFS.remove(FILENAME_HISTORY);
            msg_crisis_total = 0; msg_noncrisis_total = 0;
            msg_crisis_last = 0; msg_noncrisis_last = 0;
            msg_crisis_current = 0; msg_noncrisis_current = 0;
            return;
        }
        if ((cur_he.display == NOT_RELEVANT_FOR_DISPLAYING) &&
            (cur_he.seqnr == RDCP_NO_SEQUENCE_NUMBER) && (cur_he.local == GENERATED_EXTERNALLY)) continue;
        if (cur_he.crisis == crisis)
        {
            counter++;
            if (counter == msgno) break; // found the message to display
        }
    }

    histfile.close();
    mb_show_current(msgno);

    return;
}

char textblob[8192];

void mb_show_current(int as_number)
{
    char textfield[MINIBUFLEN], info[INFOLEN];
    if (cur_he.crisis)
    {
        if (gui_get_current_screen() != SCREEN_OACRISIS)
        {
            /* Do not interrupt user interaction on CIRE-related UI screens */
            if (
                (gui_get_current_screen() != SCREEN_EMERGENCY) &&
                (gui_get_current_screen() != SCREEN_EMERSTEPTWO) &&
                (gui_get_current_screen() != SCREEN_CIRE) &&
                (gui_get_current_screen() != SCREEN_RESP) &&
                (gui_get_current_screen() != SCREEN_EULA)
               )
            {
              serial_writeln("INFO: Switching to OA Crisis UI screen to show current message");
              gui_transition_to_screen(SCREEN_OACRISIS);
            }
        }

        lv_obj_clear_state(ui_ButtonOACNav1, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_ButtonOACNav2, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_ButtonOACNav3, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_ButtonOACNav4, LV_STATE_DISABLED);
        lv_obj_set_style_bg_color(ui_ButtonOACNav1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonOACNav2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonOACNav3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonOACNav4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

        if (msg_crisis_total <= 1)
        {
            snprintf(textfield, MINIBUFLEN, "0 von 0");
            lv_obj_set_style_bg_color(ui_ButtonOACNav1, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ButtonOACNav2, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ButtonOACNav3, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ButtonOACNav4, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_state(ui_ButtonOACNav1, LV_STATE_DISABLED);
            lv_obj_add_state(ui_ButtonOACNav2, LV_STATE_DISABLED);
            lv_obj_add_state(ui_ButtonOACNav3, LV_STATE_DISABLED);
            lv_obj_add_state(ui_ButtonOACNav4, LV_STATE_DISABLED);
            lv_textarea_set_text(ui_TextAreaOACrisis, "");
            lv_label_set_text(ui_OACTextNum, textfield);
            if (msg_crisis_total == 0) return;
        }

        if (as_number == 1)
        { // Disable backward buttons
            lv_obj_set_style_bg_color(ui_ButtonOACNav1, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ButtonOACNav2, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_state(ui_ButtonOACNav1, LV_STATE_DISABLED);
            lv_obj_add_state(ui_ButtonOACNav2, LV_STATE_DISABLED);
        }

        if ((as_number >= 1) && (as_number < msg_crisis_total))
        {
            snprintf(textfield, MINIBUFLEN, "%d von %d", as_number, msg_crisis_total);
        }

        if ((as_number == -1) || (as_number == msg_crisis_total))
        { // Disable forward buttons
            msg_crisis_current = msg_crisis_total;
            snprintf(textfield, MINIBUFLEN, "%d von %d", msg_crisis_total, msg_crisis_total);
            lv_obj_set_style_bg_color(ui_ButtonOACNav3, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ButtonOACNav4, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_state(ui_ButtonOACNav3, LV_STATE_DISABLED);
            lv_obj_add_state(ui_ButtonOACNav4, LV_STATE_DISABLED);
        }

        lv_label_set_text(ui_OACTextNum, textfield);

        char textpart[INFOLEN];
        if (cur_he.timestamp_added_abs > 0)
        {
            struct tm ti;
            if (localtime_r(&cur_he.timestamp_added_abs, &ti))
            {
                if (cur_he.local == GENERATED_EXTERNALLY)
                {
                    snprintf(textpart, INFOLEN, "Empfangen %02d.%02d.%04d %02d:%02d\n", ti.tm_mday, ti.tm_mon, ti.tm_year + 1900, ti.tm_hour, ti.tm_min);
                }
                else
                {
                    snprintf(textpart, INFOLEN, "%02d.%02d.%04d %02d:%02d\n", ti.tm_mday, ti.tm_mon, ti.tm_year + 1900, ti.tm_hour, ti.tm_min);
                }
            }
        }
        else
        {
            snprintf(textpart, INFOLEN, "");
        }

        if (cur_he.local == GENERATED_EXTERNALLY)
        {
            if (cur_he.subtype == RDCP_MSGTYPE_OA_SUBTYPE_CRISIS_TXT)
            {
                snprintf(textblob, 8192, "%sNachricht des Krisenstabs Nr. %04X\n(%s digitale Unterschrift)\n", textpart, cur_he.refnr, cur_he.signature_verified ? "Geprüfte": "Noch ohne");
            }
            else if (cur_he.subtype == RDCP_MSGTYPE_OA_SUBTYPE_FEEDBACK)
            {
                snprintf(textblob, 8192, "%sAntwort des Krisenstabs\nauf Ihre Nachricht %04X-%04X\n", textpart, getMyRDCPAddress(), cur_he.refnr);
            }
            else if (cur_he.subtype == RDCP_MSGTYPE_OA_SUBTYPE_INQUIRY)
            {
                snprintf(textblob, 8192, "%sRückfrage des Krisenstabs\nzu Ihrer Nachricht %04X-%04X\n", textpart, getMyRDCPAddress(), cur_he.refnr);
            }

            strcat(textblob, "\n");
            strcat(textblob, cur_he.content_text);
            if (cur_he.morefragments)
            {
                snprintf(info, INFOLEN, "\n[Mehrteilig: %d %s]", cur_he.morefragments, cur_he.morefragments > 1 ? "Teile folgen" : "Teil folgt");
                strcat(textblob, info);
            }
        }
        else
        { // local message
            snprintf(textblob, 8192, "%s%s", textpart, cur_he.content_text);
        }

        lv_textarea_set_accepted_chars(ui_TextAreaOACrisis, NULL);
        lv_textarea_set_max_length(ui_TextAreaOACrisis, 0);
        lv_textarea_set_text(ui_TextAreaOACrisis, (const char*) textblob);
        lv_textarea_set_accepted_chars(ui_TextAreaOACrisis, "*");
        lv_textarea_set_cursor_pos(ui_TextAreaOACrisis, 0);
        lv_textarea_cursor_up(ui_TextAreaOACrisis); // ?
    } // show crisis message

    if (!cur_he.crisis)
    { // show non-crisis message
        if ((gui_get_current_screen() != SCREEN_OANONCRISIS) &&
            (gui_get_current_screen() != SCREEN_EULA))
        {
            gui_transition_to_screen(SCREEN_OANONCRISIS);
        }

        lv_obj_clear_state(ui_ButtonOANCNav1, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_ButtonOANCNav2, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_ButtonOANCNav3, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_ButtonOANCNav4, LV_STATE_DISABLED);
        lv_obj_set_style_bg_color(ui_ButtonOANCNav1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonOANCNav2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonOANCNav3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonOANCNav4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

        if (msg_noncrisis_total <= 1)
        {
            snprintf(textfield, MINIBUFLEN, "0 von 0");
            lv_obj_set_style_bg_color(ui_ButtonOANCNav1, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ButtonOANCNav2, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ButtonOANCNav3, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ButtonOANCNav4, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_state(ui_ButtonOANCNav1, LV_STATE_DISABLED);
            lv_obj_add_state(ui_ButtonOANCNav2, LV_STATE_DISABLED);
            lv_obj_add_state(ui_ButtonOANCNav3, LV_STATE_DISABLED);
            lv_obj_add_state(ui_ButtonOANCNav4, LV_STATE_DISABLED);
            lv_textarea_set_text(ui_TextAreaOANonCrisis, "");
            lv_label_set_text(ui_OANCTextNum, textfield);
            if (msg_noncrisis_total == 0) return;
        }

        if (as_number == 1)
        { // Disable backward buttons
            lv_obj_set_style_bg_color(ui_ButtonOANCNav1, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ButtonOANCNav2, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_state(ui_ButtonOANCNav1, LV_STATE_DISABLED);
            lv_obj_add_state(ui_ButtonOANCNav2, LV_STATE_DISABLED);
        }

        if ((as_number >= 1) && (as_number < msg_noncrisis_total))
        {
            snprintf(textfield, MINIBUFLEN, "%d von %d", as_number, msg_noncrisis_total);
        }

        if ((as_number == -1) || (as_number == msg_noncrisis_total))
        { // Disable forward buttons
            msg_noncrisis_current = msg_noncrisis_total;
            snprintf(textfield, MINIBUFLEN, "%d von %d", msg_noncrisis_total, msg_noncrisis_total);
            lv_obj_set_style_bg_color(ui_ButtonOANCNav3, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ButtonOANCNav4, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_state(ui_ButtonOANCNav3, LV_STATE_DISABLED);
            lv_obj_add_state(ui_ButtonOANCNav4, LV_STATE_DISABLED);
        }

        lv_label_set_text(ui_OANCTextNum, textfield);

        char textpart[256];
        if (cur_he.timestamp_added_abs > 0)
        {
            struct tm ti;
            if (localtime_r(&cur_he.timestamp_added_abs, &ti))
            {
                if (cur_he.local == GENERATED_EXTERNALLY)
                {
                    snprintf(textpart, INFOLEN, "Empfangen %02d.%02d.%04d %02d:%02d\n", ti.tm_mday, ti.tm_mon, ti.tm_year + 1900, ti.tm_hour, ti.tm_min);
                }
                else
                {
                    snprintf(textpart, INFOLEN, "%02d.%02d.%04d %02d:%02d\n", ti.tm_mday, ti.tm_mon, ti.tm_year + 1900, ti.tm_hour, ti.tm_min);
                }
            }
        }
        else
        {
            snprintf(textpart, INFOLEN, "");
        }

        if (cur_he.local == GENERATED_EXTERNALLY)
        {

            snprintf(textblob, 8192, "%sNachricht der Gemeinde Nr. %04X\n(%s digitale Unterschrift)\n", textpart, cur_he.refnr, cur_he.signature_verified ? "Geprüfte": "Noch ohne");

            strcat(textblob, "\n");
            strcat(textblob, cur_he.content_text);
            if (cur_he.morefragments)
            {
                snprintf(info, INFOLEN, "\n[Mehrteilig: %d %s]", cur_he.morefragments, cur_he.morefragments > 1 ? "Teile folgen" : "Teil folgt");
                strcat(textblob, info);
            }
        }
        else
        { // local message
            snprintf(textblob, 8192, "%s%s", textpart, cur_he.content_text);
        }

        lv_textarea_set_accepted_chars(ui_TextAreaOANonCrisis, NULL);
        lv_textarea_set_max_length(ui_TextAreaOANonCrisis, 0);
        lv_textarea_set_text(ui_TextAreaOANonCrisis, (const char*) textblob);
        lv_textarea_set_accepted_chars(ui_TextAreaOANonCrisis, "*");
        lv_textarea_set_cursor_pos(ui_TextAreaOANonCrisis, 0);
        lv_textarea_cursor_up(ui_TextAreaOANonCrisis); // ?
    } // show non-crisis message

    set_gui_needs_screen_refresh(true);
    return;
}

void mb_show_crisis_first(void)
{
    if (msg_crisis_total == 0) return;
    if (msg_crisis_current == 1) return;
    msg_crisis_current = 1;
    mb_show_by_number(1, true);
    return;
}

void mb_show_crisis_previous(void)
{
    if (msg_crisis_total == 0) return;
    if (msg_crisis_current <= 1) return;
    msg_crisis_current -= 1;
    mb_show_by_number(msg_crisis_current, true);
    return;
}

void mb_show_crisis_next(void)
{
    if (msg_crisis_total == 0) return;
    if (msg_crisis_current == msg_crisis_total) return;
    msg_crisis_current += 1;
    mb_show_by_number(msg_crisis_current, true);
    return;
}

void mb_show_crisis_last(void)
{
    if (msg_crisis_total == 0) return;
    if (msg_crisis_current == msg_crisis_total) return;
    msg_crisis_current = msg_crisis_total;
    mb_show_by_number(msg_crisis_total, true);
    return;
}

void mb_show_noncrisis_first(void)
{
    if (msg_noncrisis_total == 0) return;
    if (msg_noncrisis_current == 1) return;
    msg_noncrisis_current = 1;
    mb_show_by_number(1, false);
    return;
}

void mb_show_noncrisis_previous(void)
{
    if (msg_noncrisis_total == 0) return;
    if (msg_noncrisis_current <= 1) return;
    msg_noncrisis_current -= 1;
    mb_show_by_number(msg_noncrisis_current, false);
    return;
}

void mb_show_noncrisis_next(void)
{
    if (msg_noncrisis_total == 0) return;
    if (msg_noncrisis_current == msg_noncrisis_total) return;
    msg_noncrisis_current += 1;
    mb_show_by_number(msg_noncrisis_current, false);
    return;
}

void mb_show_noncrisis_last(void)
{
    if (msg_noncrisis_total == 0) return;
    if (msg_noncrisis_current == msg_noncrisis_total) return;
    msg_noncrisis_current = msg_noncrisis_total;
    mb_show_by_number(msg_noncrisis_total, false);
    return;
}

/* EOF */
