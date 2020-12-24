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
