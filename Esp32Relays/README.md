# Esp32 Relays Board
This design utilizes the right side of an esp32 Dev Module for:

* A three channel relay board
* A CD4066 based analog switch. Two channels of the CD4066 are connected to 3.5mm TRS (input / output) audio connectors, and the other two are available on a pair of 0.1 in center 3 pin headers.
* A 3 pin (Ground, 5V, and Data on D32) connector intended to be used for Neopixel Matrix Display
* Two 3 pin (Ground, 3.3v, D25 or D26) connectors available for general use

A 3 pin Power connector is provided for **REQUIRED 3.3v input** (from the other side of the ESP32) and optional 5V power for when the system is not being powered by the ESP32 built in USB

Top View<br><img src="/assets/Esp32RelaysTop.png" width="392" height="120"><br>
Bottom View<br><img src="/assets/Esp32RelaysBottom.png" width="392" height="120">

## Components
The components required to assemble the board are:
* One each ESP32 WROOM Development Module
* One each 3 channel relay board
* One each 3 pin JSC SM Male connector with cable
* One each CD4066 ananlog switch
* Two each PJ-307 Audio jacks
* Two each (R1, R4) 4.7k 0805 resistors
* Four Each (R2, R3, R5, R6) 100k 0805 resistors
* Four Each (C1, C2, C3, C4) 1uf 0805 capacitors
* 0.1 center pin headers as desired

The board is designed to use this three channel relay, which requires a 5V supply but supports 3.3v logic.<br><img src="/assets/3xrelay.png" width="330" height="240"><br>

The assembly looks like this (with the larger components set in place but not soldered).<br><img src="/assets/Esp32RelaysAssembly.png" width="320" height="240">

The assembled components fit nicely into a 100 x 60 x 25 mm project box. The audio jacks are about 60.5 mm end to end, so you spread the case and they snap into place and help secure the assembly.<br><img src="/assets/Esp32RelaysLive.png" width="320" height="240">

