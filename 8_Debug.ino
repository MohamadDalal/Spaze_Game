// Prints the values inside the struct of a specific ship object
void ShipDataDump(struct Ship obj)
{
  Serial.print("Type is ");
  Serial.println(obj.Type);
  Serial.print("Speed is ");
  Serial.println(obj.Speed);
  Serial.print("Health is ");
  Serial.println(obj.Health);
  Serial.print("Number of bitmaps is ");
  Serial.println(obj.BitmapNum);
  Serial.print("The color of the ship is");
  Serial.println(obj.Color);
  
  Serial.println("Bitmap data:");
  for (int i; i<4; i++)
  {
    //printf("BitmapData1[%i] is: ", i); printf(obj.BitmapData1[i]); printf("\n");
    //printf("BitmapData2[%i] is: ", i); printf(obj.BitmapData2[i]); printf("\n");
    Serial.println(obj.BitmapData1[i]);
    Serial.println(obj.BitmapData2[i]);
  }
  Serial.print("Leftmost coord is ");
  Serial.println(obj.TopLeftCoords[0]);
  Serial.print("Topmost coord is ");
  Serial.println(obj.TopLeftCoords[1]);
  Serial.print("Rightmost coord is ");
  Serial.println(obj.BottomRightCoords[0]);
  Serial.print("Bottommost coord is ");
  Serial.println(obj.BottomRightCoords[1]);
}

// Prints the values inside the struct of a specific laser object
void Dump_Laser(struct Laser obj)
{
  Serial.print("Active = ");
  Serial.println(obj.Active);
  Serial.print("LeftCoord = ");
  Serial.println(obj.TopLeftCoords[0]);
  Serial.print("TopCoord = ");
  Serial.println(obj.TopLeftCoords[1]);
  Serial.print("Height = ");
  Serial.println(obj.Height);
  Serial.print("Duration = ");
  Serial.println(obj.Duration);
  //Serial.print("StartTime = ");
  //Serial.println(obj.StartTime);
  Serial.print("ActiveTime = ");
  Serial.println(obj.ActiveTime);
  Serial.print("AnimationSpeed = ");
  Serial.println(obj.AnimationSpeed);
}

// Prints the values inside the verical laser struct
void Dump_VertLaser()
{
  Serial.println("");
  Serial.print("Speed ");
  Serial.println(VertLaser.Speed);
  Serial.print("FireTime ");
  Serial.println(VertLaser.FireTime);
  
  Serial.print("FirePxl ");
  Serial.println(VertLaser.Speed * (VertLaser.FireTime / 1000.0));
  Serial.print("SafeTime ");
  Serial.println(VertLaser.SafeTime);
  Serial.print("SafePxl ");
  Serial.println(VertLaser.Speed * (VertLaser.SafeTime / 1000.0));
  Serial.print("Loops ");
  Serial.println(VertLaser.Loops);
  Serial.print("ActiveTime ");
  Serial.println(VertLaser.ActiveTime);
}

// Print the data of the explosion in the terminal
void Dump_Explosion(int Num)
{
  Serial.print("Dumping explosion number ");
  Serial.println(Num);
  Serial.print("Acive? "); 
  Serial.println(Explosion[Num].Active);
  Serial.print("X Coordinate ");
  Serial.println(Explosion[Num].CenterCoords[0]);
  Serial.print("Y Coordinate ");
  Serial.println(Explosion[Num].CenterCoords[1]);
  Serial.print("Big Circle Radius ");
  Serial.println(Explosion[Num].BigRad);
  Serial.print("Small Circle Radius ");
  Serial.println(Explosion[Num].SmallRad);
  Serial.print("Duration ");
  Serial.println(Explosion[Num].Duration);
  Serial.print("Active Time ");
  Serial.println(Explosion[Num].ActiveTime);
  Serial.print("Speed ");
  Serial.println(Explosion[Num].Speed);
  Serial.print("Animation Speed ");
  Serial.println(Explosion[Num].AnimationSpeed);
}

// This is a screen used to test the menu cursor navigation function
void Menu_Navigation_Screen()
{
  u8g2.clearBuffer();
  u8g2.drawStr(20, 16, "Welcome to the");
  u8g2.drawStr(32, 32, "main menu");
  char StringToDraw[50];
  sprintf(StringToDraw, "Cursor value is %i", MenuCursor);
  u8g2.drawStr(16, 48, StringToDraw);
  u8g2.sendBuffer();
  //printf("gy value is: %f \n", gy); 
}
