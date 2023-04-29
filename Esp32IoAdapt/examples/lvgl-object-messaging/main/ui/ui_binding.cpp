#include "LvglHost.h"
#include "ui.h"
#include "binding.h"

void ui_binding_init(LvglHost& host)
{
    host.addProducer("sample", ui_btnSample, BUTTON_ID, (1 << LV_EVENT_CLICKED));
    host.addProducer("files", ui_txaFiles, TEXTAREA_ID, (1 << LV_EVENT_CLICKED));
    host.addConsumer("files", ui_txaFiles, TEXTAREA_ID, (1 << LV_EVENT_CLICKED));
    host.addProducer("pressure", ui_lblPressure, LABEL_ID, (1 << LV_EVENT_CLICKED));
    host.addProducer("ls", ui_btnLs, BUTTON_ID, (1 << LV_EVENT_CLICKED));
}
