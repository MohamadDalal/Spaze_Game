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
// The two under have been replaced by the SubMenu
//int MainMenu = 0;         // Menu 0 is the starting menu
//int ErrorMenu = 0;        // Menu 0 is an error with no error, menu 1 is no gyroscope error.
int SubMenu = 0;            // SubMenu for the Menues, it all depends on the Main Menu value
/*--------------------------------Sub Menus--------------------------------
       Menu 0                       Menu 2
SubMenu 0: Starting Menu      SubMenu 0: No Error Menu
SubMenu 1: Controll Menu      SubMenu 1: No Gyro Menu
SubMenu 2: Highscores Menu    SubMenu 2: Nothing Yet

Suggestion to make the submenus all have different numbers, so you don't get accidental errors
-------------------------------------------------------------------------*/
bool SlowCalRun = false;    // If this is true, then the program runs the slow calibration process
int SlowCalTime = 0;        // Might be used to display a counter when the slow cal process is working (Will need to redesign how that thing works, if I want to implement this)
int MenuCursor = 0;         // Used to show which thing the cursor is pointing at
int LastMenu = 0;           // Used to know which menu the program has been run from, in case it needs to return to that menu
int LastSubMenu = 0;        // Used to know which sub menu the program has been run from, in case it needs to return to that sub menu

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
  printf("1-Menu is %i \n", Menu);
  printf("1-SubMenu is %i \n", SubMenu);
  Gyro_Init();
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  ShipSetup();
  //ShipDataDump(Player);
  Serial.println("Setup ran 1");
  SlowCalRun = true;
  Serial.println("Setup ran fully");
  printf("2-Menu is %i \n", Menu);
  printf("2-SubMenu is %i \n", SubMenu);
}

void loop() 
{
  if(not Gyroscope.isConnected())
  {
    LastMenu = Menu;
    LastSubMenu = SubMenu;
    Menu = 2;
    SubMenu = 1;
  }
  //Serial.println("Loop started running");
  //printf("3-Menu is %i \n", Menu);
  //printf("3-SubMenu is %i \n", SubMenu);
  Gyro_Read();
  if(SlowCalRun)
  {
    Slow_Cal_Screen();
    Gyro_Slow_Cal();
    SlowCalRun = false;
  }
  //printf("4-Menu is %i \n", Menu);
  //printf("4-SubMenu is %i \n", SubMenu);
  if(Menu == 0)
  {
    //Serial.println("Switch 0");
    if(SubMenu == 0)
    {
      //Serial.println("Switch 00");
      //Menu_Navigation_Screen();
      Main_Menu();
      Menu_Navigation();
      if(LB_Press())
      {
        Serial.println("LB_Pressed in main menu");
        SlowCalRun = true;
        delay(500);
        return;
      }
      else if(RB_Press())
      {
        Serial.println("RB_Pressed in main menu");
        switch(MenuCursor)
        {
          case 0:
            //Serial.println("RB_Pressed case 0");
            Menu = 1;
            SubMenu = 0;
            MenuCursor = 0;
            delay(100);
            return;
          case 1:
            //Serial.println("RB_Pressed case 1");
            Menu = 0;
            SubMenu = 1;
            MenuCursor = 0;
            delay(100);
            return;
          case 2:
            //Serial.println("RB_Pressed case 2");
            Menu = 0;
            SubMenu = 2;
            MenuCursor = 0;
            delay(100);
            return;
          default:
            Menu = 2;
            SubMenu = 0;
            MenuCursor = 0;
            delay(100);
            return;
        }
      }
    }
    else if(SubMenu == 1)
    {
      //Serial.println("Menu 0 SubMenu 1");
      Help_Screen();
      Menu_Navigation();
      if(LB_Press())
      {
        SubMenu = 0;
        delay(100);
        return;
      }
    }
    else if(SubMenu == 2)
    {
      //Serial.println("Menu 0 SubMenu 1");
      Coming_Soon();
      if(LB_Press())
      {
        SubMenu = 0;
        delay(100);
        return;
      }
    }
    else
    {
      Menu = 2;
      SubMenu = 0;
    }
    delay(100);
  }
  else if(Menu == 1)
  {
    //Serial.println("Switch 2");
    Game_Screen();
    delay(100);
  }
  else if(Menu == 2)
  {
    if(SubMenu == 0)
    {
      Unknown_Error_Screen();
      delay(100);
    }
    if(SubMenu == 1)        // Gyro error screen is stuck in a while loop until the gyro is reconnected
    {
      Gyro_Error_Screen();
    }
  }
}
