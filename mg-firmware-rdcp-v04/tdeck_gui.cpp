#include "tdeck_gui.h"
#include "ui.h"
#include <Arduino.h>
#include "roloran-tdeck-serial.h"
#include "settings-device.h"
#include "roloran-tdeck-persistence.h"
#include "roloran-rdcp.h"
#include "roloran-tdeck-hal.h"
#include "roloran-board.h"
#include "settings-scenario.h"

uint8_t my_previous_screen = SCREEN_SPLASH;
uint8_t my_current_screen = SCREEN_SPLASH;
uint8_t my_previous_nonsplash_screen = SCREEN_EULA;

uint8_t gui_get_current_screen(void) { return my_current_screen; }

bool infrastructure_in_crisis = true;
void set_infrastructure_in_crisis(bool value) { infrastructure_in_crisis = value; return; }

void gui_transition_to_screen(uint8_t screenId)
{
  uint8_t tmp = my_previous_screen;
  my_previous_screen = my_current_screen;
  my_current_screen = screenId;
  if (screenId == SCREEN_PREVIOUS)
  {
      my_current_screen = tmp;
      if (my_current_screen == SCREEN_SPLASH) my_current_screen = my_previous_nonsplash_screen;
  }

  if ((screenId != SCREEN_SPLASH) && (screenId != SCREEN_PREVIOUS))
  {
    my_previous_nonsplash_screen = screenId;
  }

  if ((my_current_screen == SCREEN_SPLASH) && (my_previous_screen == SCREEN_SPLASH)) my_current_screen = my_previous_nonsplash_screen;

  cpu_high();

  char uiinfo[INFOLEN];
  snprintf(uiinfo, INFOLEN, "INFO: Switching to UI screen %d", my_current_screen);
  serial_writeln(uiinfo);

  switch(my_current_screen)
  {
    case SCREEN_MAINTENANCE:
      _ui_screen_change( &ui_ScreenMaintenance, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenMaintenance_screen_init);
      break;
    case SCREEN_CIRE:
      _ui_screen_change( &ui_ScreenCIRE, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenCIRE_screen_init);
      break;
    case SCREEN_EMERGENCY:
      _ui_screen_change( &ui_ScreenEMER, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenEMER_screen_init);
      break;
    case SCREEN_OACRISIS:
      _ui_screen_change( &ui_ScreenOACrisis, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenOACrisis_screen_init);
      break;
    case SCREEN_OANONCRISIS:
      _ui_screen_change( &ui_ScreenOANonCrisis, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenOANonCrisis_screen_init);
      break;
    case SCREEN_SPLASH:
      _ui_screen_change( &ui_ScreenSplash, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenSplash_screen_init);
      break;
    case SCREEN_PROVISIONING:
      _ui_screen_change( &ui_ScreenProvisioning, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenProvisioning_screen_init);
      break;
    case SCREEN_RESP:
      _ui_screen_change( &ui_ScreenRESP, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenRESP_screen_init);
      break;
    case SCREEN_EMERSTEPTWO:
      _ui_screen_change( &ui_ScreenEMER2, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenEMER2_screen_init);
      break;
    case SCREEN_EULA:
      _ui_screen_change( &ui_ScreenEULA, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenEULA_screen_init);
      break;
    default:
      break;
  }

  return;
}

bool gui_cire_buttons_currently_disabled = false;

/**
 * GUI callback when user wants to send an RDCP emergency message.
 */
void gui_callback_emer(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  if ( event_code == LV_EVENT_CLICKED)
  {
    bool enoughData = true;
    lv_obj_set_style_text_color(ui_LabelEMERWho, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT ); 
    lv_obj_set_style_text_color(ui_LabelEMERWhere, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT ); 
    lv_obj_set_style_text_color(ui_LabelEMERWhat, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT ); 

    String s_who = lv_textarea_get_text(ui_TextAreaEMERWho);
    if (s_who.length() < 3)
    {
      enoughData = false;
      lv_obj_set_style_text_color(ui_LabelEMERWho, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT ); 
    }

    String s_where = lv_textarea_get_text(ui_TextAreaEMERWhere);
    if (s_where.length() < 3)
    {
      enoughData = false;
      lv_obj_set_style_text_color(ui_LabelEMERWhere, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT ); 
    }

    String s_what = lv_textarea_get_text(ui_TextAreaEMERWhat);
    if (s_what.length() < 3)
    {
      enoughData = false;
      lv_obj_set_style_text_color(ui_LabelEMERWhat, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT ); 
    }

    if (enoughData) gui_transition_to_screen(SCREEN_EMERSTEPTWO);
  }

  return;
}

int64_t last_button_press = 0;

/**
 * GUI callback when user wants to send an RDCP citizen request message.
 */
void gui_callback_cire(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 5000) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    bool enoughData = true;
    lv_obj_set_style_text_color(ui_LabelCIREWho, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT ); 
    lv_obj_set_style_text_color(ui_LabelCIREFreetext, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT ); 

    String s_who = lv_textarea_get_text(ui_TextAreaCIREWho);
    if (s_who.length() < 3)
    {
      enoughData = false;
      lv_obj_set_style_text_color(ui_LabelCIREWho, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT ); 
    }

    String s_what = lv_textarea_get_text(ui_TextAreaCIREFreetext);
    if (s_what.length() < 3)
    {
      enoughData = false;
      lv_obj_set_style_text_color(ui_LabelCIREFreetext, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT ); 
    }

    if (s_what.length() < 3) return;

    int category = lv_dropdown_get_selected(ui_DropdownCIRECategory);

    if (enoughData)
    {
      String owner = getOwnerDisplayName();
      if (s_who.equals(owner)) s_who = "!"; // Compress if reporting citizen is registered owner of device

      /* Prepare CIRE textual content */
      s_who.replace("#", "*");
      s_what.replace("#", "*");
      char c_who[DATABUFLEN], c_what[DATABUFLEN];
      s_who.toCharArray(c_who, DATABUFLEN);
      s_what.toCharArray(c_what, DATABUFLEN);

      char ciretext[DATABUFLEN];
      snprintf(ciretext, DATABUFLEN, "%s#%s#%d", c_who, c_what, category);

      char info[DATABUFLEN];
      snprintf(info, DATABUFLEN, "INFO: Preparing CIRE: %s", ciretext);
      serial_writeln(info);

      uint16_t refnr = get_next_cire_nonce(getMyRDCPAddress());
      gui_disable_cire_buttons();

      char gui_text[FATLEN];
      snprintf(gui_text, FATLEN, "\nIhre Nachricht (\"%s\") wird mit der Referenznummer %04X-%d gesendet. Bitte lassen Sie den MERLIN-Messenger eingeschaltet und warten Sie auf weitere Informationen.", c_what, getMyRDCPAddress(), refnr);
      mb_add_local_message(gui_text, refnr, 0, 60002, true);

      lv_textarea_set_text(ui_TextAreaCIREFreetext, "");

      gui_transition_to_screen(SCREEN_OACRISIS);
      set_gui_needs_screen_refresh(true);
      for (int i=0; i<5; i++) { tdeck_loop(); }

      rdcp_send_cire(RDCP_MSGTYPE_CIRE_SUBTYPE_REQUEST, refnr, ciretext);
    }
  }

  return;
}

void gui_callback_resp_submit(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 5000) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    bool enoughData = true;
    lv_obj_set_style_text_color(ui_LabelRESPFreetext, lv_color_hex(0xff8800), LV_PART_MAIN | LV_STATE_DEFAULT ); 

    String s_freetext = lv_textarea_get_text(ui_TextAreaRESPFreetext);
    if (s_freetext.length() < 1)
    {
      enoughData = false;
      lv_obj_set_style_text_color(ui_LabelRESPFreetext, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT ); 
    }

    if (enoughData)
    {
      char ciretext[DATABUFLEN];
      s_freetext.toCharArray(ciretext, DATABUFLEN);

      char info[DATABUFLEN];
      snprintf(info, DATABUFLEN, "INFO: Preparing CIRE: %s", ciretext);
      serial_writeln(info);

      uint16_t refnr = get_current_inquiry_refnr();
      gui_switch_red_button_mode(RED_BUTTON_MODE_EMERGENCY);
      gui_disable_cire_buttons();

      char gui_text[FATLEN];
      snprintf(gui_text, FATLEN, "\nIhre Antwort (\"%s\") wird mit der Referenznummer %04X-%04X gesendet. Bitte lassen Sie den MERLIN-Messenger eingeschaltet und warten Sie auf weitere Informationen.", ciretext, getMyRDCPAddress(), refnr);
      mb_add_local_message(gui_text, refnr, 0, RDCP_TWO_DAYS, true);

      lv_textarea_set_text(ui_TextAreaRESPFreetext, "");

      gui_transition_to_screen(SCREEN_OACRISIS);
      set_gui_needs_screen_refresh(true);
      for (int i=0; i<5; i++) { tdeck_loop(); }

      rdcp_send_cire(RDCP_MSGTYPE_CIRE_SUBTYPE_RESPONSE, refnr, ciretext);
    }
  }

  return;
}

void gui_callback_resp_abort(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 2000) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    gui_transition_to_screen(SCREEN_OACRISIS);
    set_gui_needs_screen_refresh(true);
  }

  return;
}

uint8_t red_button_mode = RED_BUTTON_MODE_EMERGENCY;

void gui_switch_red_button_mode(uint8_t new_mode)
{
  if (new_mode == RED_BUTTON_MODE_EMERGENCY)
  {
    red_button_mode = RED_BUTTON_MODE_EMERGENCY;
    gui_enable_cire_buttons();
  }
  else
  {
    red_button_mode = RED_BUTTON_MODE_INQUIRY;
    gui_enable_cire_buttons();
    
    lv_label_set_text(ui_LabelButtonEMER, "Nachfrage\nbeantworten");
    lv_label_set_text(ui_LabelButtonCIRE, " \n ");
    lv_obj_add_state(ui_LabelButtonCIRE, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(ui_ButtonEMER, lv_color_hex(0xff8800), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ButtonCIRE, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  return;
}

void gui_callback_red_button_pressed(lv_event_t *e)
{
  if (gui_cire_buttons_currently_disabled) return;

  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  if (event_code == LV_EVENT_CLICKED)
  {
    if (red_button_mode == RED_BUTTON_MODE_EMERGENCY)
    {
      gui_transition_to_screen(SCREEN_EMERGENCY);
    }
    else if (red_button_mode == RED_BUTTON_MODE_INQUIRY)
    {
      gui_transition_to_screen(SCREEN_RESP);
    }  
  }
  return;
}

void gui_callback_blue_button_pressed(lv_event_t *e)
{
  if (gui_cire_buttons_currently_disabled) return;

  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  if (event_code == LV_EVENT_CLICKED)
  {
    if (red_button_mode == RED_BUTTON_MODE_EMERGENCY) gui_transition_to_screen(SCREEN_CIRE);
  }
  return;
}

void gui_callback_eula_ok(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  if (event_code == LV_EVENT_CLICKED)
  {
    serial_writeln("INFO: EULA has been acknowledged");
    if (infrastructure_in_crisis)
    {
      gui_transition_to_screen(SCREEN_OACRISIS);
    }
    else
    {
      gui_transition_to_screen(SCREEN_OANONCRISIS);
    }
  }
  return;
}

void gui_callback_emer_abort(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  if (event_code == LV_EVENT_CLICKED)
  {
    gui_transition_to_screen(SCREEN_OACRISIS);
  }
  return;
}

void gui_callback_emer2_abort(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  if (event_code == LV_EVENT_CLICKED)
  {
    gui_transition_to_screen(SCREEN_EMERGENCY);
  }
  return;
}

void gui_callback_cire_abort(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  if (event_code == LV_EVENT_CLICKED)
  {
    gui_transition_to_screen(SCREEN_OACRISIS);
  }
  return;
}

void gui_disable_cire_buttons(void)
{
  lv_label_set_text(ui_LabelButtonEMER, "Ihre Nachricht\nwird verarbeitet");
  lv_label_set_text(ui_LabelButtonCIRE, "Bitte\nwarten");
  lv_obj_add_state(ui_LabelButtonEMER, LV_STATE_DISABLED);
  lv_obj_add_state(ui_LabelButtonCIRE, LV_STATE_DISABLED);
  gui_cire_buttons_currently_disabled = true;
  lv_obj_set_style_bg_color(ui_ButtonEMER, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_ButtonCIRE, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);

  return;
}

void gui_enable_cire_buttons(void)
{
  lv_label_set_text(ui_LabelButtonEMER, "Notfall / Unfall\nan Krisenstab melden");
  lv_label_set_text(ui_LabelButtonCIRE, "Andere Nachricht an\nKrisenstab schicken");
  lv_obj_clear_state(ui_LabelButtonEMER, LV_STATE_DISABLED);
  lv_obj_clear_state(ui_LabelButtonCIRE, LV_STATE_DISABLED);
  gui_cire_buttons_currently_disabled = false;
  lv_obj_set_style_bg_color(ui_ButtonEMER, lv_color_hex(0xE04646), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_ButtonCIRE, lv_color_hex(0x036FFC), LV_PART_MAIN | LV_STATE_DEFAULT);

  return;
}

void gui_crisis_clear(void)
{
  lv_textarea_set_text(ui_TextAreaOACrisis, "");
  set_gui_needs_screen_refresh(true);
  return;
}

void gui_noncrisis_clear(void)
{
  lv_textarea_set_text(ui_TextAreaOANonCrisis, "");
  set_gui_needs_screen_refresh(true);
  return;
}

void gui_crisis_add_text(char *s)
{
  screensaver_off();
  lv_textarea_set_accepted_chars(ui_TextAreaOACrisis, NULL);
  lv_textarea_set_max_length(ui_TextAreaOACrisis, 0);
  lv_textarea_set_cursor_pos(ui_TextAreaOACrisis, LV_TEXTAREA_CURSOR_LAST); // append new text at the end
  lv_textarea_add_text(ui_TextAreaOACrisis, s); // add the message itself
  lv_textarea_add_text(ui_TextAreaOACrisis, "\n"); // along with a linebreak
  lv_textarea_set_accepted_chars(ui_TextAreaOACrisis, "*");
  set_gui_needs_screen_refresh(true);

  return;
}

void gui_noncrisis_add_text(char *s)
{
  screensaver_off();
  lv_textarea_set_accepted_chars(ui_TextAreaOANonCrisis, NULL);
  lv_textarea_set_max_length(ui_TextAreaOANonCrisis, 0);
  lv_textarea_set_cursor_pos(ui_TextAreaOANonCrisis, LV_TEXTAREA_CURSOR_LAST); // append new text at the end
  lv_textarea_add_text(ui_TextAreaOANonCrisis, s); // add the message itself
  lv_textarea_add_text(ui_TextAreaOANonCrisis, "\n"); // along with a linebreak
  lv_textarea_set_accepted_chars(ui_TextAreaOANonCrisis, "*");
  set_gui_needs_screen_refresh(true);
  return;
}

void gui_resp_add_text(char *s)
{
  screensaver_off();
  lv_textarea_set_accepted_chars(ui_TextAreaRESPoa, NULL);
  lv_textarea_set_max_length(ui_TextAreaRESPoa, 0);
  lv_textarea_set_text(ui_TextAreaRESPoa, "");
  lv_textarea_set_cursor_pos(ui_TextAreaRESPoa, LV_TEXTAREA_CURSOR_LAST); // append new text at the end
  lv_textarea_add_text(ui_TextAreaRESPoa, s); // add the message itself
  lv_textarea_set_accepted_chars(ui_TextAreaRESPoa, "*");
  set_gui_needs_screen_refresh(true);

  return;
}

void gui_callback_emer_send(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 5000) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    String s_who = lv_textarea_get_text(ui_TextAreaEMERWho);
    String s_where = lv_textarea_get_text(ui_TextAreaEMERWhere);
    String s_what = lv_textarea_get_text(ui_TextAreaEMERWhat);

    if (s_what.length() < 3) return;

    String owner = getOwnerDisplayName();
    if (s_who.equals(owner)) s_who = "!"; // Compress if reporting citizen is registered owner of device

    /* 
     * Assemble state based on choices on EMER2 UI Screen:
     * 4-bit mask: 0 0 0 0 b4 b3 b2 b1
     * b1 = 1 if life threatening emergency, else 0
     * b2 = 1 if someone who can help is there already, else 0
     * b4 b3 = 1 .. 4 choice in dropdown list
     */

    uint8_t emer2state = 65; // Basis is 'A' to encode the state as an uppercase letter
    if (lv_obj_has_state(ui_CIRE2SwitchLebensbedrohlich, LV_STATE_CHECKED) == true) emer2state += 1;
    if (lv_obj_has_state(ui_CIRE2SwitchErsthelfer, LV_STATE_CHECKED) == true) emer2state += 2;
    uint8_t add_num_people = (lv_dropdown_get_selected(ui_Dropdown2)) << 2;
    emer2state += add_num_people;

    /* Prepare CIRE textual content */
    s_who.replace("#", "*");
    s_where.replace("#", "*");
    s_what.replace("#", "*");
    char c_who[DATABUFLEN], c_where[DATABUFLEN], c_what[DATABUFLEN];
    s_who.toCharArray(c_who, DATABUFLEN);
    s_where.toCharArray(c_where, DATABUFLEN);
    s_what.toCharArray(c_what, DATABUFLEN);

    char ciretext[DATABUFLEN];
    snprintf(ciretext, DATABUFLEN, "%s#%s#%s#%c", c_who, c_where, c_what, emer2state);

    char info[DATABUFLEN];
    snprintf(info, DATABUFLEN, "INFO: Preparing CIRE: %s", ciretext);
    serial_writeln(info);

    uint16_t refnr = get_next_cire_nonce(getMyRDCPAddress());
    gui_disable_cire_buttons();

    gui_transition_to_screen(SCREEN_OACRISIS); // Moved to here. :-)
    set_gui_needs_screen_refresh(true);
    for (int i=0; i<5; i++) tdeck_loop();

    char gui_text[FATLEN];
    snprintf(gui_text, FATLEN, "Ihre Nachricht (\"%s\") wird mit der Referenznummer %04X-%d gesendet. Bitte lassen Sie den MERLIN-Messenger eingeschaltet und warten Sie auf weitere Informationen.", c_what, getMyRDCPAddress(), refnr);
    mb_add_local_message(gui_text, refnr, 0, RDCP_TWO_DAYS, true);

    lv_textarea_set_text(ui_TextAreaEMERWhere, "");
    lv_textarea_set_text(ui_TextAreaEMERWhat, "");

    rdcp_send_cire(RDCP_MSGTYPE_CIRE_SUBTYPE_EMERGENCY, refnr, ciretext);
  }
  return;
}

void gui_callback_oac_nav1(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 500) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    mb_show_crisis_first();
  }

  return;
}

void gui_callback_oac_nav2(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 500) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    mb_show_crisis_previous();
  }

  return;
}

void gui_callback_oac_nav3(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 500) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    mb_show_crisis_next();
  }

  return;
}

void gui_callback_oac_nav4(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 500) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    mb_show_crisis_last();
  }

  return;
}

void gui_callback_oanc_nav1(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 500) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    mb_show_noncrisis_first();
  }

  return;
}

void gui_callback_oanc_nav2(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 500) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    mb_show_noncrisis_previous();
  }

  return;
}

void gui_callback_oanc_nav3(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 500) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    mb_show_noncrisis_next();
  }

  return;
}

void gui_callback_oanc_nav4(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  int64_t now = my_millis();
  if (now - last_button_press < 500) return;
  last_button_press = now;

  if (event_code == LV_EVENT_CLICKED)
  {
    mb_show_noncrisis_last();
  }

  return;
}

void gui_callback_setup(void)
{
  // EULA Screen OK Button
  lv_obj_add_event_cb(ui_EULAokButton, gui_callback_eula_ok, LV_EVENT_CLICKED, NULL);

  // OACRISIS Buttons
  lv_obj_add_event_cb(ui_ButtonEMER, gui_callback_red_button_pressed, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonCIRE, gui_callback_blue_button_pressed, LV_EVENT_CLICKED, NULL);

  // EMER1 Screen Buttons
  lv_obj_add_event_cb(ui_ButtonEMERSubmit, gui_callback_emer, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonEMERAbort, gui_callback_emer_abort, LV_EVENT_CLICKED, NULL);

  // EMER2 Screen Buttons
  lv_obj_add_event_cb(ui_ButtonEMERSubmit1, gui_callback_emer_send, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonEMERAbort1, gui_callback_emer2_abort, LV_EVENT_CLICKED, NULL);
  lv_label_set_text(ui_LabelEMERButtonAbort1, "Vorheriger\nSchritt");
  lv_obj_set_align(ui_LabelEMERButtonAbort1, LV_ALIGN_CENTER);
  lv_obj_set_style_text_align(ui_LabelEMERButtonAbort1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

  // CIRE Screen Buttons
  lv_obj_add_event_cb(ui_ButtonCIREsubmit, gui_callback_cire, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonCIREabort, gui_callback_cire_abort, LV_EVENT_CLICKED, NULL);

  // RESP Screen Buttons
  lv_obj_add_event_cb(ui_ButtonRESPsubmit, gui_callback_resp_submit, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonRESPabort, gui_callback_resp_abort, LV_EVENT_CLICKED, NULL);

  // Disable entering text into OA text areas
  lv_textarea_set_accepted_chars(ui_TextAreaOACrisis, "*");
  lv_textarea_set_accepted_chars(ui_TextAreaOANonCrisis, "*");

  String owner = getOwnerDisplayName();
  char owner_c[INFOLEN];
  if (owner.length() > 3) 
  {
    owner.toCharArray(owner_c, INFOLEN);
    lv_textarea_set_text(ui_TextAreaEMERWho, owner_c);
    lv_textarea_set_text(ui_TextAreaCIREWho, owner_c);
  }

  lv_obj_set_style_bg_color(ui_ButtonCIRE, lv_color_hex(0x036FFC), LV_PART_MAIN | LV_STATE_DEFAULT);

  // Navigation buttons for browsing through messages on Crisis and NonCrisis UI screens
  lv_obj_add_event_cb(ui_ButtonOACNav1, gui_callback_oac_nav1, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonOACNav2, gui_callback_oac_nav2, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonOACNav3, gui_callback_oac_nav3, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonOACNav4, gui_callback_oac_nav4, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonOANCNav1, gui_callback_oanc_nav1, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonOANCNav2, gui_callback_oanc_nav2, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonOANCNav3, gui_callback_oanc_nav3, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_ButtonOANCNav4, gui_callback_oanc_nav4, LV_EVENT_CLICKED, NULL);

  /* Disable cursor blinking in read-only text areas */
  lv_obj_set_style_anim_time(ui_TextAreaOACrisis, 0, LV_STATE_USER_1 | LV_PART_CURSOR);
  lv_obj_set_style_border_width(ui_TextAreaOACrisis, 0, LV_STATE_USER_1 | LV_PART_CURSOR);
  lv_obj_add_state(ui_TextAreaOACrisis, LV_STATE_FOCUSED | LV_STATE_USER_1);
  lv_obj_set_style_anim_time(ui_TextAreaOANonCrisis, 0, LV_STATE_USER_1 | LV_PART_CURSOR);
  lv_obj_set_style_border_width(ui_TextAreaOANonCrisis, 0, LV_STATE_USER_1 | LV_PART_CURSOR);
  lv_obj_add_state(ui_TextAreaOANonCrisis, LV_STATE_FOCUSED | LV_STATE_USER_1);
  lv_obj_set_style_anim_time(ui_TextAreaRESPoa, 0, LV_STATE_USER_1 | LV_PART_CURSOR);
  lv_obj_set_style_border_width(ui_TextAreaRESPoa, 0, LV_STATE_USER_1 | LV_PART_CURSOR);
  lv_obj_add_state(ui_TextAreaRESPoa, LV_STATE_FOCUSED | LV_STATE_USER_1);

  return;
}

/* EOF */