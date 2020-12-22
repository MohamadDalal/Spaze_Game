// The main menu displaying function
void Main_Menu()
{
  u8g2.clearBuffer();
  u8g2.drawStr(32, 8, "Start Game");
  u8g2.drawStr(32, 24, "Settings");
  u8g2.drawStr(32, 40, "Help");
  u8g2.drawStr(8, (MenuCursor * 16 + 8), "-->");
  u8g2.setFont(u8g2_font_p01type_tr);
  u8g2.drawStr(2, 56, "LB:CALIBRATE");
  u8g2.drawStr(84, 56, "RB:SELECT");
  u8g2.setFont(u8g2_font_ncenB08_tr);
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

// The actual game displaying function. If I acutally make a proper game this will become biggus
void Game_Screen()
{
  u8g2.clearBuffer();
  DrawShipXBM(Player);
  //u8g2.drawCircle(100, 25, 10, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  //delay(100);
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
  u8g2.drawStr(2, 56, "LB:BACK");
  u8g2.drawStr(84, 56, "RB:SELECT");
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.sendBuffer();
}

// The settings displaying function
void Settings_Screen()
{
  u8g2.clearBuffer();
  char fpsToDraw[10];
  sprintf(fpsToDraw, "FPS  %i", fps);
  u8g2.drawStr(32, 8, fpsToDraw);
  u8g2.drawStr(32, 24, "Controls");
  u8g2.drawStr(32, 40, "Something");
  u8g2.drawStr(8, (MenuCursor * 16 + 8), "-->");
  u8g2.setFont(u8g2_font_p01type_tr);
  u8g2.drawStr(2, 56, "LB:BACK");
  u8g2.drawStr(84, 56, "RB:SELECT");
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.sendBuffer();
}

// A function to be displayed if I add a selection just to fill place
void Coming_Soon()
{
  u8g2.clearBuffer();
  u8g2.drawStr(26, 32, "Coming Soon");
  u8g2.setFont(u8g2_font_p01type_tr);
  u8g2.drawStr(2, 56, "LB:BACK");
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.sendBuffer();
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
  // This will need to be redone later, so that it returns you to the place u were in before it happened
  // If I can't figure that out, then it will just reset the esp when it happens (Or return u to the main menu)
  // Above comments have been done to an extent
}
