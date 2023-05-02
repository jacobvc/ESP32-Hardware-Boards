/** LvglBinding.cpp
 * Generated using ESP-Object-Messaging: tools/ui_bind.py. Do not edit manually.
 * 
 */        
#include "LvglHost.h"
#include "ui/ui.h"
#include "LvglBinding.h"

void ui_binding_init(LvglHost& host)
{
    host.addProducer("sample", ui_btnSample, BUTTON_CT, LV_EVENT_CLICKED);
    host.addProducer("zoom", ui_txaFiles, TEXTAREA_CT, LV_EVENT_VALUE_CHANGED);
    host.addConsumer("zoom", ui_txaFiles, TEXTAREA_CT, LV_EVENT_VALUE_CHANGED);
    host.addProducer("ls", ui_btnLs, BUTTON_CT, LV_EVENT_PRESSED);
    host.addProducer("zoom_slider", ui_sldBacklight, SLIDER_CT, LV_EVENT_VALUE_CHANGED);
    host.addConsumer("zoom_slider", ui_sldBacklight, SLIDER_CT, LV_EVENT_VALUE_CHANGED);
}
