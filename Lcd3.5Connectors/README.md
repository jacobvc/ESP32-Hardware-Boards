# Lcd 3.5 Connectors
This design connects the left side of an esp32 Dev Module to a LCD touch screen display. The display uses all of the GPIOs on that side except two. A four pin connector supports connecting the display mounted SD socket using a shared SPI. The two unused ports are routed to a four pin I2C connector. 

[Used with this display](http://www.lcdwiki.com/3.5inch_SPI_Module_ILI9488_SKU:MSP3520)

The hardware connections (for LVGL driver) is supported by these definitions in 'sdconfig'

    CONFIG_LV_TFT_DISPLAY_SPI_VSPI=y
    CONFIG_LV_DISP_SPI_MOSI=4
    # CONFIG_LV_DISPLAY_USE_SPI_MISO is not set
    CONFIG_LV_DISP_SPI_CLK=17
    CONFIG_LV_DISP_SPI_CS=19
    CONFIG_LV_DISP_PIN_DC=5
    CONFIG_LV_DISP_PIN_RST=18
    CONFIG_LV_TOUCH_DRIVER_PROTOCOL_SPI=y
    # CONFIG_LV_TOUCH_CONTROLLER_SPI_HSPI is not set
    CONFIG_LV_TOUCH_CONTROLLER_SPI_VSPI=y

    #
    # Touchpanel (XPT2046) Pin Assignments
    #
    CONFIG_LV_TOUCH_SPI_MISO=2
    CONFIG_LV_TOUCH_SPI_MOSI=4
    CONFIG_LV_TOUCH_SPI_CLK=17
    CONFIG_LV_TOUCH_SPI_CS=16
    CONFIG_LV_TOUCH_PIN_IRQ=15
    # end of Touchpanel (XPT2046) Pin Assignments

And separately to use the on-board SPI:

    #define SHARED_SPI_SD_CS_PIN 22
    #define SHARED_SPI_SD_HOST VSPI_HOST // same as LCD

![Top Side](/assets/Lcd3_5ConnectTop.png)
![Bottom Side](/assets/Lcd3_5ConnectBottom.png)
