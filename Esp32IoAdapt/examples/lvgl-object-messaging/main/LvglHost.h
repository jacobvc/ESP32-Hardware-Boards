/** @file
 *
 * The ServoHost class provides for operation of
 * servo motors
 *
 */
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lvgl.h"
#include <binding.h>
#include <unordered_map>
#include "ObjMsg.h"

// typedef ObjMsgDataInt ObjMsgServoData;

class LvglHost : public ObjMsgHost
{
  typedef struct
  {
    const char *name;
    lv_obj_t *obj;
    enum BindingTypes type;
  } control_reg_def_t;

public:
  LvglHost(ObjMsgTransport &transport, uint16_t origin)
      : ObjMsgHost(transport, "LVGL", origin)
  {
  }

  int addConsumer(const char *name, lv_obj_t *control, enum BindingTypes binding, uint32_t eventFlags)
  {
    control_reg_def_t def = {.name = name, .obj = control, .type = binding};
    consume_map[name] = def;

    return true;
  }
  int addProducer(const char *name, lv_obj_t *control, enum BindingTypes binding, uint32_t eventFlags)
  {
    control_reg_def_t def = {.name = name, .obj = control, .type = binding};
    produce_map[control] = def;

    // Store name in user_data for lookup
    lv_obj_add_event_cb(control, publish_cb, LV_EVENT_ALL, this);

    return true;
  }

  bool start()
  {
    return true;
  }

  BaseType_t produce(ObjMsgDataRef data)
  {
    return transport.send(data);
  }

  bool consume(ObjMsgDataRef data)
  {
    ObjMsgData *msg = data.get();
    control_reg_def_t *ctx = GetConsumer(msg->GetName());
    if (ctx) {
      switch (ctx->type)
      {
      case BUTTON_ID:
        /* code */
        break;
      
      default:
        break;
      }
    }
    return false;
  }

protected:
  control_reg_def_t *GetConsumer(string name)
  {
    unordered_map<string, control_reg_def_t>::iterator found = consume_map.find(name);
    if (found != consume_map.end())
    {
      return &found->second;
    }
    else
    {
      return NULL;
    }
  }

  control_reg_def_t *GetProducer(lv_obj_t *control)
  {
    unordered_map<lv_obj_t *, control_reg_def_t>::iterator found = produce_map.find(control);
    if (found != produce_map.end())
    {
      return &found->second;
    }
    else
    {
      return NULL;
    }
  }
  static void publish_cb(lv_event_t *event)
  {
    LvglHost *host = (LvglHost *)event->user_data;
    control_reg_def_t *ctx = host->GetProducer(event->target);
    if (ctx)
    {
      // Publish(ctx, event);
    }
  }

  std::unordered_map<std::string, control_reg_def_t> consume_map;
  std::unordered_map<lv_obj_t *, control_reg_def_t> produce_map;
};
