// Laser functions

//int Big_Laser = 0;
struct Laser
{
  int Active = 0;                             // If the laser is active, then proceed to drawing it
  int TopLeftCoords[2] = {113, 0};            // Place to start drawing from, also used for hitboxes. The lasers need to be at least 2 pixles apart if both are present
  // Width is Right edge - Left coordinate
  int Height = 0;                             // Height of the laser, can be between 16 and 30. Laser 2 height is (16 <= Height <= 46 - Lasers[1].Height)
  int Duration = 0;                           // Duration it is active in milliseconds
  unsigned long StartTime = 0;                // Start time in millis{}
  unsigned long ActiveTime = 0;               // Store a millis value here to compare with Duration
  int AnimationSpeed = 50;                    // Speed in which animation works in milliseconds
  int LaserSpeed = 500;                      // Time the laser takes to cover the entir screen in milliseconds

  // When detecting hitbox check if the ships left coordinate is larger than the lasers left coordinate
  // Check if the ships top coordinate is smaller than the lasers top coordinate and that the ships bottom coordinate is larger than the lasers top coordiante + height

  // If the decoration is a horizontal line, then use lasers left coordinate and width to draw it. Y-coordinate changes with a tick function
};
struct Laser Laser;

// These functions setup one laser to be fired, the random function either sets up a smal laser that can be spawned anywhere, or a big laser that covers either the top or the bottom of the screen


void Activate_Laser(int Height, int StartPos, int Duration)
{
  if(Laser.Active)
  {
    return;
  }
  else
  {
    Laser.Active = 1;
    Laser.TopLeftCoords[1] = StartPos;
    Laser.Height = Height;
    Laser.Duration = Duration;
    Laser.StartTime = millis();
  }
}

void Deactivate_Laser(int Num)
{
  Laser.Active = 0;
  Laser.TopLeftCoords[0] = 113;
  Laser.TopLeftCoords[1] = 0;
  Laser.Height = 0;
  Laser.Duration = 0;
  Laser.StartTime = 0;
  Laser.ActiveTime = 0;
}

void Activate_Laser_Random(int Size)
{
  if(Laser.Active)
  {
    return;
  }
  else
  {
    switch(Size)
    {
      case 0:
        Laser.Active = 1;
        Laser.Height = random(16, 33);
        Laser.TopLeftCoords[1] = random(0, (64 - Laser.Height));
        Laser.Duration = (random(2, 9)) * 1000;
        Laser.StartTime = millis();
        return;
      case 1:
        Laser.Active = 1;
        Laser.Height = random(32, 49);
        int High;
        High = random(0, 2);
        Laser.TopLeftCoords[1] = High * (63- Laser.Height);
        Laser.Duration = (random(2, 9)) * 1000;
        Laser.StartTime = millis();
        return;
      default:
        return;
    }
  }
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
  Serial.print("StartTime = ");
  Serial.println(obj.StartTime);
  Serial.print("ActiveTime = ");
  Serial.println(obj.ActiveTime);
  Serial.print("AnimationSpeed = ");
  Serial.println(obj.AnimationSpeed);
}

void Display_Lasers()
{
  int LeftCoord;
  if(Laser.Active)
  {
    Laser.ActiveTime = millis() - Laser.StartTime;
    if (Laser.ActiveTime > (Laser.Duration + 2000))
    {
      Deactivate_Laser(0);
      Deactivate_Laser(1);
      return;
    }
    else if (Laser.ActiveTime < 2000)
    {
      int DrawMark = ((Laser.ActiveTime / 500) + 1) % 2;
      if(DrawMark)
      {
        int ExclamationHeight = Laser.TopLeftCoords[1] + (Laser.Height / 2) + 4;
        u8g2.drawStr(120, ExclamationHeight, "!");
      }
      u8g2.setFont(u8g2_font_ncenB08_tr);       // Choose font
      u8g2.drawStr(120, (Laser.TopLeftCoords[1] + 4), "-");                    // The - sign in font_ncenB08_tr is flying in the air by 4 pixels (Is 4 pixels heighter than its y position)
      u8g2.drawStr(120, (Laser.TopLeftCoords[1] + Laser.Height + 4), "-");
      return;
    }
    else if (Laser.ActiveTime < (2000 + Laser.LaserSpeed))
    {
      LeftCoord = 127 - (((Laser.ActiveTime - 2000) * (127 - 13)) / Laser.LaserSpeed);
      Laser.TopLeftCoords[0] = LeftCoord;
    }
    else
    {
      LeftCoord = 13;
      Laser.TopLeftCoords[0] = LeftCoord;
    }
    int Width = 127 - LeftCoord;
    u8g2.drawBox(Laser.TopLeftCoords[0], Laser.TopLeftCoords[1], Width, Laser.Height);
    u8g2.setDrawColor(0);
    int AnimationYCoord = (((Laser.ActiveTime * Laser.AnimationSpeed) / 1000) % (Laser.Height)) + Laser.TopLeftCoords[1] + 1;
    u8g2.drawHLine(Laser.TopLeftCoords[0], AnimationYCoord, Width);
    u8g2.setDrawColor(1);
    u8g2.sendBuffer();
  }
}


// Machine gun functions




// Explosions function
