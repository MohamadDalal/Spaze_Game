#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI   u8g2  (U8G2_R0, 5, 22, 17);

int TempBitmapPos[2] = {0,1};


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
  
}

unsigned char * Get_Bitmap(int Num)
{
  switch(Num)
  {
    case 1:           //Player ship 1 (Left part)
      
      static unsigned char Bitmap[] = {0x04, 0x03, 0x05, 0x06, 0x05, 0x03, 0x04};
      return Bitmap;
    case 2:
      static unsigned char Bitmap[] = {0x22, 0x1d, 0x22};
      return Bitmap;
    default:
      Serial.print("Bitmap out of range");
      return
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
