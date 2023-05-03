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

// Origin IDs (bit values)
#define ORIGIN_CONTROLLER (1 << 0)
#define ORIGIN_JOYSTICK   (1 << 1)
#define ORIGIN_SERVO      (1 << 2)
#define ORIGIN_ADC        (1 << 3)
#define ORIGIN_WEBSOCKET  (1 << 4)
#define ORIGIN_LVGL       (1 << 5)
#define ORIGIN_GPIO       (1 << 6)

// Variables
TaskHandle_t MessageTaskHandle;

// Objects
ObjMsgTransport transport(MSG_QUEUE_MAX_DEPTH);

// Component hosts
AdcHost adc(transport, ORIGIN_ADC, SAMPLE_INTERVAL_MS);
GpioHost gpio(transport, ORIGIN_GPIO);
JoystickHost joysticks(adc, transport, ORIGIN_JOYSTICK, SAMPLE_INTERVAL_MS);
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
      if (!msg->IsFrom(ORIGIN_WEBSOCKET)) {
        ws.consume(data.get());
      }
      if (!msg->IsFrom(ORIGIN_LVGL)) {
        lvgl.consume(data.get());
      }
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
        ESP_LOGI(TAG, "(%02x) JSON: %s", msg->GetOrigin() , str.c_str());
      }
    }
  }
}

/** HTTP root handler - serve the embedded file index.html
 * 
 * Embedded CMakefiles.txt directive: EMBED_FILES "index.html"
 */
esp_err_t http_root_handler(httpd_req_t *req)
{
    extern const unsigned char index_html_start[] asm("_binary_index_html_start");
    extern const unsigned char index_html_end[]   asm("_binary_index_html_end");
    const size_t index_html_size = (index_html_end - index_html_start);
    printf("index.html (%d bytes) from %p to %p\n", index_html_size, index_html_start, index_html_end);
    httpd_resp_send(req, (const char *)index_html_start, index_html_size);
    return ESP_OK;
}

/** HTTP favicon handler - serve the embedded file favicon.ico
 * 
 * Embedded CMakefiles.txt directive: EMBED_FILES "favicon.ico"
 */
esp_err_t http_favicon_handler(httpd_req_t *req)
{
    extern const unsigned char favicon_ico_start[] asm("_binary_favicon_ico_start");
    extern const unsigned char favicon_ico_end[]   asm("_binary_favicon_ico_end");
    const size_t favicon_ico_size = (favicon_ico_end - favicon_ico_start);
    printf("favicon.ico (%d bytes) from %p to %p\n", favicon_ico_size, favicon_ico_end);
    httpd_resp_send(req, (const char *)favicon_ico_start, favicon_ico_size);
    return ESP_OK;
}

/** HTTP joy.js handler - serve the embedded file joy.js
 * 
 * Embedded CMakefiles.txt directive: EMBED_FILES "joy.js"
 */
esp_err_t http_joy_js_handler(httpd_req_t *req)
{
    extern const unsigned char joy_js_start[] asm("_binary_joy_js_start");
    extern const unsigned char joy_js_end[]   asm("_binary_joy_js_end");
    const size_t joy_js_size = (joy_js_end - joy_js_start);
    printf("joy.js (%d bytes) from %p to %p\n", joy_js_size, joy_js_end);
    httpd_resp_send(req, (const char *)joy_js_start, joy_js_size);
    return ESP_OK;
}

void MessagingInit()
{
  joysticks.add(PT_JOY_NAME, CHANGE_EVENT, 
    PT_JOY_CHANS[1], PT_JOY_CHANS[0], PT_JOY_PINS[2]);
  joysticks.add(ZOOM_JOY_NAME, CHANGE_EVENT, 
    ZOOM_JOY_CHANS[1], ZOOM_JOY_CHANS[0], ZOOM_JOY_PINS[2]);
  joysticks.start();

  servos.add(ZOOM_SERVO_X_NAME, ZOOM_SERVO_X_PIN);
  servos.start();

  adc.Add(ZOOM_SLIDER_NAME, CHANGE_EVENT, ZOOM_SLIDER, 
    ADC_ATTEN_DB_11, ADC_BITWIDTH_12, 4096, 0, 100);
  adc.Add(PT_SLIDER_NAME, CHANGE_EVENT, PT_SLIDER, 
    ADC_ATTEN_DB_11, ADC_BITWIDTH_12, 4096, 0, 100);
  adc.start();

  gpio.start();

  LvglBindingInit(lvgl);
  lvgl.start();

  xTaskCreate(MessageTask, "MessageTask",
              CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE + 1024, NULL,
              tskIDLE_PRIORITY, &MessageTaskHandle);

  // Test the servo
  if (0)
  {
    ObjMsgServoData pos1(ORIGIN_CONTROLLER, ZOOM_SERVO_X_NAME, -90);
    servos.consume(&pos1);
    vTaskDelay(pdMS_TO_TICKS(2000));
    ObjMsgServoData pos2(ORIGIN_CONTROLLER, ZOOM_SERVO_X_NAME, 90);
    servos.consume(&pos2);
    vTaskDelay(pdMS_TO_TICKS(2000));
    ObjMsgServoData pos3(ORIGIN_CONTROLLER, ZOOM_SERVO_X_NAME, 0);
    servos.consume(&pos3);
  }

  ws.Add("/", http_root_handler, false);
  ws.Add("/favicon.ico", http_favicon_handler, false);
  ws.Add("/joy.js", http_joy_js_handler, false);

  ws.start();
}
