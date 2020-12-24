// Laser functions

//int Big_Laser = 0;
struct Laser
{
  int Active = 0;                             // If the laser is active, then proceed to drawing it
  int TopLeftCoords[2] = {113, 0};            // Place to start drawing from, also used for hitboxes. The lasers need to be at least 2 pixles apart if both are present
  // Width is Right edge - Left coordinate
  int Height = 0;                             // Height of the laser, can be between 16 and 30. Laser 2 height is (16 <= Height <= 46 - Lasers[1].Height)
  int Duration = 0;                           // Duration it is active in milliseconds
  //unsigned long StartTime = 0;                // Start time in millis{}. Not Needed anymore because if it is used the laser activation time will continue moving forward when the game is paused. Now its time is only increased when the diplay laser function is run, which only runs when the game is running.
  unsigned long ActiveTime = 0;               // Store a millis value here to compare with Duration
  int AnimationSpeed = 50;                    // Speed in which animation works in milliseconds
  int LaserSpeed = 500;                       // Time the laser takes to reach the end of the screen in milliseconds

  // When detecting hitbox check if the ships left coordinate is larger than the lasers left coordinate
  // Check if the ships top coordinate is smaller than the lasers top coordinate and that the ships bottom coordinate is larger than the lasers top coordiante + height

  // If the decoration is a horizontal line, then use lasers left coordinate and width to draw it. Y-coordinate changes with a tick function
};
struct Laser Laser;                           // Create the laser struct

// These functions setup one laser to be fired, the random function either sets up a smal laser that can be spawned anywhere, or a big laser that covers either the top or the bottom of the screen

// This function activates a laser with pre defined values. Only used for testing
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
    //Laser.StartTime = millis();
    Laser.ActiveTime = 0;
  }
}

// This function deactivates the laser by reseting its values to the way they are in the struct
void Deactivate_Laser()
{
  Laser.Active = 0;
  Laser.TopLeftCoords[0] = 113;
  Laser.TopLeftCoords[1] = 0;
  Laser.Height = 0;
  Laser.Duration = 0;
  //Laser.StartTime = 0;
  Laser.ActiveTime = 0;
}

// This function activates either a big or a small laser with random values. The size will also become random when I finish testing
void Activate_Laser_Random(int Size)
{
  if(Laser.Active)
  {
    return;
  }
  else
  {
    switch(Size)                                                          // Check the size of the laser
    {
      case 0:                                                             // If its a small laser
        Laser.Active = 1;                                                 // Set the laser as active
        Laser.Height = random(16, 33);                                    // Set the laser's height between 16 and 32 pixles
        Laser.TopLeftCoords[1] = random(0, (64 - Laser.Height));          // Set the y position of the top of the laser, so that the entire laser does not go under the screen's boundary
        Laser.Duration = (random(2, 9)) * 1000;                           // Set the time the laser will be active between 2 and 8 seconds
        //Laser.StartTime = millis();                                     // This is no longer used
        Laser.ActiveTime = 0;                                             // Reset the activity timer of the laser
        return;
      case 1:                                                             // If big laser
        Laser.Active = 1;                                                 // Set the laser as active
        Laser.Height = random(32, 49);                                    // Set the laser's height between 32 and 48 pixles
        int High;                                                         // Create an integer that decides if the laser will cover the top part or the bottom part of the screen
        High = random(0, 2);                                              // Randomly give it a number between 0 and 1
        Laser.TopLeftCoords[1] = High * (63- Laser.Height);               // When the High value is 0 (Top part) the laser starts from the top of the screen. Otherwise it starts somewhere in the middles and ends at the bottom part
        Laser.Duration = (random(2, 9)) * 1000;                           // Set the time the laser will be active between 2 and 8 seconds
        //Laser.StartTime = millis();                                     // This is no longer used
        Laser.ActiveTime = 0;                                             // Reset the activity timer of the laser
        return;
      default:                                                            // If and unknown size give an error in the terminal and don't do anything
        Serial.println("The size of the laser was not 0 or 1");
        return;
    }
  }
}

// This function displays the laser.
// First it displays the warning (Two dashes to show the size of the laser and an exclamation mark that flashes every half a second)
// Then the laser goes from the right side of the screen into the HUD's left boundary
// Then the laser stays there for its full duration
// All while a decorative horizontal line goes through the laser from top to bottom (WOW Animated stuff)
void Display_Laser()
{
  int LeftCoord;                                                                        // This integer is used to move the laser with its decoration leftwards together
  if(Laser.Active)                                                                      // Only run this if the laser is active
  {
    //Laser.ActiveTime = millis() - Laser.StartTime;                                    // No longer used
    Laser.ActiveTime += LoopTime;                                                       // Increase the activity time of the laser using the recorded looptime
    if (Laser.ActiveTime > (Laser.Duration + 2000))                                     // Check if the lasers duration with the warning 2 seconds have passed
    {
      Deactivate_Laser();                                                               // If yes deactivate the laser
      return;
    }
    else if (Laser.ActiveTime < 2000)                                                   // Check if the 2 seconds warning have not passed
    {
      int DrawMark = ((Laser.ActiveTime / 500) + 1) % 2;                                // This integer is 1 when the exclamation mark is to be drawn and 0 when not. It is set so the mark flashes with half a second intervals
      if(DrawMark)                                                                      // If the exclamation mark is present
      {
        int ExclamationHeight = Laser.TopLeftCoords[1] + (Laser.Height / 2) + 4;        // Set the exclamation mark's y position in the middle between the 2 dashes
        u8g2.drawStr(123, ExclamationHeight, "!");                                      // Draw the mark in its position
      }
      //u8g2.setFont(u8g2_font_ncenB08_tr);                                             // Choose font. Does not seem to be needed
      // The dash in font_ncenB08_tr is flying in the air by 4 pixels (Is 4 pixels heigher than its y position)       
      u8g2.drawStr(123, (Laser.TopLeftCoords[1] + 4), "-");                             // Draw the dahs at the top of the laser
      u8g2.drawStr(123, (Laser.TopLeftCoords[1] + Laser.Height + 4), "-");              // Draw the dash at the bottom of the laser
      return;
    }
    else if (Laser.ActiveTime < (2000 + Laser.LaserSpeed))                              // If it is in the period between when the laser spawns and reaches the end (Moving horizontally)
    {
      LeftCoord = 127 - (((Laser.ActiveTime - 2000) * (127 - 13)) / Laser.LaserSpeed);  // Increase the value of the left edge of the laser by a factor of its speed, so it reaches the end at the time specified by its speed
      Laser.TopLeftCoords[0] = LeftCoord;                                               // Set that left edge for the laser
    }
    else                                                                                // Otherwise if it is none of those times (It's in the period where the laser just stands there)
    {
      LeftCoord = 13;                                                                   // Set the value of the left edge at the end of the laser
      Laser.TopLeftCoords[0] = LeftCoord;                                               // Set that left edge for the laser
    }
    int Width = 127 - LeftCoord;                                                        // Calculate the width of the laser, so that it is only drawn until the right edge
    u8g2.drawBox(Laser.TopLeftCoords[0], Laser.TopLeftCoords[1], Width, Laser.Height);  // Draw the laser as a rectangle from the its data
    u8g2.setDrawColor(0);                                                               // Set the draw color to black
    int AnimationYCoord = (((Laser.ActiveTime * Laser.AnimationSpeed) / 1000) % (Laser.Height)) + Laser.TopLeftCoords[1] + 1;   // Set the Y position of the animation decoration. Use the active time to make it go around the laser with its set animation speed
    u8g2.drawHLine(Laser.TopLeftCoords[0], AnimationYCoord, Width);                     // Draw the animation decoration with its data as a black horizontal line
    u8g2.setDrawColor(1);                                                               // Set the color back to white
    //u8g2.sendBuffer();
  }
  else                                                                                  // If the laser is not active
  {
    return;
  }
}


// Machine gun functions




// Explosions function
