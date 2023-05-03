#pragma once

#include "driver/gpio.h"
// #include <algorithm>

#include "ObjMsg.h"

enum GpioFlags // GPIO flags (_GF)
{
  INVERTED_GF = 0x01,
  OFF_EVENT_GF = 0x04, // ON / OFF state after inverted applied
  ON_EVENT_GF = 0x08,
  IS_INPUT_GF = 0x10,
  // IS_BUTTON_GF = 0x20,
};

typedef ObjMsgDataInt ObjMsgGpioData;
class GpioHost;

class GpioPort
{
public:
  /** Default constructor to support map */
  GpioPort() {}
  /** Add 'channel' with range from zero to 'maxCount',
   * and scaling from 'min' to 'max'
   */
  GpioPort(string name, gpio_num_t pin, ObjMsgSample mode, GpioFlags flags, GpioHost *host)
  {
    this->host = host;
    this->name = name;
    this->mode = mode;
    this->pin = pin;
    this->flags = flags;
  }

  int GetValue()
  {
    return value;
  }
  GpioHost *host;
  string name;
  gpio_num_t pin;
  ObjMsgSample mode;
  GpioFlags flags;

  uint8_t value; // Measured value
};

class GpioHost : public ObjMsgHost
{
public:
  GpioHost(ObjMsgTransport &transport, uint16_t origin)
      : ObjMsgHost(transport, "GpioHost", origin)
  {
    event_queue = NULL;
  }

  GpioPort *Add(string name, ObjMsgSample mode, GpioFlags flags, gpio_num_t port)
  {
    ports[name] = GpioPort(name, port, mode, flags, this);
    GpioPort *tmp = &ports[name];

    if (mode == CHANGE_EVENT)
    {
      anyChangeEvents = true;
    }
    dataFactory.registerClass(origin_id, name, ObjMsgGpioData::create);

    return tmp;
  }

  bool consume(ObjMsgData *data)
  {
    // consider NOT IS_INPUT_GF
    return false;
  }

  bool start()
  {
    unordered_map<string, GpioPort>::iterator it;

    uint64_t input_mask = 0;
    uint64_t output_mask = 0;

    for (it = ports.begin(); it != ports.end(); it++)
    {
      GpioPort &port = it->second;
      if (port.flags & IS_INPUT_GF) {
        input_mask |= 1ull << port.pin;
      }
      else {
        output_mask |= 1ull << port.pin;
      }
    }

    // OUTPUTS
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;        //disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT;              //set as output mode
    io_conf.pin_bit_mask = output_mask;           //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; //disable pull-down mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;     //disable pull-up mode
    gpio_config(&io_conf);                        //configure GPIO with the given settings
    // INPUTS
    io_conf.intr_type = GPIO_INTR_ANYEDGE;        //interrupt of both edges
    io_conf.pin_bit_mask = input_mask;            //bit mask of the pins, use GPIO4/5 here
    io_conf.mode = GPIO_MODE_INPUT;               //set as input mode    
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; //disable pull-down mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;     //disable pull-up mode
    gpio_config(&io_conf);

    if (anyChangeEvents)
    {
      event_queue = xQueueCreate(10, sizeof(void *));
      xTaskCreate(input_task, "gpio_input_task",
        CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE + 50, this, 10, NULL);

      // install gpio isr service
      gpio_install_isr_service(0);

      for (it = ports.begin(); it != ports.end(); it++)
      {
        GpioPort &port = it->second;
        if (port.mode == CHANGE_EVENT)
        {
          // consider IS_INPUT_GF
          gpio_isr_handler_add(port.pin, isr_handler, &port);
        }
      }
    }
    return true;
  }

  bool measure(GpioPort *port)
  {
    // consider IS_INPUT_GF
    int level = gpio_get_level(port->pin);
    uint8_t flags = port->flags;
    if (flags & INVERTED_GF)
    {
      level = !level;
    }
    if ((level && (flags & OFF_EVENT_GF)) || (!level && (flags & ON_EVENT_GF)))
    {
      if (level != port->value)
      {
        port->value = level;
      }
    }
    return false;
  }

protected:
  static void input_task(void *arg)
  {
    GpioHost *host = (GpioHost *)arg;
    for (;;)
    {
      GpioPort *port = NULL;
      if (xQueueReceive(host->event_queue, &port, portMAX_DELAY))
      {
        if (host->measure(port))
        {
          ObjMsgDataRef point = ObjMsgGpioData::create(host->origin_id, port->name.c_str(), port->value);
          host->produce(point);
        }
      }
    }
  }

  static void isr_handler(void *arg)
  {
    GpioPort *port = (GpioPort *)arg;
    xQueueSendFromISR(port->host->event_queue, &port, NULL);
  }

public:
  unordered_map<string, GpioPort> ports;
  bool anyChangeEvents;
  QueueHandle_t event_queue;
};