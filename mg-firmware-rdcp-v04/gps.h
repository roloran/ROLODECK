#ifndef _ROLODECK_GPS
#define _ROLODECK_GPS

// Use GPS at all on ROLODECK Plus?
#define ROLODECK_USE_GPS

// Use joystick button press for quick GPS CIRE sending?
#define ROLODECK_GPS_QUICKCIRE

#include <Arduino.h>

void gps_setup(void);
void gps_loop(void);
void gps_displayInfo(void);
int gps_getAck(uint8_t *buffer, uint16_t size, uint8_t requestedClass, uint8_t requestedID);
static bool GPS_Recovery(void);

#endif