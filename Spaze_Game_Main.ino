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

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI   u8g2  (U8G2_R0, 5, 16, 17);     // Initialize screen
GY521 Gyroscope(0x68);                                                  // Initialize Gyroscope

//int fpsValueLen = 28;     // Used for testing many fps values with the one under it too
//int fpsValues[] = {60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 300, 360, 420, 480, 10, 20, 30, 40, 50};
int fpsValueLen = 4;                              // Length of fps array. Used for cycling in the menu option
int fpsValues[] = {60, 120, 144, 240};            // Fps values to cycle between. This is the one being displayed currently
int fpsDisplayValues[] = {60, 120, 144, 240};     // Real fps values to display, after program delay is taken into account. Will be used when fps is finalized
int fpsValueNum = 0;                              // Decides which element of the array the fps value will take. Changable in Settings
int fps = fpsValues[fpsValueNum];                 // The Fps to run with. It is actually just extra delay in the loop

/*----------------------------------------------------------------------------------
Notes for fps:
- The fps is not fully correct. By the time of writing this 240fps gives 83fps in menus and 67fps ingame
- FPS will be adjusted later when the entire program is done
------------------------------------------------------------------------------------*/

unsigned long LastLoopTime = 0;     // This is used for testing fps. Records the millis() value of when the loop last ran
unsigned long LoopTime = 0;         // This is used for testing fps. Shows the time it takes for the loop to finish. Can also detect unwanted delays.

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
int UseYaw = 0;
int LastMenu = 0;           // Used to know which menu the program has been run from, in case it needs to return to that menu
int LastSubMenu = 0;        // Used to know which sub menu the program has been run from, in case it needs to return to that sub menu

struct Ship
{
  //-----------------------------Inner Data----------------------------------//
  int Type;         //0 for player and 1 for enemy and 2 for asteroid
  int Speed;        //Speed in pixles per second
  int Health;       //Health points
  
  //------------------------------Visual Data--------------------------------//
  int TopLeftCoords[2]; //Coordinates of top left pixel first is x then is y
  int BottomRightCoords[2]; // Coordinates of the bottom right pixle, first is x then is y. This is taken to know how high and wide the ship is
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
  Serial.begin(115200);                     // Begin serial
  Wire.begin();                             // Begin wire library
  u8g2.begin();                             // Start screen
  u8g2.setFont(u8g2_font_ncenB08_tr);       // Choose font
  //printf("1-Menu is %i \n", Menu);
  //printf("1-SubMenu is %i \n", SubMenu);
  Gyro_Init();                              // Run function to start Gyroscope
  pinMode(32, INPUT);                       // Set pin for the left button
  pinMode(33, INPUT);                       // Set pin for the right button
  ShipSetup();                              // Setup all the ships
  //ShipDataDump(Player);
  //Serial.println("Setup ran 1");
  SlowCalRun = true;                        // Let the slow calibrator run at the start of the program
  Serial.println("Setup ran fully");
  //printf("2-Menu is %i \n", Menu);
  //printf("2-SubMenu is %i \n", SubMenu);
}

void loop() 
{
//----------------------For testing fps------------------
  LoopTime = millis() - LastLoopTime;       // Set up LoopTime
  //Serial.print("LoopTime");
  //Serial.println(LoopTime);
  LastLoopTime = millis();                  // Record time, to use in the next LoopTime setup
//-------------------------------------------------------
  
  if(not Gyroscope.isConnected())           // Check if the Gyroscope is connected
  {
    // If not record the last menu that the program was in
    LastMenu = Menu;
    LastSubMenu = SubMenu;
    // Switch to the Gyroscope connection error menu
    Menu = 2;
    SubMenu = 1;
  }
  //Serial.println("Loop started running");
  //printf("3-Menu is %i \n", Menu);
  //printf("3-SubMenu is %i \n", SubMenu);
  Gyro_Read();                              // Read data from the gyroscope
  if(SlowCalRun)                            // Check if the slow calibrator is supposed to run
  {
    Slow_Cal_Screen();                      // Show the calibration screen
    Gyro_Slow_Cal();                        // Run the function. Takes 5 seconds to finish
    SlowCalRun = false;                     // Stop the calibrator
  }
  //printf("4-Menu is %i \n", Menu);
  //printf("4-SubMenu is %i \n", SubMenu);
  if(Menu == 0)                             // If Main Menues
  {
    //Serial.println("Switch 0");
    if(SubMenu == 0)                          // If Main Menu
    {
      //Serial.println("Switch 00");
      //Menu_Navigation_Screen();
      Main_Menu();                              // Show the main menu
      Menu_Navigation();                        // Check for up and down navigation
      if(LB_Press())                            // Check if the left button has been pressed (Calibration button)
      {
        Serial.println("LB_Pressed in main menu");
        SlowCalRun = true;                        // Set it so the slow calibrator runs next time
        delay(500);                               // Wait half a second before the calibrator runs
        return;                                   // Go to the start of the main loop
      }
      else if(RB_Press())                       // Check if the right button has been pressed (Select button)
      {
        Serial.println("RB_Pressed in main menu");
        switch(MenuCursor)                        // Check where was the cursor on
        {
          case 0:                                   // Start Game
            //Serial.println("RB_Pressed case 0");
            // Set the menu to the ingame menu and reset the cursor position
            Menu = 1;
            SubMenu = 0;
            MenuCursor = 0;
            
            Gyro_Fast_Cal();                          // Run the fast calibrator
            //delay(100);
            delay(1000/fps);                          // Run the fps delay
            return;                                   // Go to the start of the main loop
          case 1:                                   // Settings
            //Serial.println("RB_Pressed case 1");
            // Set the menu to the Settings menu and reset the cursor position
            Menu = 0;
            SubMenu = 1;
            MenuCursor = 0;
            
            //delay(100);
            delay(1000/fps);                          // Run the fps delay
            return;                                   // Go to the start of the main loop
          case 2:                                   // Help
            //Serial.println("RB_Pressed case 2");
            // Set the menu to the Help menu and reset the cursor position
            Menu = 0;
            SubMenu = 2;
            MenuCursor = 0;
            
            //delay(100);
            delay(1000/fps);                          // Run the fps delay
            return;                                   // Go to the start of the main loop
          default:                                  // In case the cursor is in a place it is not supposed to be in
            // Set the menu to the unknown error menu and reset the cursor
            Menu = 2;
            SubMenu = 0;
            MenuCursor = 0;
            
            Serial.println("Cursor in a weird position in the main menu"); // Print an error message in the Serial
            //delay(100);
            delay(1000/fps);                          // Run the fps delay
            return;                                   // return to the start of the main loop
        }
      }
    }
    else if(SubMenu == 1)                     // If Settings Menu
    {
      //Serial.println("Menu 0 SubMenu 1");
      Settings_Screen();                        // Display the settings screen
      Menu_Navigation();                        // Enable menu navigation
      
      if(LB_Press())                            // If the left button was pressed (Return)
      {
        SubMenu = 0;                            // Return the sub menu to the main menu values
        //delay(100);
        delay(1000/fps);                        // Run the fps delay
        return;                                 // Return from the start of the main loop
      }
      else if(RB_Press())                       // If the right button was pressed (Select)
      {
        Serial.println("RB_Pressed in settings");
        switch(MenuCursor)                        // Check where the cursor was on
        {
          case 0:                                   // FPS changing setting
            //Serial.println("RB_Pressed settings case 0");
            // Change the fps value
            fpsValueNum = (fpsValueNum + 1) % fpsValueLen;
            fps = fpsValues[fpsValueNum];
            
            //delay(100);
            delay(1000/fps);                          // Run the fps delay
            return;                                   // Return to the start of the main loop
          case 1:                                   // Controls
            //Serial.println("RB_Pressed settings case 1");
            // Not yet made
            //delay(100);
            delay(1000/fps);
            return;
          case 2:                                   // Something
            //Serial.println("RB_Pressed settings case 2");
            // No plans for this yet
            //delay(100);
            delay(1000/fps);
            return;
          default:                                  // In case the cursor is in a place it is not supposed to be in
            // Set the menu to the unknown error menu and reset the cursor
            Menu = 2;
            SubMenu = 0;
            MenuCursor = 0;
            //delay(100);
            delay(1000/fps);                          // Run the fps delay
            return;                                   // Return to the main loop
        }
      }
    }
    else if(SubMenu == 2)                     // Help Menu
    {
      //Serial.println("Menu 0 SubMenu 1");
      Help_Screen();                            // Display the Help menu screen
      Menu_Navigation();                        // Enable menu navigation
      if(LB_Press())                            // If the left button was pressed (Return)
      {
        SubMenu = 0;                              // Go to the main menu
        //delay(100);
        delay(1000/fps);                          // Run the fps delay
        return;                                   // Return to the main loop
      }
      // No right button yet
    }
    else                                      // If the SubMenu value is a weird value, then show the unknown error menu
    {
      Serial.println("The Sub menu value is not a known value. This is from the main menues");
      Menu = 2;
      SubMenu = 0;
      //delay(100);
      delay(1000/fps);
      return;
    }
  }
  else if(Menu == 1)                        // If the Ingame Menues
  {
    // No sub menues have been implemented yet. A possible sub menu is the pause screen
    
    //Serial.println("Switch 2");
    Game_Screen();                              // Run the Game displaying function
    Game_Move_Vert();                           // Check for vertical movement from the gyroscope
    if(Pause_Game())                            // Check if the pause movement has been triggered
    {
      // No pause menu has been made yet, so the program returns to the main menu
      Serial.println("Game has been paused. Returning to main menu");
      // Set the values back to the main menu
      Menu = 0;
      SubMenu = 0;
      
      ShipSetup();                                // Reset the sprites position
      //delay(100);
      delay(1000/fps);
      return;
    }
    if(UseYaw)                                  // Check if you are supposed to use Yaw (Steering wheel mode) for horizontal movement
    {
      Game_Move_Yaw_Hor();                        // Check for horizontal movement using Yaw rotation
    }
    else
    {
      Game_Move_Roll_Hor();                       // Check for horizontal movement using Roll rotation
    }
    if(LB_Press())                              // Check if the left button has been pressed
    {
      Serial.println("LB_Pressed in game");
      Gyro_Fast_Cal();                            // Run the fast callibrator
      // Paint a circle for a bit of feedback. Only for debuging
      u8g2.drawCircle(100, 25, 10, U8G2_DRAW_ALL);
      u8g2.sendBuffer();
      //return;
    }
    else if(RB_Press())                         // Check if the right button has been pressed
    {
      Serial.println("RB_Pressed in game");
      UseYaw = (UseYaw + 1) % 2;                  // Switch to Yaw mode (This is not the final action. This button had nothing to do so this was added to it. Horizontal controlls will be changed in the settings at the end)
    }
    //delay(100);
    delay(1000/fps);                            // Run the fps delay
  }
  else if(Menu == 2)                          // Error menues
  {
    if(SubMenu == 0)                            // Unkown Error
    {
      Unknown_Error_Screen();                     // Display the unknown error screen. Only way to exit it is to restart the program
      //delay(100);
      delay(1000/fps);                            // Run the fps delay
    }
    if(SubMenu == 1)                            // Gyro error screen is stuck in a while loop until the gyro is reconnected
    {
      Gyro_Error_Screen();                        // Display the gyro error screen (The loop is inside)
      //delay(100);
      delay(1000/fps);                            // Run the fps delay
    }
  }
}
