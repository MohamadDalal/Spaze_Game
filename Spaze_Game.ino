
#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>

/*-------------------------------------Pins--------------------------------
Screen                Using SPI
GND             GND
VDD             3.3V or Vin
SCK             GPIO18
SDA             GPIO23 (MOSI)
RES             GPIO17
DC              GPIO16
CS              GPIO5

Gyroscope MPU6050     Using I2C
VCC             3.3V or Vin
GND             GND
SCL             GPIO22 (I2C SCL)
SDA             GPIO21 (I2C SDA)
-------------------------------------------------------------------------*/


U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI   u8g2  (U8G2_R0, 5, 16, 17);

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
  int BitmapData1[4]; // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Width, Height]
  int BitmapData2[4]; // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Width, Height]
  int BitmapData3[4]; // [x position relative to TopLeftCoords, y position relative to TopLeftCoords, Width, Height]
  // Bitmap pointers will be assigned using a function. All bitmaps will be stored in one place.
  unsigned char *PntBitmap1;  //Pointer to first bitmap
  unsigned char *PntBitmap2;  //Pointer to second bitmap
  unsigned char *PntBitmap3;  //Pointer to third bitmap
};
//--------------------------------------------------Initializing structs--------------------------------------------
struct Ship Player;
struct Ship Enemy[3];
//CreateShip(/*obj*/Player,/*Type*/ 0,/*spd*/ 10,/*HP*/ 5,/*LUx*/ 26,/*LUy*/ 32,/*BMNo*/ 2,/*BMA1*/ 1,/*BMA2*/ 2,/*BMA3*/ 0,/*BMx1*/ 0,/*BMy1*/ 0,/*BMx2*/ 2,/*BMy2*/ 1,/*BMx3*/ 0,/*BMy3*/ 0);
//Does not like running functions outside main functions
//----------------------------------------------------------------------------------------------------------------


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





// Fills in the struct (Struct to fill, Type, speed, health, LUx and LUy = Left Upper corner coordinates, BMNo = Number of bitmaps, BMA = BitmapAddress, BMx and BMy = bitmap x and y relative to top left of sprite)
// If the bitmap is not going to be used, then the address and the coordinates are to be left as 0
// Use the bitmaps in order, so if only one bitmap is needed, then it has to be the first one
struct Ship CreateShip(struct Ship obj, int Type, int spd, int hp, int LUx, int LUy, int BMNo, int BMA1, int BMA2, int BMA3, int BM1x, int BM1y, int BM2x, int BM2y, int BM3x, int BM3y)
{
  Serial.println("CreateShip function ran");
  obj.Type = Type;
  obj.Speed = spd;
  obj.Health = hp;
  obj.BitmapNum = BMNo;
  obj.TopLeftCoords[0] = LUx;
  obj.TopLeftCoords[1] = LUy;
  // Assign data for first bitmap
  obj.PntBitmap1 = Get_Bitmap(BMA1);  obj.BitmapData1[0] = BM1x;  obj.BitmapData1[1] = BM1y;  obj.BitmapData1[2] = TempBitmapPos[0];  obj.BitmapData1[3] = TempBitmapPos[1];
  // Assign data for second bitmap
  obj.PntBitmap2 = Get_Bitmap(BMA2);  obj.BitmapData2[0] = BM2x;  obj.BitmapData2[1] = BM2y;  obj.BitmapData2[2] = TempBitmapPos[0];  obj.BitmapData2[3] = TempBitmapPos[1];
  // Assign data for third bitmap
  obj.PntBitmap3 = Get_Bitmap(BMA3);  obj.BitmapData3[0] = BM3x;  obj.BitmapData3[1] = BM3y;  obj.BitmapData3[2] = TempBitmapPos[0];  obj.BitmapData3[3] = TempBitmapPos[1];
  return obj;
}

void ShipSetup()
{
  Serial.println("ShipSetup ran");
  //struct Ship Player;
  Player = CreateShip(/*obj*/Player,/*Type*/ 0,/*spd*/ 10,/*HP*/ 5,/*LUx*/ 26,/*LUy*/ 32,/*BMNo*/ 2,/*BMA1*/ 1,/*BMA2*/ 2,/*BMA3*/ 0,/*BMx1*/ 0,/*BMy1*/ 0,/*BMx2*/ 2,/*BMy2*/ 2,/*BMx3*/ 0,/*BMy3*/ 0);
}

void DrawShipXBM(struct Ship obj)
{
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
}

void Check_Input()
{
  //if digitalRead(
  return;
}

void setup()
{
  Serial.begin(115200);
  u8g2.begin();
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  ShipSetup();
  ShipDataDump(Player);
}

void loop() 
{
  u8g2.clearBuffer();
  DrawShipXBM(Player);
  u8g2.drawCircle(100, 25, 10, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  delay(100);
}
