/*
 * roloran-tdeck-hal.c
 */

/* -------------------------------------------------------------------------- */

#include "roloran-tdeck-hal.h"
#include "roloran-tdeck-serial.h"
#include "settings-device.h"

/* -------------------------------------------------------------------------- */

#include <lvgl.h>
#define GFX_DEV_DEVICE LILYGO_T_DECK_DEBUG
#include <Arduino_GFX_Library.h>
#include <SPI.h>
#include <Wire.h>

#include "ui.h"
#include "tdeck_gui.h"
#include "roloran-lora.h"

#define KEY_SCAN_MS_INTERVAL 200

/* -------------------------------------------------------------------------- */

#define GFX_DEV_DEVICE LILYGO_T_DECK
#define GFX_EXTRA_PRE_INIT()                \
  {                                         \
    pinMode(TDECK_SDCARD_CS, OUTPUT);       \
    digitalWrite(TDECK_SDCARD_CS, HIGH);    \
    pinMode(TDECK_RADIO_CS, OUTPUT);        \
    digitalWrite(TDECK_RADIO_CS, HIGH);     \
    pinMode(TDECK_PERI_POWERON, OUTPUT);    \
    digitalWrite(TDECK_PERI_POWERON, HIGH); \
    pinMode(TDECK_TFT_CS, OUTPUT);          \
    digitalWrite(TDECK_TFT_CS, HIGH);       \
    delay(500);                             \
  }
#define GFX_BL TDECK_TFT_BACKLIGHT
Arduino_DataBus *bus = new Arduino_ESP32SPI(TDECK_TFT_DC, TDECK_TFT_CS, TDECK_SPI_SCK, TDECK_SPI_MOSI, TDECK_SPI_MISO); // breaks sx , 1, false);
Arduino_GFX *gfx     = new Arduino_ST7789(bus, GFX_NOT_DEFINED /* RST */, 1 /* rotation */, false /* IPS */);

/* -------------------------------------------------------------------------- */

int64_t timestamp_last_activity   = my_millis();
int64_t timestamp_screensaver_off = my_millis();
bool    tdeck_screensaver_is_on   = false;
bool    tdeck_display_is_on       = true;
bool    tdeck_has_fatfs           = true;
bool    gui_need_screen_refresh   = false;
bool    we_have_the_lock          = false;

SemaphoreHandle_t highlander = NULL;
StaticSemaphore_t highlanderBuffer;

bool is_screensaver_on(void) { return tdeck_screensaver_is_on; }

void set_gui_needs_screen_refresh(bool yesno)
{
  gui_need_screen_refresh = yesno;
  return;
}

int64_t my_millis()
{
  return esp_timer_get_time() / 1000;
}

int64_t my_millis_diff(int64_t a, int64_t b)
{
  return a - b;
}

unsigned long tdeck_screensaver_off_since(void)
{
  return my_millis_diff(my_millis(), timestamp_screensaver_off);
}

void screensaver_on(void)
{
  if (tdeck_screensaver_is_on) return;
  digitalWrite(TDECK_TFT_BACKLIGHT, LOW);
  gfx->displayOff();
  tdeck_display_is_on = false;
  tdeck_screensaver_is_on = true;
  return;
}

void screensaver_off(void)
{
  if (tdeck_screensaver_is_on == false) return;
  cpu_high();
  digitalWrite(TDECK_TFT_BACKLIGHT, HIGH);
  gfx->displayOn();
  tdeck_display_is_on = true;
  tdeck_screensaver_is_on = false;
  timestamp_screensaver_off = my_millis();
  delay(5);
  digitalWrite(TDECK_TFT_BACKLIGHT, HIGH); /* Turn it on twice because this seems buggy. */
  gfx->displayOn();
  timestamp_last_activity = my_millis();
  tdeck_loop();
  set_gui_needs_screen_refresh(true);
  gui_handle_PTSD(true);
  return;
}

uint8_t cpufreq = 240;

uint8_t get_cpufreq(void) { return cpufreq; }

void cpu_high(void)
{
  setCpuFrequencyMhz(240);
  cpufreq = 240;
  return;
}

void cpu_low(void)
{
  cpufreq = 80;
  setCpuFrequencyMhz(80);
  return;
}

void check_screensaver_activation(void)
{
  if (getScreensaverDelay() == 0) return;
  if (tdeck_display_is_on)
  {
    if (my_millis_diff(my_millis(), timestamp_last_activity) > getScreensaverDelay()) {
      screensaver_on();
      cpu_low();
    }
  }
  else
  {
    if (my_millis_diff(millis(), timestamp_last_activity) <= getScreensaverDelay()) {
      cpu_high();
      screensaver_off();
    }
  }
  return;
}

/* -------------------------------------------------------------------------- */

#define TOUCH_MODULES_GT911
#define TOUCH_MODULE_ADDR GT911_SLAVE_ADDRESS1
#define TOUCH_SCL 8
#define TOUCH_SDA 18
#define TOUCH_RES -1
#define TOUCH_INT 16

#include <TouchLib.h>

bool touch_swap_xy = false;
int16_t touch_map_x1 = -1;
int16_t touch_map_x2 = -1;
int16_t touch_map_y1 = -1;
int16_t touch_map_y2 = -1;

int16_t touch_max_x = 0, touch_max_y = 0;
int16_t touch_raw_x = 0, touch_raw_y = 0;
int16_t touch_last_x = 0, touch_last_y = 0;

TouchLib touch(Wire, TOUCH_SDA, TOUCH_SCL, TOUCH_MODULE_ADDR);

void touch_init(int16_t w, int16_t h, uint8_t r)
{
  touch_max_x = w - 1;
  touch_max_y = h - 1;
  if (touch_map_x1 == -1)
  {
    switch (r)
    {
    case 3:
      touch_swap_xy = true;
      touch_map_x1 = touch_max_x;
      touch_map_x2 = 0;
      touch_map_y1 = 0;
      touch_map_y2 = touch_max_y;
      break;
    case 2:
      touch_swap_xy = false;
      touch_map_x1 = touch_max_x;
      touch_map_x2 = 0;
      touch_map_y1 = touch_max_y;
      touch_map_y2 = 0;
      break;
    case 1:
      touch_swap_xy = true;
      touch_map_x1 = 0;
      touch_map_x2 = touch_max_x;
      touch_map_y1 = touch_max_y;
      touch_map_y2 = 0;
      break;
    default: // case 0:
      touch_swap_xy = false;
      touch_map_x1 = 0;
      touch_map_x2 = touch_max_x;
      touch_map_y1 = 0;
      touch_map_y2 = touch_max_y;
      break;
    }
  }
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  delay(10);
  touch.init();
  delay(10);
}

void translate_touch_raw()
{
  if (touch_swap_xy)
  {
    touch_last_x = map(touch_raw_y, touch_map_x1, touch_map_x2, 0, touch_max_x);
    touch_last_y = map(touch_raw_x, touch_map_y1, touch_map_y2, 0, touch_max_y);
  }
  else
  {
    touch_last_x = map(touch_raw_x, touch_map_x1, touch_map_x2, 0, touch_max_x);
    touch_last_y = map(touch_raw_y, touch_map_y1, touch_map_y2, 0, touch_max_y);
  }
}

bool touch_touched()
{
  if (touch.read())
  {
    TP_Point t = touch.getPoint(0);
    touch_raw_x = t.x;
    touch_raw_y = t.y;

    touch_last_x = touch_raw_x;
    touch_last_y = touch_raw_y;

    translate_touch_raw();
    return true;
  }

  return false;
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_touched())
  {
    if (tdeck_display_is_on && tdeck_screensaver_off_since() > 2000)
    {
      data->state = LV_INDEV_STATE_PR;
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    }
    else /* if back from screensaver, don't accidentally press a button */
    {
      data->state = LV_INDEV_STATE_PR;
      data->point.x = 0;
      data->point.y = 0;
    }
    timestamp_last_activity = my_millis();
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

/* -------------------------------------------------------------------------- */

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  if (highlander) xSemaphoreTake(highlander, portMAX_DELAY);
  digitalWrite(TDECK_RADIO_CS, HIGH);
  digitalWrite(TDECK_TFT_CS, LOW);
  delay(1);
#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif
  digitalWrite(TDECK_TFT_CS, HIGH);
  delay(1);
  lv_disp_flush_ready(disp);
  if (highlander) xSemaphoreGive(highlander);
  return;
}

/* -------------------------------------------------------------------------- */

static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf, *disp_draw_buf2;
static lv_disp_drv_t disp_drv;

bool keyboard_interrupted = false;

int64_t next_key_scan_ms = 0;
bool trackball_interrupted = false;
int16_t trackball_up_count = 0;
int16_t trackball_down_count = 0;
int16_t trackball_left_count = 0;
int16_t trackball_right_count = 0;
int16_t trackball_click_count = 0;

void IRAM_ATTR ISR_key()
{
  keyboard_interrupted = true;
  timestamp_last_activity = my_millis();
}

void IRAM_ATTR ISR_up()
{
  trackball_interrupted = true;
  ++trackball_up_count;
  timestamp_last_activity = my_millis();
}

void IRAM_ATTR ISR_down()
{
  trackball_interrupted = true;
  ++trackball_down_count;
  timestamp_last_activity = my_millis();
}

void IRAM_ATTR ISR_left()
{
  trackball_interrupted = true;
  ++trackball_left_count;
  timestamp_last_activity = my_millis();
}

void IRAM_ATTR ISR_right()
{
  trackball_interrupted = true;
  ++trackball_right_count;
  timestamp_last_activity = my_millis();
}

void IRAM_ATTR ISR_click()
{
  trackball_interrupted = true;
  ++trackball_click_count;
  timestamp_last_activity = my_millis();
  gui_need_screen_refresh = true;
}

/* -------------------------------------------------------------------------- */

void my_keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
  static uint32_t last_key = 0;
  if (last_key)
  {
    if (tdeck_display_is_on && tdeck_screensaver_off_since() > 2000)
    {
      data->key = last_key;
      data->state = LV_INDEV_STATE_REL;
    }
    last_key = 0;
  }
  else
  {
    if (keyboard_interrupted || (my_millis() > next_key_scan_ms))
    {
      uint32_t key;

      if (trackball_up_count)
      {
        key = LV_KEY_UP;
        --trackball_up_count;
      }
      else if (trackball_down_count)
      {
        key = LV_KEY_DOWN;
        --trackball_down_count;
      }
      else if (trackball_left_count)
      {
        key = LV_KEY_LEFT;
        --trackball_left_count;
      }
      else if (trackball_right_count)
      {
        key = LV_KEY_RIGHT;
        --trackball_right_count;
      }
      else if (trackball_click_count)
      {
        key = LV_KEY_ENTER;
        --trackball_click_count;
      }
      else
      {
        Wire.requestFrom(TDECK_KEYBOARD_ADDR, 1);
        while (Wire.available() > 0)
        {
           key = Wire.read();
        }
      }

      if (key > 0)
      {
        switch (key)
        {
        case 8:
          key = LV_KEY_BACKSPACE;
          break;
        case 9:
          key = LV_KEY_NEXT;
          break;
        case 10:
        case 13:
          key = LV_KEY_ENTER;
          break;
        case 27:
          key = LV_KEY_ESC;
          break;
        case 180:
          key = LV_KEY_LEFT;
          break;
        case 181:
          key = LV_KEY_UP;
          break;
        case 182:
          key = LV_KEY_DOWN;
          break;
        case 183:
          key = LV_KEY_RIGHT;
          break;
        }

        if (tdeck_display_is_on && tdeck_screensaver_off_since() > 2000) data->key = key;
        data->state = LV_INDEV_STATE_PR;

        next_key_scan_ms = my_millis() + KEY_SCAN_MS_INTERVAL;
        keyboard_interrupted = false;
        timestamp_last_activity = my_millis();

        last_key = key;
      }
    }
  }
}

/* -------------------------------------------------------------------------- */

void tdeck_setup(void)
{
  serial_begin();
  bool initialization_failed = false;

  highlander = xSemaphoreCreateBinaryStatic(&highlanderBuffer);
  assert(highlander);
  xSemaphoreGive(highlander);

  /* ------------------------------------------------------------ */
  GFX_EXTRA_PRE_INIT();
  
  SPI.end();
  SPI.begin(TDECK_SPI_SCK, TDECK_SPI_MISO, TDECK_SPI_MOSI);
  
  if (!gfx->begin())
  {
    Serial.println("ERROR: gfx->begin() failed!");
    initialization_failed = true;
  }
  gfx->fillScreen(BLACK);
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);

  /* ------------------------------------------------------------ */
  touch_init(gfx->width(), gfx->height(), gfx->getRotation());

  /* ------------------------------------------------------------ */
  gfx->invertDisplay(true);

  /* ------------------------------------------------------------ */
  pinMode(TDECK_KEYBOARD_INT, INPUT_PULLUP);
  attachInterrupt(TDECK_KEYBOARD_INT, ISR_key, FALLING);

  uint8_t kbtry = 0;
  while (true)
  {
    delay(250);
    Wire.requestFrom(TDECK_KEYBOARD_ADDR, 1);
    delay(250);
    if (Wire.read() != -1) break;
    kbtry++;
    if (kbtry > 5)
    {
      Serial.println(SERIAL_PREFIX "ERROR: Keyboard not working, please turn device off and on again!");
      initialization_failed = true;
      break;
    }
  }

  /* ------------------------------------------------------------ */
  pinMode(TDECK_TRACKBALL_UP, INPUT_PULLUP);
  attachInterrupt(TDECK_TRACKBALL_UP, ISR_up, FALLING);
  pinMode(TDECK_TRACKBALL_DOWN, INPUT_PULLUP);
  attachInterrupt(TDECK_TRACKBALL_DOWN, ISR_down, FALLING);
  pinMode(TDECK_TRACKBALL_LEFT, INPUT_PULLUP);
  attachInterrupt(TDECK_TRACKBALL_LEFT, ISR_left, FALLING);
  pinMode(TDECK_TRACKBALL_RIGHT, INPUT_PULLUP);
  attachInterrupt(TDECK_TRACKBALL_RIGHT, ISR_right, FALLING);
  pinMode(TDECK_TRACKBALL_CLICK, INPUT_PULLUP);
  attachInterrupt(TDECK_TRACKBALL_CLICK, ISR_click, FALLING);

  /* ------------------------------------------------------------ */
  lv_init();

  screenWidth = gfx->width();
  screenHeight = gfx->height();

#ifdef ESP32
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 40, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else
  disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * screenWidth * 20);
#endif
  if (!disp_draw_buf)
  {
    Serial.println(SERIAL_PREFIX "ERROR: LVGL disp_draw_buf allocate failed!");
    initialization_failed = true;
  }
  else
  {
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * 20); // 10 sufficient
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    // disp_drv.full_refresh = true; 
    lv_disp_t *disp;
    disp = lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    static lv_indev_drv_t indev_drv_key;
    lv_indev_drv_init(&indev_drv_key);
    indev_drv_key.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_key.read_cb = my_keyboard_read;
    static lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_key);

    static lv_group_t *g = lv_group_create();
    lv_group_set_default(g);
    lv_indev_set_group(kb_indev, g);

    ui_init();
    lv_timer_handler();

    if (initialization_failed)
    {
      gui_transition_to_screen(SCREEN_MAINTENANCE);
      tdeck_loop();
      int wait = 0;
      while(1)
      {
        wait++;
        Serial.println("ERROR: T-Deck hardware initialization failed. Device needs power cycle!");
        lv_timer_handler();
        delay(1000);
        if (wait == 60) ESP.restart(); // Rebooting does not fix hardware init errors, so this is somewhat useless.
      }
    }
  }

  return;
}

/* -------------------------------------------------------------------------- */

void tdeck_re_init_spi(void)
{
  if (highlander) xSemaphoreTake(highlander, portMAX_DELAY);
  GFX_EXTRA_PRE_INIT();
  SPI.end();
  SPI.begin(TDECK_SPI_SCK, TDECK_SPI_MISO, TDECK_SPI_MOSI);
  if (!gfx->begin()) serial_writeln("ERROR: gfx begin failed during re-init");
  gfx->fillScreen(BLACK);
  gfx->invertDisplay(true);
  if (highlander) xSemaphoreGive(highlander);
  radio_setup();
  startReceiveMode();
  return;
}

/* -------------------------------------------------------------------------- */

void tdeck_loop()
{
  uint16_t wait_counter = 0;

  /* Wait for SX126x background task to finish */
  while (digitalRead(TDECK_RADIO_CS) == LOW)
  {
    wait_counter++;
    if (wait_counter > 5)
    {
      serial_writeln("WARNING: Potential SPI Interface deadlock - busy with radio CS");
      wait_counter = 0;
    }
    delay(1);
  }

  int cpu = get_cpufreq();
  cpu_high();
  gfx->drawPixel(0, 0, 7);

  lv_timer_handler();
  delay(1);

  if (gui_need_screen_refresh)
  {
    gui_need_screen_refresh = false;
    if (tdeck_display_is_on)
    {
      gui_transition_to_screen(gui_get_current_screen());
      lv_timer_handler();
      lv_obj_invalidate(lv_scr_act()); // Forces the screen to be redrawn
      lv_refr_now(NULL); // Forces LVGL to refresh immediately
    }
  }

  if (cpu == 80) cpu_low();
  return;
}

extern bool has_txed;
extern int64_t has_txed_timestamp;

void gui_handle_PTSD(bool forced)
{
  return; // disabled for now
  int64_t now = my_millis();

  if ((now - has_txed_timestamp > 15000) || forced)
  {
    has_txed = false;
    int cpu = get_cpufreq();

    cpu_high();
    serial_writeln("INFO: Forced GUI redraw");

    // tdeck_re_init_spi();
    delay(10);
    lv_obj_invalidate(lv_scr_act()); // Forces the screen to be redrawn
    lv_refr_now(NULL); // Forces LVGL to refresh immediately

    tdeck_loop();
    gui_transition_to_screen(SCREEN_SPLASH);
    tdeck_loop();
    delay(500);
    set_gui_needs_screen_refresh(true);
    tdeck_loop();
    delay(5);
    tdeck_loop();
    delay(5);

    gui_transition_to_screen(SCREEN_PREVIOUS);
    tdeck_loop();
    delay(5);
    set_gui_needs_screen_refresh(true);
    tdeck_loop();
    delay(5);
    tdeck_loop();

    if (cpu == 80) cpu_low();
  }

  return;
}

/* EOF */