#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>
//#include "GY521.h"

/*-------------------------------------Pins--------------------------------
Screen          Using SPI
GND             GND
VDD             3.3V or Vin
SCK             GPIO18
SDA             GPIO23 (MOSI)
RES             GPIO17
DC              GPIO16
CS              GPIO5

Gyroscope       Using I2C
VCC             3.3V or Vin
GND             GND
SCL             GPIO22 (I2C SCL)
SDA             GPIO21 (I2C SDA)

Buttons
Left Button     GPIO32
Right Button    GPIO33
-------------------------------------------------------------------------*/


U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI   u8g2  (U8G2_R0, 5, 16, 17);
//GY521 Gyroscope(0x68);

int Menu = 0;
int MainMenu = 0;
int ErrorMenu = 0;

int MenuCursor = 0;
