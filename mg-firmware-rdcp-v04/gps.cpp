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

char gps_position[128];
char gps_timestamp[128];

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

bool setupGPS()
{
    SerialGPS.begin(9600, SERIAL_8N1, BOARD_GPS_RX_PIN, BOARD_GPS_TX_PIN);
    bool result = false;
    uint32_t startTimeout;
    for (int i = 0; i < 3; ++i) 
    {
        SerialGPS.write("$PCAS03,0,0,0,0,0,0,0,0,0,0,,,0,0*02\r\n");
        delay(5);
        startTimeout = my_millis() + 3000;
        while (SerialGPS.available())
        {
            SerialGPS.readString();
            if (millis() > startTimeout) 
            {
                serial_writeln("ERROR: GPS setup NMEA timeout");
                return false;
            }
        };
        SerialGPS.flush();
        delay(200);

        SerialGPS.write("$PCAS06,0*1B\r\n");
        startTimeout = my_millis() + 500;
        String ver = "";
        while (!SerialGPS.available()) 
        {
            if (millis() > startTimeout) 
            {
                serial_writeln("ERROR: GPS setup communication timeout");
                return false;
            }
        }

        SerialGPS.setTimeout(10);
        ver = SerialGPS.readStringUntil('\n');
        if (ver.startsWith("$GPTXT,01,01,02")) 
        {
            serial_writeln("INFO: GPS init succeeded");
            result = true;
            break;
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
    pinMode(BOARD_POWERON, OUTPUT);
    digitalWrite(BOARD_POWERON, HIGH);

    if (!setupGPS()) 
    {
        SerialGPS.begin(38400, SERIAL_8N1, BOARD_GPS_RX_PIN, BOARD_GPS_TX_PIN);
        if (!GPS_Recovery()) 
        {
            SerialGPS.updateBaudRate(9600);
            if (!GPS_Recovery()) 
            {
                while (1) 
                {
                    serial_writeln("ERROR: GPS connect failed, fatal");
                    delay(1000);
                }
            }
            SerialGPS.updateBaudRate(38400);
        }
    }
}

void gps_loop(void)
{
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
    }

    delay(1);
}

int64_t last_gps_info_printed = 0;

void gps_displayInfo(void)
{
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
    bool        ubxFrame = 0;
    uint32_t    startTime = my_millis();
    uint16_t    needRead;

    while (my_millis() - startTime < 800) 
    {
        while (SerialGPS.available()) 
        {
            int c = SerialGPS.read();
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
                if (SerialGPS.readBytes(buffer, needRead) != needRead) 
                {
                    ubxFrameCounter = 0;
                } 
                else 
                {
                    return needRead;
                }
                break;

            default:
                break;
            }
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