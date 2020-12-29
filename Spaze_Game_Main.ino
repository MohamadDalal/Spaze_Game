#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>
#include "GY521.h"

/*------------------------------------Notes--------------------------------------------------------------------------------
The comments on the buttons show their functionalities when not inversed
As the inversion effects the pin reading instead of the button press function reading

The if else if checks for the menues can probably be made into switches, but I am already too deep in

-------------------------------------------------------------------------------------------------------------------------*/

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

//int fpsValueLen = 28;                                   // Used for testing many fps values with the one under it too
//int fpsValues[] = {60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 300, 360, 420, 480, 10, 20, 30, 40, 50};
int fpsValueLen = 4;                                    // Length of fps array. Used for cycling in the menu option
int fpsValues[] = {60, 120, 144, 2000};            // Fps values to cycle between. This is the one being displayed currently
char fpsDisplayValues[][4] = {"60", "120", "144", "inf"};     // This is what the menu shows, it is so it shows infinity instead of 2000 (Because 2000 is no delay)
int fpsValueNum = 0;                                    // Decides which element of the array the fps value will take. Changable in Settings
int fpsMax = fpsValues[fpsValueNum];                       // The Fps to run with. It is actually just extra delay in the loop
int fpsDelay = 0;                                       // Delay to be added to the loop in order to hold as close to the max fps as possible
int LastfpsDelay = 0;                                   // Used to store the fps delay in the last loop. Used to fine tune the fps delay

/*----------------------------------------------------------------------------------
Notes for fps:
- The fps is not fully correct. By the time of writing this 240fps gives 83fps in menus and 67fps ingame
- FPS will be adjusted later when the entire program is done
Ignore above
- FPS must be finished
- FPS was set into Max FPS, where the program puts a delay to hold the max fps, if the programs fps is lower than the max fps, then no delay is added.
------------------------------------------------------------------------------------*/

unsigned long LastLoopTime = 0;     // This is used for testing fps. Records the millis() value of when the loop last ran
unsigned long LoopTime = 0;         // This is used for testing fps. Shows the time it takes for the loop to finish. Can also detect unwanted delays.
unsigned long GameTime = 0;         // The time that the game has ran, used to display the time at the middle of the HUD

int Menu = 0;               // Menu 0 is main menu, menu 1 is game menu, menu 2 is error menu
// The two under have been replaced by the SubMenu
//int MainMenu = 0;         // Menu 0 is the starting menu
//int ErrorMenu = 0;        // Menu 0 is an error with no error, menu 1 is no gyroscope error.
int SubMenu = 0;            // SubMenu for the Menues, it all depends on the Main Menu value
int Page = 0;               // The page of multi paged menues
bool FromPause = false;     // Checks if this menu has been accessed from the pause menu and not the main menu (This is on when the game is paused and off when in the main menu)
/*--------------------------------Sub Menus------------------------------------------------------
      Menu 0                        Menu 1                      Menu 2
SubMenu 0: Starting Menu      Submenu 0: Game Menu        SubMenu 0: No Error Menu
SubMenu 1: Settings Menu      Submenu 1: Pause Menu       SubMenu 1: No Gyro Menu
SubMenu 2: Help Menu          Submenu 2: Game Over
SubMenu 3: Controlls Menu     Submenu 3: Score&Stats

Suggestion to make the submenus all have different numbers, so you don't get accidental errors
-----------------------------------------------------------------------------------------------*/
bool SlowCalRun = false;    // If this is true, then the program runs the slow calibration process
int SlowCalTime = 0;        // Might be used to display a counter when the slow cal process is working (Will need to redesign how that thing works, if I want to implement this)
int MenuCursor = 0;         // Used to show which thing the cursor is pointing at
bool UseYaw = false;        // This setting decides if the horizontal movement will use Yaw or Roll angles
bool InvertButtons = false; // Inverts the functions of the LB and the RB. Used in the controls function, where the pin it reads from has been inverted. Also used in the menu display functions to change what is written at the bottom
bool CorruptShip = false;   // Invert the two ship bitmaps :P
unsigned long FastCalPressTime;   // When the fast cal is pressed make this record the time it has been pressed, so it can show something for an amount of time for feedback
int LastMenu = 0;           // Used to know which menu the program has been run from, in case it needs to return to that menu
int LastSubMenu = 0;        // Used to know which sub menu the program has been run from, in case it needs to return to that sub menu

int BigLaser = 0;               // Delete later, only used for testing
int Weapon = 0;

void setup()
{
  Serial.begin(115200);                     // Begin serial
  Wire.begin();                             // Begin wire library
  u8g2.begin();                             // Start screen
  u8g2.setFont(u8g2_font_ncenB08_tr);       // Choose font
  u8g2.setBitmapMode(1);                    // Bitmaps go over each other
  //printf("1-Menu is %i \n", Menu);
  //printf("1-SubMenu is %i \n", SubMenu);
  Gyro_Init();                              // Run function to start Gyroscope
  pinMode(34, INPUT);                       // Set pin for the left button
  pinMode(35, INPUT);                       // Set pin for the right button
  //ShipSetup();                              // Setup all the ships. Now it setups the ship when the game actually starts instead in the program setup
  //ShipDataDump(Player);                     // This is unusuable after I have returned the ship struct to its rightfull place. Will find another way to use this later
  //Serial.println("Setup ran 1");
  SlowCalRun = true;                        // Let the slow calibrator run at the start of the program
  Serial.println("Setup ran fully");
  //printf("2-Menu is %i \n", Menu);
  //printf("2-SubMenu is %i \n", SubMenu);
}

void loop() 
{
//----------------------FPS Stuff------------------------
  LoopTime = millis() - LastLoopTime;       // Set up LoopTime
  //Serial.print("LoopTime ");
  //Serial.println(LoopTime);
  //Serial.print ("FPS ");
  //Serial.println(1000 / LoopTime);
  LastLoopTime = millis();                  // Record time, to use in the next LoopTime setup
  fpsDelay = LastfpsDelay + ((1000 / fpsMax) - LoopTime);   // Adjust the delay so that it makes the programs fps closer to the max fps
  //Serial.print ("FPS Delay ");
  //Serial.println(fpsDelay);
  if (fpsDelay < 0)                         // If the delay needed is negative (The fps is lower than the max fps)
  {
    fpsDelay = 0;                           // Add no delay
    LastfpsDelay = 0;                       // Update the last fps delay
  }
  else                                      // Otherwise if a delay is needed
  {
    LastfpsDelay = fpsDelay;                // Update the last fps delay with that delay
  }
//-------------------------------------------------------
  
  if(not Gyroscope.isConnected())           // Check if the Gyroscope is connected
  {
    // If not record the last menu that the program was in
    LastMenu = Menu;                        // Set the last menu so it return to the right menu when the gyro is connected again
    LastSubMenu = SubMenu;                  // Set the last submenu so it return to the right submenu when the gyro is connected again
    // Switch to the Gyroscope connection error menu
    Menu = 2;                               // Go to the errors menu
    SubMenu = 1;                            // Go to the gyroscope connection error submenu
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
    if(SubMenu == 0)                          // If the Main Menu
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
            
            ShipSetup();                              // Setup all the ships
            GameTime = 0;                             // Reset the game time to 0 (This is also done when the game is exited from the pause menu, but it is done here again just to be safe)
            
            Gyro_Fast_Cal();                          // Run the fast calibrator
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Go to the start of the main loop
          case 1:                                   // Settings
            //Serial.println("RB_Pressed case 1");
            // Set the menu to the Settings menu and reset the cursor position
            Menu = 0;
            SubMenu = 1;
            MenuCursor = 0;
            
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Go to the start of the main loop
          case 2:                                   // Help
            //Serial.println("RB_Pressed case 2");
            // Set the menu to the Help menu and reset the cursor position
            Menu = 0;
            SubMenu = 2;
            MenuCursor = 0;
            
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Go to the start of the main loop
          default:                                  // In case the cursor is in a place it is not supposed to be in
            // Set the menu to the unknown error menu and reset the cursor
            Menu = 2;
            SubMenu = 0;
            MenuCursor = 0;
            
            Serial.println("Cursor in a weird position in the main menu"); // Print an error message in the Serial
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
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
        if(FromPause)                             // If this menu has been accessed from the pause menu
        {
          // Set the menu to the ingame pause menu and reset cursor
          Menu = 1;
          SubMenu = 1;
          MenuCursor = 0;
          
          delay(fpsDelay);                          // Apply the fps delay
          return;                                   // Return to the start of the main loop
        }
        else                                      // If it is accessed from the main menu
        {
          // Set the menu to the main menu and reset cursor
          Menu = 0;
          SubMenu = 0;
          MenuCursor = 1;
          
          //delay(100);
          delay(fpsDelay);                          // Run the fps delay
          return;                                   // Return from the start of the main loop
        }
      }
      else if(RB_Press())                       // If the right button was pressed (Select)
      {
        Serial.println("RB_Pressed in settings");
        switch(MenuCursor)                        // Check where the cursor was on
        {
          case 0:                                   // FPS changing setting
            //Serial.println("RB_Pressed settings case 0");
            // Change the fps value
            fpsValueNum = (fpsValueNum + 1) % fpsValueLen;    // Change the max fps to the next fps setting
            fpsMax = fpsValues[fpsValueNum];                  // Apply the change
            
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Return to the start of the main loop
          case 1:                                   // Controls
            //Serial.println("RB_Pressed settings case 1");
            Menu = 0;
            SubMenu = 3;
            MenuCursor = 0;
            //delay(100);
            delay(fpsDelay);
            return;
          case 2:                                   // Difficulty or Fast Calibration
            //Serial.println("RB_Pressed settings case 2");
            // If I ever have time to implement this, then I will write something here
            if(FromPause)                             // If the menu was accessed from the pause menu
            {
              Gyro_Fast_Cal();                          // Run the fast callibratior
              FastCalPressTime = millis();              // Set the time the calibrator was run
            }
            /*else                                    // Else nothing because difficulty is not changeable yet
            {
              
            }*/
            //delay(100);
            delay(fpsDelay);                        // Apply the fps delay
            return;                                 // return to the start of the main loop
          default:                                // In case the cursor is in a place it is not supposed to be in
            // Set the menu to the unknown error menu and reset the cursor
            Menu = 2;
            SubMenu = 0;
            MenuCursor = 0;
            //delay(100);
            delay(fpsDelay);                        // Run the fps delay
            return;                                 // Return to the main loop
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
        if(FromPause)                             // If this was accessed from the pause menu
        {
          // Set the menu to the ingame pause menu and reset the cursor
          Menu = 1;
          SubMenu = 1;
          MenuCursor = 1;
          
          delay(fpsDelay);                          // Apply the fps delay
          return;                                   // Return to the start of the loop
        }
        else                                      // If it was accessed from the main menu
        {
          // Set the menu value to the main menu and reset the cursor position
          Menu = 0;
          SubMenu = 0;
          MenuCursor = 2;
          
          //delay(100);
          delay(fpsDelay);                          // Run the fps delay
          return;                                   // Return to the main loop
        }
      }
      // No right button yet
    }
    else if(SubMenu == 3)                   // Controls Menu
    {
      Settings_Controls_Menu();
      Menu_Navigation();
      if(LB_Press())                            // If the left button was pressed (Return)
      {
        // Set the menu to the settings menu and reset the cursor
        Menu = 0;
        SubMenu = 1;
        MenuCursor = 1;
        
        //delay(100);
        delay(fpsDelay);                        // Run the fps delay
        return;                                 // Return from the start of the main loop
      }
      else if(RB_Press())                       // If the right button was pressed (Select)
      {
        Serial.println("RB_Pressed in controlls");
        switch(MenuCursor)                        // Check where the cursor was on
        {
          case 0:                                   // Steerwheel mode
            //Serial.println("RB_Pressed controls case 0");
            // Change to steerwheel mode (UseYaw)
            UseYaw = !UseYaw;
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Return to the start of the main loop
          case 1:                                   // LB<-->RB Invert button functions
            //Serial.println("RB_Pressed controls case 1");
            // Invert the functions of the left and right buttons
            InvertButtons = !InvertButtons;
            delay(250);                               // Add an extra 250ms delay, because the millis check does not work with the buttons at the exact time they inverted
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Return to the start of the main loop
          case 2:                                   // Corrupted ship mode
            //Serial.println("RB_Pressed controls case 2");
            // Corrupt the ship by inverting the two bitmaps
            if(FromPause)                             // If this option was triggered, when the settings menu was accessed from the pause menu
            {
              CorruptShip = CorruptShip;                // Do not change the corruption (There is no turning back)
            }
            else                                      // If this option was triggered, when the settings menu was accessed from the main menu
            {
              CorruptShip = !CorruptShip;               // Invert the corruption option
            }
            //delay(100);
            delay(fpsDelay);                          // Apply the fps delay
            return;                                   // Return to the start of the main loop
          default:                                  // In case the cursor is in a place it is not supposed to be in
            // Set the menu to the unknown error menu and reset the cursor
            Menu = 2;
            SubMenu = 0;
            MenuCursor = 0;
            
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Return to the main loop
        }
      }
    }
    else                                      // If the SubMenu value is a weird value, then show the unknown error menu
    {
      Serial.println("The Sub menu value is not a known value. This is from the main menues");
      Menu = 2;
      SubMenu = 0;
      
      //delay(100);
      delay(fpsDelay);
      return;
    }
    delay(fpsDelay);                          // Run the fps delay for
  }
  else if(Menu == 1)                        // If the Ingame Menues
  {
    // No sub menues have been implemented yet. A possible sub menu is the pause screen
    //Serial.println("Switch 2");
    if(SubMenu == 0)                          // Ingame
    {
      GameTime += LoopTime;                     // Calculate the time that the game session has been running
      Detect_Hit();
      Check_Invincible();
      u8g2.clearBuffer();                       // Clear the buffer before sending the visuals
      Game_HUD();                               // Run the HUD displaying function
      Game_Screen();                            // Run the Game displaying function.
      u8g2.sendBuffer();                        // Only one sendBuffer() must be used per loop, as that thing lags like hell. Therefore, the game screen sends after everything was fully drawn
      Game_Move_Vert();                         // Check for vertical movement from the gyroscope
      if(Pause_Game())                          // Check if the pause movement has been triggered (Heavy shake)
      {
        // No pause menu has been made yet, so the program returns to the main menu
        Serial.println("Game has been paused.");
        // Set the values back to the main menu and reset the cursor
        Menu = 1;
        SubMenu = 1;
        MenuCursor = 0;

        Weapon = (Weapon + 1) % 3;
        
        //ShipSetup();                              // Reset the sprites position. No longer needed after making a proper pause menu
        //delay(100);
        delay(fpsDelay);                            // Run the fps delay
        return;                                     // Return to the start of the main loop
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
        FastCalPressTime = millis();                // Set the time that the fast callibrator has been triggered
        //u8g2.drawCircle(100, 25, 10, U8G2_DRAW_ALL); // Paint a circle for a bit of feedback. Only for debuging
        //u8g2.sendBuffer();
        //return;
      }
      else if(RB_Press())                         // Check if the right button has been pressed. Everything here is for testing, the right button will be used to pause the game
      {
        Serial.println("RB_Pressed in game");
        switch(Weapon)
        {
          case 0:                                       // Fire Laser
            //Activate_Laser(0, 32, 16, 4000);
            BigLaser = (BigLaser + 1) % 2;              // Change the value for firing a big laser
            Serial.print("BigLaser ");
            Serial.println(BigLaser);
            Activate_Laser_Random(BigLaser);            // Fire a laser with random settings
            break;
          case 1:                                       // Make explosion
            Serial.println("Megumin says: EXUPLOSION");
            Activate_Explosion_Random();
            break;
          case 2:
            Serial.println("How about a moving laser?");
            Activate_VertLaser_Random();
            break;
          default:
            // Set the menu to the unknown error menu and reset the cursor
            Menu = 2;
            SubMenu = 0;
            MenuCursor = 0;
            
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Return to the main loop  
        }
      }
    }
    else if(SubMenu == 1)                       //Pause Menu
    {
      Pause_Menu();                               // Display the pause menu
      Menu_Navigation();                          // Detect menu navigation
      if(LB_Press())                              // If left button is pressed (Back)
      {
        // Set the ingame menu and reset the cursor position
        Menu = 1;
        SubMenu = 0;
        MenuCursor = 0;
      }
      else if(RB_Press())                         // If the right button is pressed (Select)
      {
        Serial.println("RB_Pressed in pause menu");
        switch(MenuCursor)                        // Check where the cursor was on
        {
          case 0:                                   // Settings
            //Serial.println("RB_Pressed pause case 0");
            // Switch to the settings menu and reset cursor
            Menu = 0;
            SubMenu = 1;
            MenuCursor = 0;
            
            FromPause = true;                         // Tell the program that the menu has been accessed from the pause menu
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Return to the start of the main loop
          case 1:                                   // Help
            //Serial.println("RB_Pressed pause case 1");
            // Set the menu to the help menu and reset the cursor
            Menu = 0;
            SubMenu = 2;
            MenuCursor = 0;
            
            FromPause = true;                         // Tell the program that the menu has been accessed from the pause menu
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Return to the start of the main loop
          case 2:                                   // Main menu
            //Serial.println("RB_Pressed pause case 2");
            // Set the menu to the main menu and reset the cursor
            Menu = 0;
            SubMenu = 0;
            MenuCursor = 0;
            
            Reset_Game();                             // Reset the game varaibles
            FromPause = false;                        // Reset the value, as we are now in the main menu
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Return to the start of the main loop
          default:                                  // In case the cursor is in a place it is not supposed to be in
            // Set the menu to the unknown error menu and reset the cursor
            Menu = 2;
            SubMenu = 0;
            MenuCursor = 0;
            
            //delay(100);
            delay(fpsDelay);                          // Run the fps delay
            return;                                   // Return to the main loop
        }
      }
    }
    else                                        // The submenu is an unknown value
    {
      Serial.println("The Sub menu value is not a known value. This is from the ingame menues");
      // Run the unknown error menu
      Menu = 2;
      SubMenu = 0;
      
      //delay(100);
      delay(fpsDelay);                            // Run the fps delay
      return;                                     // Return to the start of the main loop
    }
    //delay(100);
    delay(fpsDelay);                            // Run the fps delay
  }
  else if(Menu == 2)                          // Error menues
  {
    if(SubMenu == 0)                            // Unkown Error
    {
      Unknown_Error_Screen();                     // Display the unknown error screen. Only way to exit it is to restart the program
      //delay(100);
      delay(fpsDelay);                            // Run the fps delay
    }
    if(SubMenu == 1)                            // Gyro error screen is stuck in a while loop until the gyro is reconnected
    {
      Gyro_Error_Screen();                        // Display the gyro error screen (The loop is inside)
      //delay(100);
      delay(fpsDelay);                            // Run the fps delay
    }
  }
}
