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

int Menu = 0;               // Menu 0 is main menu, menu 1 is game menu, menu 2 is error menu
int MainMenu = 0;           // Menu 0 is the starting menu
int ErrorMenu = 0;          // Menu 0 is an error with no error, menu 1 is no gyroscope error.

bool SlowCalRun = false;    // If this is true, then the program runs the slow calibration process
int SlowCalTime = 0;        // Might be used to display a counter when the slow cal process is working (Will need to redesign how that thing works, if I want to implement this)
int MenuCursor = 0;         // Used to show which thing the cursor is pointing at

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
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
  Gyro_Init();
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  ShipSetup();
  //ShipDataDump(Player);
  Serial.println("Setup ran 1");
  if(ErrorMenu != 3)
  {  
    SlowCalRun = true;
  }
  Serial.println("Setup ran fully");
}

void loop() 
{
  //Serial.println("Loop started running");
  Gyro_Read();
  if(SlowCalRun)
   {
     u8g2.clearBuffer();
     u8g2.drawStr(32, 32, "Calibrating");
     u8g2.drawStr(24, 48, "Takes 5 seconds");
     u8g2.sendBuffer();
     Gyro_Slow_Cal();
     SlowCalRun = false;
   }
  if(Menu == 0)
  {
    //Serial.println("Switch 0");
    if(MainMenu == 0)
    {
      //Serial.println("Switch 00");
      u8g2.clearBuffer();
      u8g2.drawStr(20, 16, "Welcome to the");
      u8g2.drawStr(32, 32, "main menu");
      char StringToDraw[50];
      sprintf(StringToDraw, "Cursor value is %i", MenuCursor);
      u8g2.drawStr(16, 48, StringToDraw);
      u8g2.sendBuffer();
      //printf("gy value is: %f \n", gy); 
      Menu_Navigation();
    }
    delay(100);
  }
  else if(Menu == 1)
  {
    //Serial.println("Switch 2");
    u8g2.clearBuffer();
    DrawShipXBM(Player);
    //u8g2.drawCircle(100, 25, 10, U8G2_DRAW_ALL);
    u8g2.sendBuffer();
    delay(100);
  }
  else if(Menu == 2)
  {
    if(ErrorMenu == 1)        // Gyro error screen has been moved to the init function itself. This might either be useful or be deleted later
    {
      u8g2.clearBuffer();
      u8g2.drawStr(16, 16, "No Gyroscope detected");
      u8g2.drawStr(16, 32, "Please check connectors");
      u8g2.drawStr(16, 48, "And restart the device");
      u8g2.sendBuffer();
      if(Gyroscope.isConnected())
      {
        Menu = 0;
        MainMenu = 0;
        // This will need to be redone later, so that it returns you to the place u were in before it happened
        // If I can't figure that out, then it will just reset the esp when it happens (Or return u to the main menu)
      }
    }
  }
}
