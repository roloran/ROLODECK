// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: ROLORAN_TDeck_Neuhaus

#include "ui.h"

void ui_ScreenRESP_screen_init(void)
{
    ui_ScreenRESP = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenRESP, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelRESPHeader1 = lv_label_create(ui_ScreenRESP);
    lv_obj_set_width(ui_LabelRESPHeader1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelRESPHeader1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelRESPHeader1, 2);
    lv_obj_set_y(ui_LabelRESPHeader1, -111);
    lv_obj_set_align(ui_LabelRESPHeader1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelRESPHeader1, "Antwort an Krisenstab schicken");
    lv_obj_set_style_text_color(ui_LabelRESPHeader1, lv_color_hex(0xFABE1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelRESPHeader1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelRESPHeader1, &ui_font_Hack12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ButtonRESPsubmit = lv_btn_create(ui_ScreenRESP);
    lv_obj_set_width(ui_ButtonRESPsubmit, 100);
    lv_obj_set_height(ui_ButtonRESPsubmit, 50);
    lv_obj_set_x(ui_ButtonRESPsubmit, -99);
    lv_obj_set_y(ui_ButtonRESPsubmit, 89);
    lv_obj_set_align(ui_ButtonRESPsubmit, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ButtonRESPsubmit, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ButtonRESPsubmit, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ButtonRESPsubmit, lv_color_hex(0xCE9600), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonRESPsubmit, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ButtonRESPabort = lv_btn_create(ui_ScreenRESP);
    lv_obj_set_width(ui_ButtonRESPabort, 100);
    lv_obj_set_height(ui_ButtonRESPabort, 50);
    lv_obj_set_x(ui_ButtonRESPabort, 101);
    lv_obj_set_y(ui_ButtonRESPabort, 89);
    lv_obj_set_align(ui_ButtonRESPabort, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ButtonRESPabort, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ButtonRESPabort, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ButtonRESPabort, lv_color_hex(0x625D5A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonRESPabort, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelButtonRESPsubmit = lv_label_create(ui_ScreenRESP);
    lv_obj_set_width(ui_LabelButtonRESPsubmit, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelButtonRESPsubmit, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelButtonRESPsubmit, -99);
    lv_obj_set_y(ui_LabelButtonRESPsubmit, 90);
    lv_obj_set_align(ui_LabelButtonRESPsubmit, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelButtonRESPsubmit, " Antwort \nabsenden");
    lv_obj_set_style_text_align(ui_LabelButtonRESPsubmit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelButtonRESPsubmit, &ui_font_Hack14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelButtonRESPabort = lv_label_create(ui_ScreenRESP);
    lv_obj_set_width(ui_LabelButtonRESPabort, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelButtonRESPabort, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelButtonRESPabort, 102);
    lv_obj_set_y(ui_LabelButtonRESPabort, 90);
    lv_obj_set_align(ui_LabelButtonRESPabort, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelButtonRESPabort, "Abbrechen");
    lv_obj_set_style_text_font(ui_LabelButtonRESPabort, &ui_font_Hack14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelRESPFreetext = lv_label_create(ui_ScreenRESP);
    lv_obj_set_width(ui_LabelRESPFreetext, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelRESPFreetext, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelRESPFreetext, -16);
    lv_obj_set_y(ui_LabelRESPFreetext, -26);
    lv_obj_set_align(ui_LabelRESPFreetext, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelRESPFreetext, "Ihre Antwort (max. 150 Zeichen):");
    lv_obj_set_style_text_color(ui_LabelRESPFreetext, lv_color_hex(0xFABE1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelRESPFreetext, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelRESPFreetext, &ui_font_Hack14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TextAreaRESPFreetext = lv_textarea_create(ui_ScreenRESP);
    lv_obj_set_width(ui_TextAreaRESPFreetext, 298);
    lv_obj_set_height(ui_TextAreaRESPFreetext, 70);
    lv_obj_set_x(ui_TextAreaRESPFreetext, 0);
    lv_obj_set_y(ui_TextAreaRESPFreetext, 19);
    lv_obj_set_align(ui_TextAreaRESPFreetext, LV_ALIGN_CENTER);
    lv_textarea_set_max_length(ui_TextAreaRESPFreetext, 150);
    lv_textarea_set_placeholder_text(ui_TextAreaRESPFreetext, "Hier Nachricht eingeben...");
    lv_obj_set_style_text_font(ui_TextAreaRESPFreetext, &ui_font_Hack14, LV_PART_MAIN | LV_STATE_DEFAULT);



    ui_TextAreaRESPoa = lv_textarea_create(ui_ScreenRESP);
    lv_obj_set_width(ui_TextAreaRESPoa, 289);
    lv_obj_set_height(ui_TextAreaRESPoa, 50);
    lv_obj_set_x(ui_TextAreaRESPoa, -2);
    lv_obj_set_y(ui_TextAreaRESPoa, -68);
    lv_obj_set_align(ui_TextAreaRESPoa, LV_ALIGN_CENTER);
    lv_textarea_set_text(ui_TextAreaRESPoa, "Bitte antworten Sie unten auf die Frage des Krisenstabs.");
    lv_textarea_set_placeholder_text(ui_TextAreaRESPoa, "Bezug");
    lv_obj_set_style_text_color(ui_TextAreaRESPoa, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_TextAreaRESPoa, 255, LV_PART_MAIN | LV_STATE_DEFAULT);



    lv_obj_add_event_cb(ui_ButtonRESPabort, ui_event_ButtonRESPabort, LV_EVENT_ALL, NULL);

}
