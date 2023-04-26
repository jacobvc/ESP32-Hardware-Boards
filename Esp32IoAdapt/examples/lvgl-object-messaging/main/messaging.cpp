#include "ObjMsg.h"

#include "JoystickHost.h"
#include "ServoHost.h"
#include "messaging.h"
#include "Esp32IoAdapt.h"

#include "WebsocketHost.h"

#include <unordered_map>

// Pan-tilt Slider and joystick IDs and logical assignments
#define PT_JOY_NAME "pantilt"
#define PT_JOY_PINS IA_GVIIS1_PINS
#define PT_JOY_CHANS IA_GVIIS1_CHANS
#define PT_SLIDER IA_GVI1_CHAN

// Zoom Slider and joystick IDs and logical assignments
#define ZOOM_JOY_NAME "zoom"
#define ZOOM_JOY_PINS IA_GVSSS1_PINS
#define ZOOM_JOY_CHANS IA_GVSSS1_CHANS

#define ZOOM_SLIDER_NAME "zoom_slider"
#define ZOOM_SLIDER IA_GVI2_CHAN

#define SAMPLE_INTERVAL_MS 100

#define ZOOM_SERVO_X "zoom_servo_x"
#define ZOOM_SERVO_X_PIN IA_GVS1_PIN

#define ORIGIN_CONTROLLER 0x01
#define ORIGIN_JOYSTICK 0x02
#define ORIGIN_SERVO 0x04
#define ORIGIN_ADC 0x08
#define ORIGIN_WEBSOCKET 0x10

TaskHandle_t MessageTaskHandle;

ObjMsgTransport transport(MSG_QUEUE_MAX_DEPTH);

AdcHost adc(transport, ORIGIN_ADC, SAMPLE_INTERVAL_MS);
JoystickHost joysticks(adc, transport, 
  ORIGIN_JOYSTICK, SAMPLE_INTERVAL_MS);
ServoHost servos(transport, ORIGIN_SERVO);
WebsocketHost ws(transport, ORIGIN_WEBSOCKET);

static void MessageTask(void *pvParameters)
{
  ObjMsgDataRef data;
  TickType_t wait = portMAX_DELAY;

  for (;;)
  {
    if (transport.receive(data, wait))
    {
      ObjMsgJoystickData *jsd = static_cast<ObjMsgJoystickData *>(data.get());
      if (jsd && jsd->GetName().compare(ZOOM_JOY_NAME) == 0)
      {
        joystick_sample_t sample;
        jsd->GetValue(sample);
        ObjMsgDataRef servo = ObjMsgServoData::create(
          jsd->GetOrigin(), ZOOM_SERVO_X, sample.x);
        servos.consume(servo);
      }
      else
      {
        string str;
        data.get()->serializeJson(str);
        printf("JSON: %s\n", str.c_str());
      }
    }
  }
}

void MessagingInit()
{
  joysticks.add(PT_JOY_NAME, CHANGE_EVENT, 
    PT_JOY_CHANS[1], PT_JOY_CHANS[0], PT_JOY_PINS[2]);
  joysticks.add(ZOOM_JOY_NAME, CHANGE_EVENT, 
    ZOOM_JOY_CHANS[1], ZOOM_JOY_CHANS[0], ZOOM_JOY_PINS[2]);
  joysticks.start();

  servos.add(ZOOM_SERVO_X, ZOOM_SERVO_X_PIN);
  servos.start();

  adc.Add(ZOOM_SLIDER_NAME, CHANGE_EVENT, ZOOM_SLIDER, 
    ADC_ATTEN_DB_11, ADC_BITWIDTH_12, 4096, 0, 100);
  adc.start();

  xTaskCreate(MessageTask, "MessageTask",
              CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE + 128, NULL,
              tskIDLE_PRIORITY, &MessageTaskHandle);

  // Test the servo
  if (0)
  {
    ObjMsgDataRef pos = ObjMsgServoData::create(ORIGIN_CONTROLLER, 
      ZOOM_SERVO_X, -90);
    servos.consume(pos);
    vTaskDelay(pdMS_TO_TICKS(2000));
    pos = ObjMsgServoData::create(ORIGIN_CONTROLLER, ZOOM_SERVO_X, 90);
    servos.consume(pos);
    vTaskDelay(pdMS_TO_TICKS(2000));
    pos = ObjMsgServoData::create(ORIGIN_CONTROLLER, ZOOM_SERVO_X, 0);
    servos.consume(pos);
  }
  ws.start();
}
