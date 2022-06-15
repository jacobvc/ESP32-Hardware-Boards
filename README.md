# Esp32 Dev Module Hardware Boards
Hardware and circuit board designs to enhance esp32 Dev Module connectivity

I think that I have about average circuit board design and soldering skills for an (older) mechanical engineer. As mostly a hobbiest developer, I tend to develop reusable functionality focused circuit boards as opposed to project specific circuit boards. My soldering skills are OK for 0.1 inch center parts. 0805 surface mount parts are at or beyond my limit.

I am sharing these eagle board designs that I have found to be useful; mostly as a sort of tidy bridge between a breadboard and a project specific design. Unless otherwise specified, they are for (30 pin) esp32 Dev Module boards.

## Esp32 Dev Module Pins
This design consists of two 15 pin pin headers; ESP32-LEFT and ESP32-RIGHT, with wires and labels corresponding to pin functionality. They are not 'connected' to one-another and are generally intended to be used as a quick start in laying out a board for one ot the other side of an esp32 Dev Module. 

## Lcd 3.5 Connectors
This design connects one side of an esp32 Dev Module (ESP32-LEFT) to a LCD touch screen display. The display uses all of the GPIOs on that side except two. A four pin connector supports connecting the display mounted SD socket using a shared SPI. The two unused ports are routed to a four pin I2C connector. 

## Right Side IO Connectors
This design connects one side of an esp32 Dev Module (ESP32-RIGHT) to JST ZH 1.5mm keyed connectors. The connectors include
1. 1 each 2 pin GS connector (switch)
2. 3 each 3 pin GVS connectors (potentiomer, servo motor, etc)
3. 1 each 4 pin GVSS connector (serial port)
4. 2 each 5 pin GVSSS connectors (thumb joystick)
