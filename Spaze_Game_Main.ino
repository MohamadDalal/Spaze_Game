#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>
#include "GY521.h"

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
GY521 Gyroscope(0x68);

int Menu = 0;
int MainMenu = 0;
int ErrorMenu = 0;

int MenuCursor = 0;

struct Ship
{
  //-----------------------------Inner Data----------------------------------//
  int Type;         //0 for player and 1 for enemy and 2 for asteroid
  int Speed;        //Speed in pixle per second
  int Health;       //Health points
  
  //------------------------------Visual Data--------------------------------//
  int TopLeftCoords[2]; //Coordinates of top left pixel first is x then is y
  int BitmapNum;    //Amount of bitmaps to be used. sprite might be split in different parts. Currently capped at 3
  //int HitboxNum;    //Number of hitboxes // Decided to tie hitboxes to bitmaps, will be added if needed
  int BitmapData1[4]; // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Width, Height]
  int BitmapData2[4]; // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Width, Height]
  int BitmapData3[4]; // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Width, Height]
  // Bitmap pointers will be assigned using a function. All bitmaps will be stored in one place.
  unsigned char *PntBitmap1;  //Pointer to first bitmap
  unsigned char *PntBitmap2;  //Pointer to second bitmap
  unsigned char *PntBitmap3;  //Pointer to third bitmap
};
//--------------------------------------------------Initializing structs--------------------------------------------
struct Ship Player;
struct Ship Enemy[3];
//CreateShip(/*obj*/Player,/*Type*/ 0,/*spd*/ 10,/*HP*/ 5,/*LUx*/ 26,/*LUy*/ 32,/*BMNo*/ 2,/*BMA1*/ 1,/*BMA2*/ 2,/*BMA3*/ 0,/*BMx1*/ 0,/*BMy1*/ 0,/*BMx2*/ 2,/*BMy2*/ 1,/*BMx3*/ 0,/*BMy3*/ 0);
//Does not like running functions in global area
//----------------------------------------------------------------------------------------------------------------


void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Gyro_Init();
  u8g2.begin();
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  ShipSetup();
  ShipDataDump(Player);

  u8g2.clearBuffer();
  u8g2.drawStr(32, 32, "Calibrating");
  u8g2.drawStr(24, 48, "Takes 5 seconds");
  
  u8g2.sendBuffer();
  
  Gyro_Slow_Cal();
}

void loop() 
{
  switch(Menu)
  {
    case 0:                 //Main Menu
      switch(MainMenu)
        {
        case 0:
          u8g2.clearBuffer();
          u8g2.drawStr(32, 32, "Welcome to the main menu");
          char StringToDraw[50];
          sprintf(StringToDraw, "Cursor value is %i", MenuCursor);
          u8g2.drawStr(24, 48, StringToDraw);
          u8g2.sendBuffer();
          Menu_Navigation();
          delay(100);
        default:
          delay(20);
        }
    default:
      u8g2.clearBuffer();
      DrawShipXBM(Player);
      //u8g2.drawCircle(100, 25, 10, U8G2_DRAW_ALL);
      u8g2.sendBuffer();
      delay(100);
  }
}
