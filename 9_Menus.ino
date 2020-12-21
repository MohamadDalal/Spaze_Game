void Main_Menu()
{
  u8g2.clearBuffer();
  u8g2.drawStr(32, 8, "Start Game");
  u8g2.drawStr(32, 24, "Help/Controls");
  u8g2.drawStr(32, 40, "High Scores");
  u8g2.drawStr(8, (MenuCursor * 16 + 8), "-->");
  u8g2.setFont(u8g2_font_p01type_tr);
  u8g2.drawStr(2, 56, "LB:CALIBRATE");
  u8g2.drawStr(84, 56, "RB:SELECT");
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.sendBuffer();
  //delay(10);
}

void Slow_Cal_Screen()
{
  u8g2.clearBuffer();
  u8g2.drawStr(32, 32, "Calibrating");
  u8g2.drawStr(24, 48, "Takes 5 seconds");
  u8g2.sendBuffer();
}

void Game_Screen()
{
  u8g2.clearBuffer();
  DrawShipXBM(Player);
  //u8g2.drawCircle(100, 25, 10, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  //delay(100);
}

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

void Coming_Soon()
{
  u8g2.clearBuffer();
  u8g2.drawStr(26, 32, "Coming Soon");
  u8g2.setFont(u8g2_font_p01type_tr);
  u8g2.drawStr(2, 56, "LB:BACK");
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.sendBuffer();
}

void Unknown_Error_Screen()
{
  u8g2.clearBuffer();
  u8g2.drawStr(4, 16, "An unknown error has");
  u8g2.drawStr(40, 32, "occured");
  u8g2.drawStr(24, 48, "Please restart");
  u8g2.sendBuffer();
}

void Gyro_Error_Screen()
{
  u8g2.clearBuffer();
  u8g2.drawStr(0, 16, "No Gyroscope detected");
  u8g2.drawStr(8, 32, "Please check that all");
  u8g2.drawStr(8, 48, "wires are connected");
  u8g2.sendBuffer();
  while(not Gyroscope.isConnected())
  {
    delay(100);
  }
  Menu = LastMenu;
  SubMenu = LastSubMenu;
  // This will need to be redone later, so that it returns you to the place u were in before it happened
  // If I can't figure that out, then it will just reset the esp when it happens (Or return u to the main menu)
  // Above comments have been done to an extent
}
