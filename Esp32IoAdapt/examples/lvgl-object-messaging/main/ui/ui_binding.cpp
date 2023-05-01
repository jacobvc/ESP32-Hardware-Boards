#include "LvglHost.h"
#include "ui.h"
#include "binding.h"

void ui_binding_init(LvglHost& host)
{
    host.addProducer("sample", ui_btnSample, BUTTON_CT, LV_EVENT_CLICKED);
    host.addProducer("files", ui_txaFiles, TEXTAREA_CT, LV_EVENT_VALUE_CHANGED);
    host.addConsumer("zoom", ui_txaFiles, TEXTAREA_CT, LV_EVENT_VALUE_CHANGED);
    host.addProducer("ls", ui_btnLs, BUTTON_CT, LV_EVENT_PRESSED);
    host.addProducer("zoom_slider", ui_sldBacklight, SLIDER_CT, LV_EVENT_VALUE_CHANGED);
    host.addConsumer("zoom_slider", ui_sldBacklight, SLIDER_CT, LV_EVENT_VALUE_CHANGED);
}
