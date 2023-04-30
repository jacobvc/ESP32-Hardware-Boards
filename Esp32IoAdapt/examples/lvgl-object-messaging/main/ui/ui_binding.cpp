#include "LvglHost.h"
#include "ui.h"
#include "binding.h"

void ui_binding_init(LvglHost& host)
{
    host.addProducer("sample", ui_btnSample, BUTTON_CT, (1 << LV_EVENT_CLICKED));
    host.addProducer("files", ui_txaFiles, TEXTAREA_CT, (1 << LV_EVENT_VALUE_CHANGED));
    host.addConsumer("files", ui_txaFiles, TEXTAREA_CT, (1 << LV_EVENT_VALUE_CHANGED));
    host.addProducer("ls", ui_btnLs, BUTTON_CT, (1 << LV_EVENT_CLICKED));
    host.addProducer("backlight", ui_sldBacklight, SLIDER_CT, (1 << LV_EVENT_VALUE_CHANGED));
    host.addConsumer("backlight", ui_sldBacklight, SLIDER_CT, (1 << LV_EVENT_VALUE_CHANGED));
}
