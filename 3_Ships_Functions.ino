struct Ship
{
  //-----------------------------Inner Data----------------------------------//
  int Type;                   // 0 for Invincible and 1 for Normal
  int Speed;                  // Speed in pixles per second
  int Health;                 // Health points
  
  //------------------------------Visual Data--------------------------------//
  int TopLeftCoords[2];       // Coordinates of top left pixel first is x then is y
  int BottomRightCoords[2];   // Coordinates of the bottom right pixle, first is x then is y. This is taken to know how high and wide the ship is
  int BitmapNum;              // Amount of bitmaps to be used. sprite might be split in different parts. Currently capped at 3
  //int HitboxNum;            // Number of hitboxes // Decided to tie hitboxes to bitmaps, will be added if needed
  int BitmapData1[4];         // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Width, Height]
  int BitmapData2[4];         // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Width, Height]
  int BitmapData3[4];         // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Width, Height]
  // Bitmap pointers will be assigned using a function. All bitmaps will be stored in one place.
  unsigned char *PntBitmap1;  // Pointer to first bitmap
  unsigned char *PntBitmap2;  // Pointer to second bitmap
  unsigned char *PntBitmap3;  // Pointer to third bitmap
};
//--------------------------------------------------Initializing structs--------------------------------------------
struct Ship Player;
//struct Ship Enemy[3];
//CreateShip(/*obj*/Player,/*Type*/ 0,/*spd*/ 10,/*HP*/ 5,/*LUx*/ 26,/*LUy*/ 32,/*BMNo*/ 2,/*BMA1*/ 1,/*BMA2*/ 2,/*BMA3*/ 0,/*BMx1*/ 0,/*BMy1*/ 0,/*BMx2*/ 2,/*BMy2*/ 1,/*BMx3*/ 0,/*BMy3*/ 0);
//Does not like running functions in global area
//----------------------------------------------------------------------------------------------------------------



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

// Write the ships you want to create here, then this function will be run to create them at the start of a game session
void ShipSetup()
{
  //Serial.println("ShipSetup ran");    //Debug
  //struct Ship Player;                 //Struct had to be initialized in global area, so this failed
  int PlayerBMx1;                       // Store the x value of the first bitmap of the player
  int PlayerBMx2;                       // Store the x value of the first bitmap of the player
  if(CorruptShip)                       // If the corrupt ship option is triggered
  {
    //Invert the two bitmaps horizontally
    PlayerBMx1 = 6;
    PlayerBMx2 = 0;
  }
  else                                  // If it is not
  {
    // Put the correct value in
    PlayerBMx1 = 0;
    PlayerBMx2 = 3;
  }
  // Create the ships by running create ship function
  Player = CreateShip(/*obj*/Player,/*Type*/ 1,/*spd*/ 150,/*HP*/ 5,/*LUx*/ 26,/*LUy*/ 32,/*BMNo*/ 2,/*BMA1*/ 1,/*BMA2*/ 2,/*BMA3*/ 0,/*BMx1*/ PlayerBMx1,/*BMy1*/ 0,/*BMx2*/ PlayerBMx2,/*BMy2*/ 2,/*BMx3*/ 0,/*BMy3*/ 0);
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
      Menu = 2;
      SubMenu = 0;
  }
  u8g2.setDrawColor(1);               // Set the color back to only black
}
