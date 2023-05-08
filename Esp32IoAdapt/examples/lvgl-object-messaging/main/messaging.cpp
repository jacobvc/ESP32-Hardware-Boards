#include "ObjMsg.h"

#include "GpioHost.h"
#include "JoystickHost.h"
#include "ServoHost.h"
#include "LvglHost.h"
#include "messaging.h"
#include "Esp32IoAdapt.h"

#include "WebsocketHost.h"

#include <unordered_map>

#define TAG "APP"

// Pan-tilt Slider and joystick pin/channel assignments
#define PT_SLIDER IA_GVI1_CHAN

#define PT_JOY_PINS IA_GVIIS1_PINS
#define PT_JOY_CHANS IA_GVIIS1_CHANS

// Zoom Slider and joystick pin/channel assignments
#define ZOOM_SLIDER IA_GVI2_CHAN

#define ZOOM_JOY_PINS IA_GVSSS1_PINS
#define ZOOM_JOY_CHANS IA_GVSSS1_CHANS

// Servo pin assignments
#define ZOOM_SERVO_X_PIN IA_GVS1_PIN

// Sample intervals
#define SAMPLE_INTERVAL_MS 100

// Component names
#define PT_JOY_NAME "pantilt"
#define PT_SLIDER_NAME "pantilt_slider"
#define ZOOM_JOY_NAME "zoom"
#define ZOOM_SLIDER_NAME "zoom_slider"
#define ZOOM_SERVO_X_NAME "zoom_servo_x"

// Origin IDs
enum Origins
{
  ORIGIN_CONTROLLER,
  ORIGIN_JOYSTICK,
  ORIGIN_SERVO,
  ORIGIN_ADC,
  ORIGIN_WEBSOCKET,
  ORIGIN_LVGL,
  ORIGIN_GPIO,
};

const char *origins[] = {
  "ORIGIN_CONTROLLER",
  "ORIGIN_JOYSTICK",
  "ORIGIN_SERVO",
  "ORIGIN_ADC",
  "ORIGIN_WEBSOCKET",
  "ORIGIN_LVGL",
  "ORIGIN_GPIO",
};

// Variables
TaskHandle_t MessageTaskHandle;

// Objects
ObjMsgTransport transport(MSG_QUEUE_MAX_DEPTH);

// Component hosts
AdcHost adc(transport, ORIGIN_ADC, SAMPLE_INTERVAL_MS);
GpioHost gpio(transport, ORIGIN_GPIO);

JoystickHost joysticks(adc, gpio, transport, ORIGIN_JOYSTICK, SAMPLE_INTERVAL_MS);
ServoHost servos(transport, ORIGIN_SERVO);
LvglHost lvgl(transport, ORIGIN_LVGL);
WebsocketHost ws(transport, ORIGIN_WEBSOCKET);

// Implementation
static void MessageTask(void *pvParameters)
{
  ObjMsgDataRef data;
  TickType_t wait = portMAX_DELAY;

  for (;;)
  {
    if (transport.receive(data, wait))
    {
      ObjMsgData *msg = data.get();
      ObjMsgJoystickData *jsd = static_cast<ObjMsgJoystickData *>(data.get());
      if (jsd && jsd->GetName().compare(ZOOM_JOY_NAME) == 0)
      {
        joystick_sample_t sample;
        jsd->GetRawValue(sample);
        ObjMsgServoData servo(jsd->GetOrigin(), ZOOM_SERVO_X_NAME, sample.x);
        servos.consume(&servo);
      }
      else
      {
        string str;
        msg->Serialize(str);
        ESP_LOGI(TAG, "(%s) JSON: %s", origins[msg->GetOrigin()], str.c_str());
      }
      if (!msg->IsFrom(ORIGIN_WEBSOCKET))
      {
        ws.consume(data.get());
      }
      if (!msg->IsFrom(ORIGIN_LVGL))
      {
        lvgl.consume(data.get());
      }
    }
  }
}

/** Example 
 * 
*/
bool LvglJoystickComsumer(LvglHost *host, ObjMsgData *data)
{
  ObjMsgJoystickData *jsd = static_cast<ObjMsgJoystickData *>(data);
  if (jsd)
  {
    joystick_sample_t sample;
    jsd->GetRawValue(sample);

    ObjMsgDataInt x(jsd->GetOrigin(), (jsd->GetName() + "x").c_str(), sample.x);
    host->consume(&x);
    ObjMsgDataInt y(jsd->GetOrigin(), (jsd->GetName() + "y").c_str(), sample.y);
    host->consume(&y);

    return true;
  }
  return false;
}

void MessagingInit()
{
  // Configure and start joysticks
  joysticks.add(PT_JOY_NAME, CHANGE_EVENT,
                PT_JOY_CHANS[1], PT_JOY_CHANS[0], PT_JOY_PINS[2]);
  joysticks.add(ZOOM_JOY_NAME, CHANGE_EVENT,
                ZOOM_JOY_CHANS[1], ZOOM_JOY_CHANS[0], ZOOM_JOY_PINS[2]);
  joysticks.start();

  // Configure and start servos
  servos.add(ZOOM_SERVO_X_NAME, ZOOM_SERVO_X_PIN);
  servos.start();

  // Configure and start ADC
  adc.Add(ZOOM_SLIDER_NAME, CHANGE_EVENT, ZOOM_SLIDER,
          ADC_ATTEN_DB_11, ADC_BITWIDTH_12, 4096, 0, 100);
  adc.Add(PT_SLIDER_NAME, CHANGE_EVENT, PT_SLIDER,
          ADC_ATTEN_DB_11, ADC_BITWIDTH_12, 4096, 0, 100);
  adc.start();

  // Configure and start GPIO
  gpio.start();

  // Configure and start lvgl
  LvglBindingInit(lvgl);
  lvgl.addVirtualConsumer(ZOOM_JOY_NAME, LvglJoystickComsumer);
  lvgl.start();

  xTaskCreate(MessageTask, "MessageTask",
              CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE + 1024, NULL,
              tskIDLE_PRIORITY, &MessageTaskHandle);

  // Configure and start wifi / http / websocket
  extern void HttpPaths(WebsocketHost & ws);
  HttpPaths(ws);
  // WARNING - (for now) do this last. It will not return until WiFi starts
  ws.start();
}
