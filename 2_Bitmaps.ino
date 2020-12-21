//static unsigned char Bitmap[8];
int TBmP[] = {0,0};              //Temporary bitmap position (TempBitmapPos)
int *PTBmP = TBmP;    //Pointer to temporary bitmap position (PntTempBitmapPos)
  
unsigned char * Get_Bitmap(int Num)
{
  switch(Num)
  {
    case 0:           //No Bitmap/Reset bitmap data
      *PTBmP = 0;    //Assign width of bitmap
      PTBmP++;       //Go up to height field
      *PTBmP = 0;    //Assign height of bitmap
      PTBmP--;       //Go back to width field
      static unsigned char Bitmap0[] = {0x00};
      return Bitmap0;
    case 1:           //Player ship 1 (Left part)
      *PTBmP = 3;    //Assign width of bitmap
      PTBmP++;       //Go up to height field
      *PTBmP = 7;    //Assign height of bitmap
      PTBmP--;       //Go back to width field
      //static unsigned char Bitmap1[] = {0x04, 0x03, 0x05, 0x06, 0x05, 0x03, 0x04}; Gimp gives the value for the black pixels, which means I need to invert the picture before putting it here
      static unsigned char Bitmap1[] = {0x03, 0x04, 0x02, 0x01, 0x02, 0x04, 0x03};
      return Bitmap1;
/*    
      Killed cause I don't think that the right method, but I am not sure
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
      *PTBmP = 6;    //Assign width of bitmap
      PTBmP++;       //Go up to height field
      *PTBmP = 3;    //Assign height of bitmap
      PTBmP--;       //Go back to width field
      static unsigned char Bitmap2[] = {0x1d, 0x22, 0x1d};
      return Bitmap2;
    default:
      Serial.println("Bitmap out of range");
      return Bitmap0;
  }
}
