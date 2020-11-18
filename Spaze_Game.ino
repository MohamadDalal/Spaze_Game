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
    case 1:           //Player ship 1 (Left part)
      *PntTempBitmapPos = 7;    //Assign height of bitmap
      PntTempBitmapPos++;       //Go up to width field
      *PntTempBitmapPos = 3;    //Assign width of bitmap
      PntTempBitmapPos--;       //Go back to height field
      static unsigned char Bitmap1[] = {0x04, 0x03, 0x05, 0x06, 0x05, 0x03, 0x04};
      return Bitmap1
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
      return Bitmap2
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
      return Bitmap;
  }
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
