/** LvglBinding.cpp   Generated 2023-05-08
 *
 * Generated using ESP-Object-Messaging: tools/ui_bind.py. Do not edit manually.
 * 
 */        
#include "LvglHost.h"
#include "ui/ui.h"
#include "LvglBinding.h"

void LvglBindingInit(LvglHost& host)
{
    host.addProducer("sample", ui_btnSample, BUTTON_CT, LV_EVENT_CLICKED);
    host.addConsumer("sample", ui_btnSample, BUTTON_CT);
    host.addConsumer("pressure", ui_lblPressure, LABEL_CT);
    host.addProducer("zoomy", ui_sldZoomY, SLIDER_CT, LV_EVENT_VALUE_CHANGED);
    host.addConsumer("zoomy", ui_sldZoomY, SLIDER_CT);
    host.addProducer("zoomx", ui_arcZoomX, ARC_CT, LV_EVENT_VALUE_CHANGED);
    host.addConsumer("zoomx", ui_arcZoomX, ARC_CT);
    host.addProducer("schedule", ui_calSchedule, CALENDAR_CT, LV_EVENT_VALUE_CHANGED);
    host.addConsumer("schedule", ui_calSchedule, CALENDAR_CT);
}
