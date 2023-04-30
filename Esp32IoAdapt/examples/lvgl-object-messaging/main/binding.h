#pragma once


enum ControlType {
    LABEL_CT,
    TEXTAREA_CT,
    COMBO_CT,
    LED_CT,
    GAUGE_CT,
    BUTTON_CT,
    SLIDER_CT,
    SWITCH_CT,
};

class LvglHost;

void ui_binding_init(LvglHost& host);
