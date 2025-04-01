#ifndef _TDECK_GUI
#define _TDECK_GUI

#include <Arduino.h>

#define SCREEN_PREVIOUS     0 //< screenId for "go to previous screen"
#define SCREEN_SPLASH       1 //< screenId for Splash screen
#define SCREEN_OANONCRISIS  2 //< screenId for Official Announcements in Non-Crisis mode screen
#define SCREEN_OACRISIS     3 //< screenId for Official Announcements in Crisis mode screen
#define SCREEN_EMERGENCY    4 //< screenId for Emergency screen
#define SCREEN_CIRE         5 //< screenId for Citizen Request screen
#define SCREEN_MAINTENANCE  6 //< screenId for Maintenance Notice screen
#define SCREEN_PROVISIONING 7 //< screenId for Provisioning Notice screen
#define SCREEN_RESP         8 //< screenId for Response to Inquiry screen
#define SCREEN_EMERSTEPTWO  9 //< screenId for second step of Emergency report
#define SCREEN_EULA        10 //< screenId for Warning about how to use this device

#define RED_BUTTON_MODE_EMERGENCY 0 //< Left button on OACRISIS Screen is used for emergencies
#define RED_BUTTON_MODE_INQUIRY   1 //< Left button on OACRISIS Screen is used for inquiries

/**
 * Switch the T-Deck GUI to the screen identified by its screenId
 * @param screenID UI screen to switch to 
 */
void gui_transition_to_screen(uint8_t screenId);

/**
 * Set up callback functions on GUI events that are not handled
 * directly by the code exported from SquareLine Studio.
 */
void gui_callback_setup(void);

/**
 * @return UI screen id currently shown 
 */
uint8_t gui_get_current_screen(void);

/**
 * @param s Text to add to the crisis OA screen 
 */
void gui_crisis_add_text(char *s);

/**
 * @param s Text to add to the non-crisis OA screen 
 */
void gui_noncrisis_add_text(char *s);

/**
 * Re-enable the CIRE GUI buttons (e.g., after receiving a CIRE ACK)
 */
void gui_enable_cire_buttons(void);

/**
 * Disable the CIRE GUI buttons (e.g., when waiting for a CIRE ACK)
 */
void gui_disable_cire_buttons(void);

/**
 * Clear all text in the crisis OA screen 
 */
void gui_crisis_clear(void);

/**
 * Clear all text in the non-crisis OA screen
 */
void gui_noncrisis_clear(void);

/**
 * @param value true if the device should be put into crisis mode, false for non-crisis mode 
 */
void set_infrastructure_in_crisis(bool value);

/**
 * @param new_mode RED_BUTTON_MODE_EMERGENCY or RED_BUTTON_MODE_INQURIY
 */
void gui_switch_red_button_mode(uint8_t new_mode);

#endif

/* EOF */