#ifndef _ROLORAN_TDECK_HAL
#define _ROLORAN_TDECK_HAL

#include <lvgl.h>

#define TDECK_SERIAL_SPEED 115200

/**
 * Turn the screensaver on
 */
void screensaver_on(void);

/**
 * Turn the screensaver off
 */
void screensaver_off(void);

/**
 * Change the CPU frequency to the maximum (240 MHz)
 */
void cpu_high(void);

/**
 * Change the CPU frequency to the minimum (80 MHz)
 */
void cpu_low(void);

/**
 * The screensaver turns on when there is no activity for the configured delay time.
 * By setting timestamp_last_activity to current my_millis(), the screensaver is deactived.
 * This is done by several functions in this library to handle interaction (touchscreen,
 * trackball, or keyboard key press). It can also be done by application-specific
 * functions, e.g., to make sure that the display turns on automatically when a new
 * message is displayed. Note that the interaction turning off the screensaver may
 * still trigger an (undesired) reaction of the application (such as pressing a "send"
 * button); we try to handle this in the code in this file (quite successfully with the
 * touchscreen; keyboard seems to be interpreted as "enter" key), but this still could
 * be improved. One workaround could be to set up a separate UI screen with no interaction
 * elements to which the screensaver transitions on activation, and we have to remember
 * and go back to the previous UI screen on deactivation. However, we currently get
 * a lot of artifacts on-screen when switching between UI screens too frequently. 
 * Thus, we leave it as it is for now.
 */
void check_screensaver_activation(void);

/**
 * Initialize the T-Deck hardware (except SX1262) and GUI
 */
void tdeck_setup(void);

/**
 * Must be called periodically to update the GUI
 */
void tdeck_loop(void);

/**
 * Prototype of our own keyboard_read function. Used internally.
 */
void my_keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data);

/**
 * Prototype of our own touchpad_read function. Used internally.
 */
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

/**
 * ESP32-based version of millis(). Note that int64_t is used for timestamps across this project. 
 * @return int64_t timestamp, roughly reflecting milliseconds since device power-on given a monotonic clock
 */
int64_t my_millis();

/**
 * Calculate the difference between two millisecond timestamps
 * @param a First timestamp (minuend)
 * @param b Second timestamp (subtrahend)
 * @return Difference between timestamps 
 */
int64_t my_millis_diff(int64_t a, int64_t b);

/**
 * @return Number of milliseconds since screensaver has been turned off
 */
unsigned long tdeck_screensaver_off_since(void);

/**
 * Sets the indicator for LVGL whether the screen should be completely refreshed
 * @param yesno True if screen should be refreshed as soon as possible, false otherwise
 */
void set_gui_needs_screen_refresh(bool yesno);

/**
 * Get current CPU frequency. 
 * @return CPU frequency in MHz (80 or 240)
 */
uint8_t get_cpufreq(void);

/**
 * Is the screensaver currently on? 
 * @return true if screensaver is on, false if screensaver is off
 */
bool is_screensaver_on(void);

/**
 * Completely refresh the display. 
 * Use on post-transmission snow disorder symptoms. 
 */
void gui_handle_PTSD(bool forced);

/**
 * LilyGo T-Deck pinout
 * (note that we use the older edition without integrated GPS)
 */
/* --------------------------------------------------------------------- */
#define TDECK_PERI_POWERON 10
#define TDECK_SPI_MOSI 41
#define TDECK_SPI_MISO 38
#define TDECK_SPI_SCK 40
#define TDECK_SDCARD_CS 39
#define TDECK_TFT_CS 12
#define TDECK_TFT_DC 11
#define TDECK_TFT_BACKLIGHT 42
#define TDECK_RADIO_CS 9
#define TDECK_RADIO_BUSY 13
#define TDECK_RADIO_RST 17
#define TDECK_RADIO_DIO1 45
#define TDECK_I2C_SDA 18
#define TDECK_I2C_SCL 8
#define TDECK_I2C_FREQ 800000UL
#define TDECK_TOUCH_INT 16
#define TDECK_KEYBOARD_INT 46
#define TDECK_KEYBOARD_ADDR 0x55
#define TDECK_TRACKBALL_UP 3
#define TDECK_TRACKBALL_DOWN 15
#define TDECK_TRACKBALL_LEFT 1
#define TDECK_TRACKBALL_RIGHT 2
#define TDECK_TRACKBALL_CLICK 0
#define TDECK_ES7210_MCLK 48
#define TDECK_ES7210_LRCK 21
#define TDECK_ES7210_SCK 47
#define TDECK_ES7210_DIN 14
#define TDECK_I2S_WS 5
#define TDECK_I2S_BCK 7
#define TDECK_I2S_DOUT 6
#define TDECK_BAT_ADC 4
/* --------------------------------------------------------------------- */

#endif

/* EOF */