// All menues that can be navigated have the following
// Three option to choose from and a cursor to point on the selected option
// Navigation instructions at the bottom of the screen

// Page menues will have different pages to display according to the page number
// They will also have navigation instructions


// The main menu displaying function
void Main_Menu()
{
  u8g2.clearBuffer();
  // These are the three selection options
  u8g2.drawStr(32, 10, "Start Game");
  u8g2.drawStr(32, 26, "Settings");
  u8g2.drawStr(32, 42, "Help");
  // This is the cursor that moves around with the help of Menu_Navigation()
  u8g2.drawStr(8, (MenuCursor * 16 + 10), "-->");
  u8g2.setFont(u8g2_font_p01type_tr);                 // Set to a small font for the button help thingeys
  if(InvertButtons)                                   // If the LB and RB buttons are inverted show this
  {
    // These are the navigation instructions at the bottom when the button inversion is on
    u8g2.drawStr(2, 56, "LB:SELECT");
    u8g2.drawStr(50, 56, "TILT");
    u8g2.drawStr(74, 56, "RB:CALIBRATE");
    u8g2.setFontDirection(1);
    u8g2.drawStr(56, 46, "<");
    u8g2.drawStr(56, 59, ">");
    u8g2.setFontDirection(0);
  }
  else                                                // Otherwise show this
  {
    // These are the navigation instructions at the bottom when the button inversion is off
    u8g2.drawStr(2, 56, "LB:CALIBRATE");
    u8g2.drawStr(62, 56, "TILT");
    u8g2.drawStr(84, 56, "RB:SELECT");
    u8g2.setFontDirection(1);
    u8g2.drawStr(68, 46, "<");
    u8g2.drawStr(68, 59, ">");
    u8g2.setFontDirection(0);
  }
  u8g2.setFont(u8g2_font_ncenB08_tr);                 // Return to the default font
  u8g2.sendBuffer();
  //delay(10);
}

// The slow calibrator displaying fuction
void Slow_Cal_Screen()
{
  u8g2.clearBuffer();
  u8g2.drawStr(32, 24, "Calibrating");
  u8g2.drawStr(24, 40, "Takes 5 seconds");
  u8g2.sendBuffer();
}

// The help screen displaying function
void Help_Screen()
{
  u8g2.clearBuffer();
  u8g2.drawStr(32, 8, "Basics");
  u8g2.drawStr(32, 24, "Controls");
  u8g2.drawStr(32, 40, "PowerUps");
  u8g2.drawStr(8, (MenuCursor * 16 + 8), "-->");
  u8g2.setFont(u8g2_font_p01type_tr);
  if(InvertButtons)                                   // If the LB and RB buttons are inverted show this
  {
    u8g2.drawStr(2, 56, "LB:SELECT");
    u8g2.drawStr(50, 56, "TILT");
    u8g2.drawStr(95, 56, "RB:BACK");
    u8g2.setFontDirection(1);
    u8g2.drawStr(56, 46, "<");
    u8g2.drawStr(56, 59, ">");
    u8g2.setFontDirection(0);
  }
  else                                                // Otherwise show this
  {
    u8g2.drawStr(2, 56, "LB:BACK");
    u8g2.drawStr(50, 56, "TILT");
    u8g2.drawStr(84, 56, "RB:SELECT");
    u8g2.setFontDirection(1);
    u8g2.drawStr(56, 46, "<");
    u8g2.drawStr(56, 59, ">");
    u8g2.setFontDirection(0);
  }
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.sendBuffer();
}

// The settings displaying function
void Settings_Screen()
{
  u8g2.clearBuffer();
  char fpsToDraw[14];
  sprintf(fpsToDraw, "Max FPS  %s", fpsDisplayValues[fpsValueNum]);
  u8g2.drawStr(32, 8, fpsToDraw);
  u8g2.drawStr(32, 24, "Controls");
  if(FromPause)                                       // If this menu was accessed from the pause menu show this
  {
    if((millis() - FastCalPressTime) < 500)           // If the fast callibrator ran in the last half a second show Done (Pause menu edition)
    {
      u8g2.drawStr(32, 40, "Done");
    }
    else                                              // Otherwise show the option (Pause menu edition)
    {
      u8g2.drawStr(32, 40, "Fast Calibration");
    }
  }
  else                                                // Otherwise show the difficulty setting (does not change yet)
  {
    u8g2.drawStr(32, 40, "Difficulty  ???");
  }
  u8g2.drawStr(8, (MenuCursor * 16 + 8), "-->");
  u8g2.setFont(u8g2_font_p01type_tr);
  if(InvertButtons)                                   // If the LB and RB buttons are inverted show this
  {
    u8g2.drawStr(2, 56, "LB:SELECT");
    u8g2.drawStr(50, 56, "TILT");
    u8g2.drawStr(95, 56, "RB:BACK");
    u8g2.setFontDirection(1);
    u8g2.drawStr(56, 46, "<");
    u8g2.drawStr(56, 59, ">");
    u8g2.setFontDirection(0);
  }
  else                                                // Otherwise show this
  {
    u8g2.drawStr(2, 56, "LB:BACK");
    u8g2.drawStr(50, 56, "TILT");
    u8g2.drawStr(84, 56, "RB:SELECT");
    u8g2.setFontDirection(1);
    u8g2.drawStr(56, 46, "<");
    u8g2.drawStr(56, 59, ">");
    u8g2.setFontDirection(0);
  }
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.sendBuffer();
}

// The controls menu
void Settings_Controls_Menu()
{
  u8g2.clearBuffer();
  char StrToDraw[20];
  if(UseYaw)                                          // If the Steerwheel setting is on then show ON
  {
    u8g2.drawStr(32, 8, "Steerwheel  ON");
  }
  else                                                // Otherwise show OFF
  {
    u8g2.drawStr(32, 8, "Steerwheel  OFF");
  }
  if(InvertButtons)                                   // If buttons invertions setting is on then show ON
  {
    u8g2.drawStr(32, 24, "LB<-->RB  ON");
  }
  else                                                // Otherwise show OFF
  {
    u8g2.drawStr(32, 24, "LB<-->RB  OFF");
  }
  if(FromPause)                                       // If this menu was accessed from the pause menu show this
  {
    if(CorruptShip)                                   // If the corrupt ship setting is on then show this (Pause screen edition)
    {
      u8g2.drawStr(32, 40, "CORRUPTION!!!");
    }
    else                                              // Otherwise show this (Pause screen edition)
    {
      u8g2.drawStr(32, 40, "Nothing here");
    }
  }
  else
  {
    if(CorruptShip)                                   // If the corrupt ship setting is on then abide by the players insanity and show YES
    {
      u8g2.drawStr(32, 40, "Corrupted?  YES");
    }
    else                                              // Otherwise answer the players sanity by showin NO!
    {
      u8g2.drawStr(32, 40, "Corrupted?  NO!");
    }
  }
  u8g2.drawStr(8, (MenuCursor * 16 + 8), "-->");
  u8g2.setFont(u8g2_font_p01type_tr);
  if(InvertButtons)                                   // If the LB and RB buttons are inverted show this
  {
    u8g2.drawStr(2, 56, "LB:SELECT");
    u8g2.drawStr(50, 56, "TILT");
    u8g2.drawStr(95, 56, "RB:BACK");
    u8g2.setFontDirection(1);
    u8g2.drawStr(56, 46, "<");
    u8g2.drawStr(56, 59, ">");
    u8g2.setFontDirection(0);
  }
  else                                                // Otherwise show this
  {
    u8g2.drawStr(2, 56, "LB:BACK");
    u8g2.drawStr(50, 56, "TILT");
    u8g2.drawStr(84, 56, "RB:SELECT");
    u8g2.setFontDirection(1);
    u8g2.drawStr(56, 46, "<");
    u8g2.drawStr(56, 59, ">");
    u8g2.setFontDirection(0);
  }
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.sendBuffer();
}

// A function to be displayed if I add a selection just to fill place
void Coming_Soon()
{
  u8g2.clearBuffer();
  u8g2.drawStr(26, 32, "Coming Soon");
  u8g2.setFont(u8g2_font_p01type_tr);
  if(InvertButtons)                                   // If the LB and RB buttons are inverted show this
  {
    u8g2.drawStr(2, 56, "RB:BACK");
  }
  else                                                // Otherwise show this
  {
    u8g2.drawStr(2, 56, "LB:BACK");
  }
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.sendBuffer();
}

// Function that displays the HUD with the correct values
void Game_HUD()
{
  u8g2.setFontDirection(3);                           // Make the writing direction bottom to top
  char StrToDraw[7];
  if((millis() - FastCalPressTime) < 500)             // Check if the fast cal has been activated in the last half second
  {
    u8g2.drawStr(10, 63, "C");                        // If yes show C in place of your health for feedback
  }
  else                                                
  {
    sprintf(StrToDraw, "%i", Player.Health);
    u8g2.drawStr(10, 63, StrToDraw);                  // Otherwise show your hitpoints at the bottom right
  }
  sprintf(StrToDraw, "%i", (1000 / LoopTime));        // Calculate the current fps
  u8g2.drawStr(10, 10, StrToDraw);                    // Draw the fps at the top right
  sprintf(StrToDraw, "%i:%i", ((GameTime / 60000) % 60), ((GameTime / 1000) % 60));       // Calculate the running time and write it in a Minutes:Seconds format
  int Width;                                          // This is filled with the width of the time
  Width  = u8g2.getStrWidth(StrToDraw);               // Get the width of the time string
  u8g2.drawStr(10, (31 + (Width / 2)), StrToDraw);    // Draw the time in the middle right of the screen
  u8g2.setFontDirection(0);                           // Reset the writing direction to left to right
  u8g2.drawLine(12, 0, 12, 63);                       // Draw a line to denote the left boundary
  u8g2.drawLine(121, 0, 121, 63);                     // Draw a line to denote the right boundary
  //u8g2.sendBuffer();
  // The left side has 12 pixles to write numbers vertically in, so a vertical line will be drawn at 12 to seperate them
  // The right side will either have 6 or 8 pixels of space, so a verical line will be drawn at either 121 or 119
  return;
}

// The actual game displaying function. If I acutally make a proper game this will become biggus
void Game_Screen()
{
  //u8g2.clearBuffer();
  Display_Laser();                                    // Check if a laser needs to be drawn and draw it
  DrawShipXBM(Player);                                // Draw the player's ship
  //u8g2.drawCircle(100, 25, 10, U8G2_DRAW_ALL);
  //u8g2.sendBuffer();
  //delay(100);
}

void Pause_Menu()
{
  u8g2.clearBuffer();
  u8g2.drawStr(32, 10, "Settings");
  u8g2.drawStr(32, 26, "Help");
  u8g2.drawStr(32, 42, "Main Menu");
  u8g2.drawStr(8, (MenuCursor * 16 + 10), "-->");
  u8g2.setFont(u8g2_font_p01type_tr);
  if(InvertButtons)                                   // If the LB and RB buttons are inverted show this
  {
    u8g2.drawStr(2, 56, "LB:SELECT");
    u8g2.drawStr(50, 56, "TILT");
    u8g2.drawStr(95, 56, "RB:BACK");
    u8g2.setFontDirection(1);
    u8g2.drawStr(56, 46, "<");
    u8g2.drawStr(56, 59, ">");
    u8g2.setFontDirection(0);
  }
  else                                                // Otherwise show this
  {
    u8g2.drawStr(2, 56, "LB:BACK");
    u8g2.drawStr(50, 56, "TILT");
    u8g2.drawStr(84, 56, "RB:SELECT");
    u8g2.setFontDirection(1);
    u8g2.drawStr(56, 46, "<");
    u8g2.drawStr(56, 59, ">");
    u8g2.setFontDirection(0);
  }
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.sendBuffer();
  //delay(10);
}

// The unknown error displaying function
void Unknown_Error_Screen()
{
  u8g2.clearBuffer();
  u8g2.drawStr(4, 16, "An unknown error has");
  u8g2.drawStr(40, 32, "occured");
  u8g2.drawStr(24, 48, "Please restart");
  u8g2.sendBuffer();
}

// The disconnected Gyro displaying function
void Gyro_Error_Screen()
{
  u8g2.clearBuffer();
  u8g2.drawStr(0, 16, "No Gyroscope detected");
  u8g2.drawStr(8, 32, "Please check that all");
  u8g2.drawStr(8, 48, "wires are connected");
  u8g2.sendBuffer();
  while(not Gyroscope.isConnected())            // Don't leave this screen until the gyroscope has been connected again
  {
    delay(100);
  }
  // Return to the last menu that the program was on before this. Suggestion: When this is triggered in the ingame menu, return to the pause menu.
  Menu = LastMenu;
  SubMenu = LastSubMenu;
}
