# IO Adapter Boards
EspIoAdapt is a ESP-32 "LEFT_SIDE" board that present th I/O pins on the left side of 
a ESP32 Dev Module as (mostly) JST ZH 1.5mm connectors.

## EspIoAdapt
0.1 inch connectors
* 1 each 2 pin GS connector  (switch)
* 1 each 3 pin GVS connector (servo motor, etc). 

JST ZH 1.5mm keyed connectors. The connectors include
* 2 each 3 pin GVI connectors (potentiometer, etc). 
* 1 each 4 pin GVSS connector (serial port, I2C, etc)
* 1 each 5 pin GVSSS connectors (joystick, misc)
* 1 each 5 pin GVIIS connectors (joystick, misc)

Note:
*  GVS.. == Ground / 3v3 / Sense GPIO
*  GVI.. == Ground / 3v3 / Input only GPIO
*  The "V" pins are all 3.3v, so they may not be suited for 5V devices such as 5V servo motors

![Top Side](/assets/Esp32IoAdaptTop.png "Top View")
![Bottom Side](/assets/Esp32IoAdaptBottom.png "Bottom View")
Intended to be used with pre-built JST-ZH cables. 
![Cable](/assets/s-l300.jpg)

## Joystick Adapter
A simple adapter board adapting straight through from 1.5 mm to 0.1 inch. Intended to connect thumb joystick via JST ZH connector 
or soldered wires from JST ZH cable.

![Joystick adapter](/assets/joystickadapt.png "Joystick adapter")

## Slider Adapter
A simple adapter / mounting board for mounting a 35 mm slider potentiometer and connecting via JST ZH connector 
or soldered wires from JST ZH cable.

![Slider](/assets/35mmSlider.png "Slider")
![Slider adapter](/assets/slideradapt.png "Slider adapter")
