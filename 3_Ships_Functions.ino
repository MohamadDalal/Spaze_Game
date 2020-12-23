// Fills in the struct (Struct to fill, Type, speed, health, LUx and LUy = Left Upper corner coordinates, BMNo = Number of bitmaps, BMA = BitmapAddress, BMx and BMy = bitmap x and y relative to top left of sprite)
// If the bitmap is not going to be used, then the address and the coordinates are to be left as 0
// Use the bitmaps in order, so if only one bitmap is needed, then it has to be the first one
struct Ship CreateShip(struct Ship obj, int Type, int spd, int hp, int LUx, int LUy, int BMNo, int BMA1, int BMA2, int BMA3, int BM1x, int BM1y, int BM2x, int BM2y, int BM3x, int BM3y)
{
  //Serial.println("CreateShip function ran");
  obj.Type = Type;
  obj.Speed = spd;
  obj.Health = hp;
  obj.BitmapNum = BMNo;
  obj.TopLeftCoords[0] = LUx;
  obj.TopLeftCoords[1] = LUy;
  // Assign data for first bitmap
  obj.PntBitmap1 = Get_Bitmap(BMA1);  obj.BitmapData1[0] = BM1x;  obj.BitmapData1[1] = BM1y;  obj.BitmapData1[2] = TBmP[0];  obj.BitmapData1[3] = TBmP[1];
  int BM1RightCoord = BM1x + TBmP[0];         // Get the Rightmost pixel of the bitmap in relation to the LeftUpper point of the sprite
  int BM1BottomCoord = BM1y + TBmP[1];        // Get the Bottommost pixel of the bitmap in relation to the LeftUpper point of the sprite
  // Assign data for second bitmap
  obj.PntBitmap2 = Get_Bitmap(BMA2);  obj.BitmapData2[0] = BM2x;  obj.BitmapData2[1] = BM2y;  obj.BitmapData2[2] = TBmP[0];  obj.BitmapData2[3] = TBmP[1];
  int BM2RightCoord = BM2x + TBmP[0];         // Get the Rightmost pixel of the bitmap in relation to the LeftUpper point of the sprite
  int BM2BottomCoord = BM2y + TBmP[1];        // Get the Bottommost pixel of the bitmap in relation to the LeftUpper point of the sprite
  // Assign data for third bitmap
  obj.PntBitmap3 = Get_Bitmap(BMA3);  obj.BitmapData3[0] = BM3x;  obj.BitmapData3[1] = BM3y;  obj.BitmapData3[2] = TBmP[0];  obj.BitmapData3[3] = TBmP[1];
  int BM3RightCoord = BM3x + TBmP[0];         // Get the Rightmost pixel of the bitmap in relation to the LeftUpper point of the sprite
  int BM3BottomCoord = BM3y + TBmP[1];        // Get the Bottommost pixel of the bitmap in relation to the LeftUpper point of the sprite

  Serial.print("BM1RightCoord is: "); Serial.println(BM1RightCoord);
  Serial.print("BM2RightCoord is: "); Serial.println(BM2RightCoord);
  Serial.print("BM3RightCoord is: "); Serial.println(BM3RightCoord);
  int TempBMMaxR = max(BM1RightCoord, BM2RightCoord);     // Compare the Rightmost point of bitmap 1 and 2
  Serial.print("TempBMMaxR is: "); Serial.println(TempBMMaxR);

  Serial.print("BM1BottomCoord is: "); Serial.println(BM1BottomCoord);
  Serial.print("BM2BottomCoord is: "); Serial.println(BM2BottomCoord);
  Serial.print("BM3BottomCoord is: "); Serial.println(BM3BottomCoord);
  int TempBMMaxB = max(BM1BottomCoord, BM2BottomCoord);    // Compare the Bottommost point of bitmap 1 and 2
  Serial.print("TempBMMaxB is: "); Serial.println(TempBMMaxB);
 
  obj.BottomRightCoords[0] = LUx  + max(TempBMMaxR, BM3RightCoord) - 1;    // Compare the Rightmost point of the max from the last comparison and Bitmap 3. This give the rightmost point of the sprite. -1 becuase the pixels begin counting from 0, a sprite of width 7 has the last pixel at 6 not 7
  obj.BottomRightCoords[1] = LUy + max(TempBMMaxB, BM3BottomCoord) - 1;   // Compare the Bottommost point of the max from the last comparison and Bitmap 3. This give the Bottommost point of the sprite. Same as above
  return obj;
}

// Write the ships you want to create here, then the setup function will just run this function to create them
void ShipSetup()
{
  //Serial.println("ShipSetup ran");    //Debug
  //struct Ship Player;                 //Struct had to be initialized in global area, so this failed
  Player = CreateShip(/*obj*/Player,/*Type*/ 0,/*spd*/ 150,/*HP*/ 5,/*LUx*/ 26,/*LUy*/ 32,/*BMNo*/ 2,/*BMA1*/ 1,/*BMA2*/ 2,/*BMA3*/ 0,/*BMx1*/ 0,/*BMy1*/ 0,/*BMx2*/ 3,/*BMy2*/ 2,/*BMx3*/ 0,/*BMy3*/ 0);
}

// Draws a ship with max 3 bitmaps
void DrawShipXBM(struct Ship obj)
{
  u8g2.setDrawColor(2);               // Set the color so that the ship is white on black background and black on white backgrounds
  switch(obj.BitmapNum)
  {
    //drawXBM args: x position, y position, width, height, bitmap array
    case 0:
      //Serial.println("BitmapNum 0");
      break;
    case 1:
      //Serial.println("BitmapNum 1");
      u8g2.drawXBM(obj.TopLeftCoords[0] - obj.BitmapData1[0], obj.TopLeftCoords[1] - obj.BitmapData1[1], obj.BitmapData1[2], obj.BitmapData1[3], obj.PntBitmap1);
      break;
    case 2:
      //Serial.println("BitmapNum 2");
      u8g2.drawXBM(obj.TopLeftCoords[0] + obj.BitmapData1[0], obj.TopLeftCoords[1] + obj.BitmapData1[1], obj.BitmapData1[2], obj.BitmapData1[3], obj.PntBitmap1);
      u8g2.drawXBM(obj.TopLeftCoords[0] + obj.BitmapData2[0], obj.TopLeftCoords[1] + obj.BitmapData2[1], obj.BitmapData2[2], obj.BitmapData2[3], obj.PntBitmap2);
      break;
    case 3:
      //Serial.println("BitmapNum 3");
      u8g2.drawXBM(obj.TopLeftCoords[0] - obj.BitmapData1[0], obj.TopLeftCoords[1] - obj.BitmapData1[1], obj.BitmapData1[2], obj.BitmapData1[3], obj.PntBitmap1);
      u8g2.drawXBM(obj.TopLeftCoords[0] - obj.BitmapData2[0], obj.TopLeftCoords[1] - obj.BitmapData2[1], obj.BitmapData2[2], obj.BitmapData2[3], obj.PntBitmap2);
      u8g2.drawXBM(obj.TopLeftCoords[0] - obj.BitmapData3[0], obj.TopLeftCoords[1] - obj.BitmapData3[1], obj.BitmapData3[2], obj.BitmapData3[3], obj.PntBitmap3);
      break;
    default:
      Serial.println("Dahek is dis?");
  }
  u8g2.setDrawColor(1);               // Set the color back to only black
}
