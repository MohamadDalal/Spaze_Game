void setup()
{
  Serial.begin(115200);
  //Wire.begin();
  //Gyro_Init();
  u8g2.begin();
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  ShipSetup();
  ShipDataDump(Player);

  u8g2.clearBuffer();
  u8g2.drawStr(32, 32, "Calibrating");
  u8g2.drawStr(24, 48, "Takes 5 seconds");
  
  u8g2.sendBuffer();
  
  //Gyro_Slow_Cal();
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
          //Menu_Navigation();
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
