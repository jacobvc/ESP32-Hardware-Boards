#pragma once


enum ControlType {
    ARC_CT,
    BUTTON_CT,
    IMAGE_CT,
    LABEL_CT,
    PANEL_CT,
    TEXTAREA_CT,
    CALENDAR_CT,
    CHECKBOX_CT,
    COLORWHEEL_CT,
    DROPDOWN_CT,
    IMGBUTTON_CT,
    KEYBOARD_CT,
    ROLLER_CT,
    SLIDER_CT,
    SWITCH_CT,
};

class LvglHost;

void ui_binding_init(LvglHost& host);
