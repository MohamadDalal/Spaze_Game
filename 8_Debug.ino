// Prints the values inside the struct of a specific ship object
void ShipDataDump(struct Ship obj)
{
  Serial.println(obj.Type);
  Serial.println(obj.Speed);
  Serial.println(obj.Health);
  Serial.println(obj.BitmapNum);
  for (int i; i<4; i++)
  {
    //printf("BitmapData1[%i] is: ", i); printf(obj.BitmapData1[i]); printf("\n");
    //printf("BitmapData2[%i] is: ", i); printf(obj.BitmapData2[i]); printf("\n");
    Serial.println(obj.BitmapData1[i]);
    Serial.println(obj.BitmapData2[i]);
  }
}

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
