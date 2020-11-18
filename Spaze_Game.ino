#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI   u8g2  (U8G2_R0, 5, 22, 17);

//static unsigned char Bitmap[8];
int TempBitmapPos[] = {0,0};
int *PntTempBitmapPos = TempBitmapPos;


struct Ship
{
  //-----------------------------Inner Data----------------------------------//
  int Type;         //0 for player and 1 for enemy and 2 for asteroid
  int Speed;        //Speed in pixle per second
  int Health;       //Health points
  
  //------------------------------Visual Data--------------------------------//
  int TopLeftCoords[2]; //Coordinates of top left pixel first is x then is y
  int BitmapNum;    //Amount of bitmaps to be used. sprite might be split in different parts. Currently capped at 3
  //int HitboxNum;    //Number of hitboxes // Decided to tie hitboxes to bitmaps, will be added if needed
  int BitmapData1[4]; // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Height, Width]
  int BitmapData2[4]; // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Height, Width]
  int BitmapData3[4]; // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Height, Width]
  // Bitmap pointers will be assigned using a function. All bitmaps will be stored in one place.
  unsigned char *PntBitmap1;  //Pointer to first bitmap
  unsigned char *PntBitmap2;  //Pointer to second bitmap
  unsigned char *PntBitmap3;  //Pointer to third bitmap
};

unsigned char * Get_Bitmap(int Num)
{
  switch(Num)
  {
    case 0:           //No Bitmap/Reset bitmap data
      *PntTempBitmapPos = 0;    //Assign height of bitmap
      PntTempBitmapPos++;       //Go up to width field
      *PntTempBitmapPos = 0;    //Assign width of bitmap
      PntTempBitmapPos--;       //Go back to height field
      static unsigned char Bitmap0[] = {0x00};
      return Bitmap0;
    case 1:           //Player ship 1 (Left part)
      *PntTempBitmapPos = 7;    //Assign height of bitmap
      PntTempBitmapPos++;       //Go up to width field
      *PntTempBitmapPos = 3;    //Assign width of bitmap
      PntTempBitmapPos--;       //Go back to height field
      static unsigned char Bitmap1[] = {0x04, 0x03, 0x05, 0x06, 0x05, 0x03, 0x04};
      return Bitmap1;
/*    Killed cause I don't think that the right method, but I am not sure
      //Write the bitmap
      Bitmap[0] = 0x04;
      Bitmap[1] = 0x03;
      Bitmap[2] = 0x05;
      Bitmap[3] = 0x06;
      Bitmap[4] = 0x05;
      Bitmap[5] = 0x03;
      Bitmap[6] = 0x04;
      Bitmap[7] = 0x00;

      return Bitmap;            //Return a pointer to the bitmap
*/
    case 2:           //Player ship 2 (Right part)
      *PntTempBitmapPos = 3;    //Assign height of bitmap
      PntTempBitmapPos++;       //Go up to width field
      *PntTempBitmapPos = 6;    //Assign width of bitmap
      PntTempBitmapPos--;       //Go back to height field
      static unsigned char Bitmap2[] = {0x22, 0x1d, 0x22};
      return Bitmap2;
/*
      Bitmap[0] = 0x22; 
      Bitmap[1] = 0x1d;
      Bitmap[2] = 0x22;
      Bitmap[3] = 0x00;
      Bitmap[4] = 0x00;
      Bitmap[5] = 0x00;
      Bitmap[6] = 0x00;
      Bitmap[7] = 0x00;
      return Bitmap;
*/
    default:
      Serial.print("Bitmap out of range");
      return Bitmap0;
  }
}


// Fills in the struct (Struct to fill, Type, speed, health, BMNo = Number of bitmaps, BMA = BitmapAddress, BMx and BMy = bitmap x and y relative to top left of sprite)
// If the bitmap is not going to be used, then the address and the coordinates are to be left as 0
// Use the bitmaps in order, so if only one bitmap is needed, then it has to be the first one
void CreateShip(struct Ship obj, int Type, int spd, int hp, int BMNo, int BMA1, int BMA2, int BMA3, int BM1x, int BM1y, int BM2x, int BM2y, int BM3x, int BM3y)
{
  obj.Type = Type;
  obj.Speed = spd;
  obj.Health = hp;
  obj.BitmapNum = BMNo;
  // Assign data for first bitmap
  obj.PntBitmap1 = Get_Bitmap(BMA1);
  obj.BitmapData1[0] = BM1x;
  obj.BitmapData1[1] = BM1y;
  obj.BitmapData1[2] = TempBitmapPos[0];
  obj.BitmapData1[3] = TempBitmapPos[1];
  // Assign data for second bitmap
  obj.PntBitmap2 = Get_Bitmap(BMA2);
  obj.BitmapData2[0] = BM2x;
  obj.BitmapData2[1] = BM2y;
  obj.BitmapData2[2] = TempBitmapPos[0];
  obj.BitmapData2[3] = TempBitmapPos[1];
  // Assign data for third bitmap
  obj.PntBitmap3 = Get_Bitmap(BMA3);
  obj.BitmapData3[0] = BM3x;
  obj.BitmapData3[1] = BM3y;
  obj.BitmapData3[2] = TempBitmapPos[0];
  obj.BitmapData3[3] = TempBitmapPos[1];
}


void setup()
{
  Serial.begin(115200);
  u8g2.begin();
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

}
