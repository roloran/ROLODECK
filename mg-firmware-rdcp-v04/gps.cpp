#include <Arduino.h>
#include "gps.h"
#include "roloran-tdeck-serial.h"       // Serial/UART communication
#include "roloran-tdeck-hal.h"          // T-Deck hardware-related functions
#include "roloran-tdeck-persistence.h"  // File-system / persistence related functions
#include "settings-device.h"            // Device-specific settings

/*
    The code in this file is largely based on LilyGo's GPS Shield example from
        https://github.com/Xinyuan-LilyGO/T-Deck
*/

char gps_position[128] = "No GPS available";
char gps_timestamp[128] = "No GPS available";

#ifndef SerialGPS
#define SerialGPS Serial1
#endif

#define BOARD_GPS_TX_PIN                 43
#define BOARD_GPS_RX_PIN                 44

#define BOARD_POWERON                    10

#include <Arduino.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
static bool GPS_Recovery();
static bool gps_available = false;

static void gps_mark_unavailable(void)
{
    gps_available = false;
    snprintf(gps_position, sizeof(gps_position), "No GPS available");
    snprintf(gps_timestamp, sizeof(gps_timestamp), "No GPS available");
}

static bool gps_timed_out(int64_t start, int64_t timeout_ms)
{
    return my_millis_diff(my_millis(), start) >= timeout_ms;
}

static void gps_begin(uint32_t baudrate)
{
    SerialGPS.begin(baudrate, SERIAL_8N1, BOARD_GPS_RX_PIN, BOARD_GPS_TX_PIN);
    SerialGPS.setTimeout(10);
}

static void gps_update_baudrate(uint32_t baudrate)
{
    SerialGPS.updateBaudRate(baudrate);
    SerialGPS.setTimeout(10);
}

static bool gps_wait_quiet(int64_t max_ms, int64_t quiet_ms)
{
    int64_t start = my_millis();
    int64_t last_byte = start;

    while (!gps_timed_out(start, max_ms))
    {
        bool saw_byte = false;

        while (SerialGPS.available())
        {
            SerialGPS.read();
            saw_byte = true;
            if (gps_timed_out(start, max_ms)) return false;
        }

        if (saw_byte)
        {
            last_byte = my_millis();
        }
        else if (my_millis_diff(my_millis(), last_byte) >= quiet_ms)
        {
            return true;
        }

        delay(1);
    }

    return false;
}

static bool gps_read_line(char *buf, size_t len, int64_t timeout_ms)
{
    size_t pos = 0;
    int64_t start = my_millis();

    if (len == 0) return false;

    while (!gps_timed_out(start, timeout_ms))
    {
        if (!SerialGPS.available())
        {
            delay(1);
            continue;
        }

        int c = SerialGPS.read();
        if (c < 0) continue;

        if (c == '\n')
        {
            buf[pos] = '\0';
            return true;
        }

        if ((c != '\r') && (pos + 1 < len))
        {
            buf[pos++] = (char)c;
        }
    }

    buf[pos] = '\0';
    return false;
}

static bool gps_read_line_with_prefix(const char *prefix, char *buf, size_t len, int64_t timeout_ms, bool *saw_line)
{
    int64_t start = my_millis();
    size_t prefix_len = strlen(prefix);

    if (saw_line) *saw_line = false;

    while (!gps_timed_out(start, timeout_ms))
    {
        int64_t elapsed = my_millis_diff(my_millis(), start);
        int64_t remaining = timeout_ms - elapsed;
        if (remaining <= 0) break;

        if (!gps_read_line(buf, len, remaining)) break;
        if (saw_line) *saw_line = true;
        if (strncmp(buf, prefix, prefix_len) == 0) return true;
    }

    return false;
}

static bool gps_read_payload(uint8_t *target, uint16_t count, int64_t start, int64_t timeout_ms)
{
    uint16_t pos = 0;

    while (pos < count)
    {
        if (gps_timed_out(start, timeout_ms)) return false;

        if (!SerialGPS.available())
        {
            delay(1);
            continue;
        }

        int c = SerialGPS.read();
        if (c >= 0)
        {
            target[pos++] = (uint8_t)c;
        }
    }

    return true;
}

bool setupGPS()
{
    gps_begin(9600);
    bool result = false;
    for (int i = 0; i < 3; ++i) 
    {
        SerialGPS.write("$PCAS03,0,0,0,0,0,0,0,0,0,0,,,0,0*02\r\n");
        delay(5);
        if (!gps_wait_quiet(3000, 100))
        {
            serial_writeln("ERROR: GPS setup NMEA timeout");
            return false;
        }
        SerialGPS.flush();
        delay(200);

        SerialGPS.write("$PCAS06,0*1B\r\n");
        char ver[128];
        bool saw_line = false;
        if (gps_read_line_with_prefix("$GPTXT,01,01,02", ver, sizeof(ver), 500, &saw_line)) 
        {
            serial_writeln("INFO: GPS init succeeded");
            result = true;
            break;
        }
        if (!saw_line)
        {
            serial_writeln("ERROR: GPS setup communication timeout");
            return false;
        }
        delay(500);
    }
    SerialGPS.write("$PCAS04,5*1C\r\n");
    delay(250);
    SerialGPS.write("$PCAS03,1,1,1,1,1,1,1,1,1,1,,,0,0*02\r\n");
    delay(250);
    SerialGPS.write("$PCAS11,3*1E\r\n");
    return result;
}

void gps_setup(void)
{
    gps_mark_unavailable();
    pinMode(BOARD_POWERON, OUTPUT);
    digitalWrite(BOARD_POWERON, HIGH);

    if (!setupGPS()) 
    {
        gps_begin(38400);
        if (!GPS_Recovery()) 
        {
            gps_update_baudrate(9600);
            if (!GPS_Recovery()) 
            {
                serial_writeln("ERROR: GPS connect failed, continuing without GPS");
                gps_mark_unavailable();
                return;
            }
            gps_update_baudrate(38400);
        }
    }

    gps_available = true;
    snprintf(gps_position, sizeof(gps_position), "No GPS position");
    snprintf(gps_timestamp, sizeof(gps_timestamp), "No GPS timestamp");
}

void gps_loop(void)
{
    if (!gps_available) return;

    while (SerialGPS.available()) 
    {
        int c = SerialGPS.read();
        if (gps.encode(c)) 
        {
            gps_displayInfo();
        }
    }

    if (my_millis() > 30000 && gps.charsProcessed() < 10) 
    {
        serial_writeln("ERROR: No GPS detected");
        gps_mark_unavailable();
        return;
    }

    delay(1);
}

int64_t last_gps_info_printed = 0;

void gps_displayInfo(void)
{
    if (!gps_available)
    {
        gps_mark_unavailable();
        return;
    }

    if (gps.location.isValid()) 
    {
        double lat = gps.location.lat();
        double lng = gps.location.lng();
        snprintf(gps_position, 128, "GPS: %.6f,%.6f", lat, lng);
    } 
    else 
    {
        snprintf(gps_position, 128, "No GPS position");
    }

    if (gps.date.isValid() && gps.time.isValid()) 
    {
        snprintf(gps_timestamp, 128, "TS: %02d.%02d.%02d %02d:%02d:%02d UTC",
           (int) gps.date.day(),
           (int) gps.date.month() ,
           (int) gps.date.year(),
           (int) gps.time.hour(),
           (int) gps.time.minute(),
           (int) gps.time.second()
        );
    } 
    else 
    {
        snprintf(gps_timestamp, 128, "No GPS timestamp");
    }

    int64_t now = my_millis();
    if (now - last_gps_info_printed > 5000)
    {
        last_gps_info_printed = now;
        char gps_info[256];
        snprintf(gps_info, 256, "INFO: Current GNSS status is %s, %s", gps_position, gps_timestamp);
        serial_writeln(gps_info);
    }

    return;
}

uint8_t buffer[256];

int gps_getAck(uint8_t *buffer, uint16_t size, uint8_t requestedClass, uint8_t requestedID)
{
    uint16_t    ubxFrameCounter = 0;
    int64_t     startTime = my_millis();
    uint16_t    needRead = 0;

    while (!gps_timed_out(startTime, 800)) 
    {
        if (!SerialGPS.available())
        {
            delay(1);
            continue;
        }

        int c = SerialGPS.read();
        if (c < 0) continue;

        switch (ubxFrameCounter) 
        {
        case 0:
            if (c == 0xB5) 
            {
                ubxFrameCounter++;
            }
            break;
        case 1:
            if (c == 0x62) 
            {
                ubxFrameCounter++;
            }
            else 
            {
                ubxFrameCounter = 0;
            }
            break;
        case 2:
            if (c == requestedClass) 
            {
                ubxFrameCounter++;
            } 
            else 
            {
                ubxFrameCounter = 0;
            }
            break;
        case 3:
            if (c == requestedID) 
            {
                ubxFrameCounter++;
            } 
            else 
            {
                ubxFrameCounter = 0;
            }
            break;
        case 4:
            needRead = c;
            ubxFrameCounter++;
            break;
        case 5:
            needRead |=  (c << 8);
            ubxFrameCounter++;
            break;
        case 6:
            if (needRead >= size) 
            {
                ubxFrameCounter = 0;
                break;
            }
            if (!gps_read_payload(buffer, needRead, startTime, 800)) 
            {
                return 0;
            } 
            return needRead;

        default:
            break;
        }
    }
    return 0;
}

static bool GPS_Recovery(void)
{
    uint8_t cfg_clear1[] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1C, 0xA2};
    uint8_t cfg_clear2[] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1B, 0xA1};
    uint8_t cfg_clear3[] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x03, 0x1D, 0xB3};
    SerialGPS.write(cfg_clear1, sizeof(cfg_clear1));

    if (gps_getAck(buffer, 256, 0x05, 0x01)) 
    {
        serial_writeln("INFO: GPS getAck(1) succeeded");
    }
    SerialGPS.write(cfg_clear2, sizeof(cfg_clear2));
    if (gps_getAck(buffer, 256, 0x05, 0x01)) 
    {
        serial_writeln("INFO: GPS getAck(2) succeeded");
    }
    SerialGPS.write(cfg_clear3, sizeof(cfg_clear3));
    if (gps_getAck(buffer, 256, 0x05, 0x01)) 
    {
        serial_writeln("INFO: GPS getAck(3) succeeded");
    }

    uint8_t cfg_rate[] = {0xB5, 0x62, 0x06, 0x08, 0x00, 0x00, 0x0E, 0x30};
    SerialGPS.write(cfg_rate, sizeof(cfg_rate));
    if (gps_getAck(buffer, 256, 0x06, 0x08)) 
    {
        serial_writeln("INFO: GPS getAck(4) succeeded");
    } 
    else 
    {
        return false;
    }
    return true;
}
/* EOF */
