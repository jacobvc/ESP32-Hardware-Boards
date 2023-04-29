#pragma once


enum BindingTypes {
    LABEL_ID,
    TEXTAREA_ID,
    COMBO_ID,
    LED_ID,
    GAUGE_ID,
    BUTTON_ID,
    SLIDER_ID,
    SWITCH_ID,
};

class LvglHost;

void ui_binding_init(LvglHost& host);
