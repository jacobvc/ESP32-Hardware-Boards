#include "LvglHost.h"
#include "ui.h"
#include "binding.h"

void ui_binding_init(LvglHost& host)
{
    // addProducer("sdcard", ui_lblSdCard, LABEL_CT, (1 << LV_EVENT_VALUE_CHANGED));
    // addConsumer("sdcard", ui_lblSdCard, LABEL_CT, (1 << LV_EVENT_VALUE_CHANGED));
    // addProducer("path", ui_lblPath, LABEL_CT, (1 << LV_EVENT_VALUE_CHANGED));
    // addConsumer("path", ui_lblPath, LABEL_CT, (1 << LV_EVENT_VALUE_CHANGED));
    host.addProducer("sample", ui_btnSample, BUTTON_CT, (1 << LV_EVENT_CLICKED));
    // addConsumer("sample", ui_btnSample, BUTTON_CT, (1 << LV_EVENT_CLICKED));
    host.addProducer("files", ui_txaFiles, TEXTAREA_CT, (1 << LV_EVENT_VALUE_CHANGED));
    host.addConsumer("files", ui_txaFiles, TEXTAREA_CT, (1 << LV_EVENT_VALUE_CHANGED));
    // addProducer("temp", ui_lblTemp, LABEL_CT, (1 << LV_EVENT_VALUE_CHANGED));
    // addConsumer("temp", ui_lblTemp, LABEL_CT, (1 << LV_EVENT_VALUE_CHANGED));
    // addProducer("pressure", ui_lblPressure, LABEL_CT, (1 << LV_EVENT_VALUE_CHANGED));
    // addConsumer("pressure", ui_lblPressure, LABEL_CT, (1 << LV_EVENT_VALUE_CHANGED));
    host.addProducer("ls", ui_btnLs, BUTTON_CT, (1 << LV_EVENT_CLICKED));
    // addConsumer("ls", ui_btnLs, BUTTON_CT, (1 << LV_EVENT_CLICKED));
    host.addProducer("backlight", ui_sldBacklight, SLIDER_CT, (1 << LV_EVENT_VALUE_CHANGED));
    host.addConsumer("backlight", ui_sldBacklight, SLIDER_CT, (1 << LV_EVENT_VALUE_CHANGED));
}
