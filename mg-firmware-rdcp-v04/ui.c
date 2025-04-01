// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: ROLORAN_TDeck_Neuhaus

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_ScreenSplash
void ui_ScreenSplash_screen_init(void);
lv_obj_t * ui_ScreenSplash;
lv_obj_t * ui_PanelSplashBackground;
lv_obj_t * ui_ImageSplashLogo;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenOANonCrisis
void ui_ScreenOANonCrisis_screen_init(void);
lv_obj_t * ui_ScreenOANonCrisis;
lv_obj_t * ui_LabelOANonCrisis;
lv_obj_t * ui_TextAreaOANonCrisis;
lv_obj_t * ui_ButtonOANCNav1;
lv_obj_t * ui_ButtonOANCNav2;
lv_obj_t * ui_ButtonOANCNav3;
lv_obj_t * ui_ButtonOANCNav4;
lv_obj_t * ui_LabelOANCNav1;
lv_obj_t * ui_LabelOANCNav2;
lv_obj_t * ui_LabelOANCNav3;
lv_obj_t * ui_LabelOANCNav4;
lv_obj_t * ui_OANCTextNum;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenOACrisis
void ui_ScreenOACrisis_screen_init(void);
lv_obj_t * ui_ScreenOACrisis;
lv_obj_t * ui_ButtonEMER;
lv_obj_t * ui_ButtonCIRE;
lv_obj_t * ui_LabelOACrisis;
lv_obj_t * ui_TextAreaOACrisis;
lv_obj_t * ui_LabelButtonEMER;
lv_obj_t * ui_LabelButtonCIRE;
lv_obj_t * ui_ButtonOACNav1;
lv_obj_t * ui_ButtonOACNav2;
lv_obj_t * ui_ButtonOACNav3;
lv_obj_t * ui_ButtonOACNav4;
lv_obj_t * ui_OACTextNum;
lv_obj_t * ui_LabelOACNav1;
lv_obj_t * ui_LabelOACNav2;
lv_obj_t * ui_LabelOACNav3;
lv_obj_t * ui_LabelOACNav4;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenEMER
void ui_ScreenEMER_screen_init(void);
lv_obj_t * ui_ScreenEMER;
lv_obj_t * ui_ButtonEMERSubmit;
lv_obj_t * ui_ButtonEMERAbort;
lv_obj_t * ui_LabelEMERButtonSubmit;
lv_obj_t * ui_LabelEMERButtonAbort;
lv_obj_t * ui_LabelEMERHeader;
lv_obj_t * ui_TextAreaEMERWho;
lv_obj_t * ui_TextAreaEMERWhere;
lv_obj_t * ui_TextAreaEMERWhat;
lv_obj_t * ui_LabelEMERWho;
lv_obj_t * ui_LabelEMERWhere;
lv_obj_t * ui_LabelEMERWhat;
lv_obj_t * ui_Label1;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenCIRE
void ui_ScreenCIRE_screen_init(void);
lv_obj_t * ui_ScreenCIRE;
lv_obj_t * ui_LabelCIREHeader;
lv_obj_t * ui_ButtonCIREsubmit;
lv_obj_t * ui_ButtonCIREabort;
lv_obj_t * ui_LabelButtonCIREsubmit;
lv_obj_t * ui_LabelButtonCIREabort;
lv_obj_t * ui_LabelCIREWho;
lv_obj_t * ui_LabelCIRECategory;
lv_obj_t * ui_LabelCIREFreetext;
lv_obj_t * ui_TextAreaCIREFreetext;
lv_obj_t * ui_TextAreaCIREWho;
lv_obj_t * ui_DropdownCIRECategory;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenMaintenance
void ui_ScreenMaintenance_screen_init(void);
lv_obj_t * ui_ScreenMaintenance;
lv_obj_t * ui_LabelMaintenanceMiddle;
lv_obj_t * ui_LabelMaintenanceTop;
lv_obj_t * ui_LabelMaintenanceBottom;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenRESP
void ui_ScreenRESP_screen_init(void);
lv_obj_t * ui_ScreenRESP;
lv_obj_t * ui_LabelRESPHeader1;
lv_obj_t * ui_ButtonRESPsubmit;
void ui_event_ButtonRESPabort(lv_event_t * e);
lv_obj_t * ui_ButtonRESPabort;
lv_obj_t * ui_LabelButtonRESPsubmit;
lv_obj_t * ui_LabelButtonRESPabort;
lv_obj_t * ui_LabelRESPFreetext;
lv_obj_t * ui_TextAreaRESPFreetext;
lv_obj_t * ui_TextAreaRESPoa;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenProvisioning
void ui_ScreenProvisioning_screen_init(void);
lv_obj_t * ui_ScreenProvisioning;
lv_obj_t * ui_LabelProvisioningMiddle;
lv_obj_t * ui_LabelProvisioningTop;
lv_obj_t * ui_LabelProvisioningBottom;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenEMER2
void ui_ScreenEMER2_screen_init(void);
lv_obj_t * ui_ScreenEMER2;
lv_obj_t * ui_ButtonEMERSubmit1;
lv_obj_t * ui_ButtonEMERAbort1;
lv_obj_t * ui_LabelEMERButtonSubmit1;
lv_obj_t * ui_LabelEMERButtonAbort1;
lv_obj_t * ui_LabelEMERHeader1;
lv_obj_t * ui_CIRE2SwitchLebensbedrohlich;
lv_obj_t * ui_Label2;
lv_obj_t * ui_Dropdown2;
lv_obj_t * ui_CIRE2SwitchErsthelfer;
lv_obj_t * ui_Label3;
lv_obj_t * ui_Label4;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenEULA
void ui_ScreenEULA_screen_init(void);
lv_obj_t * ui_ScreenEULA;
lv_obj_t * ui_LabelMaintenanceMiddle1;
lv_obj_t * ui_LabelEULA1;
lv_obj_t * ui_EULAokButton;
lv_obj_t * ui_Label5;
// CUSTOM VARIABLES

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_ButtonRESPabort(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_ScreenOACrisis, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenOACrisis_screen_init);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_ScreenSplash_screen_init();
    ui_ScreenOANonCrisis_screen_init();
    ui_ScreenOACrisis_screen_init();
    ui_ScreenEMER_screen_init();
    ui_ScreenCIRE_screen_init();
    ui_ScreenMaintenance_screen_init();
    ui_ScreenRESP_screen_init();
    ui_ScreenProvisioning_screen_init();
    ui_ScreenEMER2_screen_init();
    ui_ScreenEULA_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_ScreenSplash);
}
