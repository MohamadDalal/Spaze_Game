// Randomly generate between 0 and 1 with

// Randomness of lasers is a function where the chance of a big laser increases, which decreases the chance of a small laser.
// A laser will be fired every 10 seconds unless the machinegun is fired, which I think would be fired for 20 seconds at the start of each minute

// The explosions will be up to 5 at the same time, where every 2 minutes the number of explosions increases. I will try to get the explosions to avoid the lasers, so they cover the area not covered by the lasers


// Laser functions

//int Big_Laser = 0;
struct Laser
{
  int Active = 0;                             // If the laser is active, then proceed to drawing it
  int TopLeftCoords[2] = {113, 0};            // Place to start drawing from, also used for hitboxes. The lasers need to be at least 2 pixles apart if both are present
  // Width is Right edge - Left coordinate
  int Height = 0;                             // Height of the laser, can be between 16 and 30. Laser 2 height is (16 <= Height <= 46 - Lasers[1].Height)
  int Duration = 0;                           // Duration it is active in milliseconds
  //unsigned long StartTime = 0;              // Start time in millis{}. Not Needed anymore because if it is used the laser activation time will continue moving forward when the game is paused. Now its time is only increased when the diplay laser function is run, which only runs when the game is running.
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
  //Dump_Laser(Laser);                                                      // Dump the data of the laser
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
const int ExplosionsMax = 10;                                                           // The maximum amount of explosions allowed (If there is place for them)

struct Explosion
{
  int Active = 0;                                                                       // If the explosion is active this is 1 (Used for various checks)
  bool DetectHit = false;                                                               // This is true if the explosion is to deal damage. Made so the explosion does not deal damage while the warning animation is happening
  int CenterCoords[2] = {-64, -64};                                                     // The poisition of the center of the explosion. Initially somewhere far away from the screens boundaries
  int BigRad = 0;                                                                       // The radius of the explosion
  int SmallRad = 0;                                                                     // The radius of the small circle used for the warning animation
  int Duration = 0;                                                                     // The duration of the explosion, does not count the warning time in it
  int ActiveTime = 0;                                                                   // The time that the explosion has been active
  int Speed = 1000;                                                                     // The time it takes for the warning to finish. Or the speed of which the explosion goes off
  int AnimationSpeed = 200;                                                             // The speed of the animation, where the explosion's radius becomes smaller by a pixel and back to original size
};
struct Explosion Explosion[ExplosionsMax];                                              // An array with all the explosions. Since this is only made at the start, then the ExplosionMax variable cannot be bigger than what it was at the start of the program

// Print the data of the explosion in the terminal
void Dump_Explosion(int Num)
{
  Serial.print("Dumping explosion number ");
  Serial.println(Num);
  Serial.print("Acive? "); 
  Serial.println(Explosion[Num].Active);
  Serial.print("X Coordinate ");
  Serial.println(Explosion[Num].CenterCoords[0]);
  Serial.print("Y Coordinate ");
  Serial.println(Explosion[Num].CenterCoords[1]);
  Serial.print("Big Circle Radius ");
  Serial.println(Explosion[Num].BigRad);
  Serial.print("Small Circle Radius ");
  Serial.println(Explosion[Num].SmallRad);
  Serial.print("Duration ");
  Serial.println(Explosion[Num].Duration);
  Serial.print("Active Time ");
  Serial.println(Explosion[Num].ActiveTime);
  Serial.print("Speed ");
  Serial.println(Explosion[Num].Speed);
  Serial.print("Animation Speed ");
  Serial.println(Explosion[Num].AnimationSpeed);
}

// Activate and explosion with preset values. Arguments(Which explosion in the array to activate, xPosition of the center, yPosition of the center, Radius of the big circle, Duration of the explosion)
void Activate_Explosion(int Num, int xPos, int yPos, int Rad, int Duration)
{
  Serial.println("Explosion activated");
  if(Explosion[Num].Active)                                                             // If the explosion in that array slot is already active
  {
    return;                                                                             // Do not activate anything
  }
  else
  {
    Explosion[Num].Active = 1;                                                          // Make the explosion active
    // Fill the arguments in
    Explosion[Num].CenterCoords[0] = xPos;
    Explosion[Num].CenterCoords[1] = yPos;
    Explosion[Num].BigRad = Rad;
    Explosion[Num].Duration = Duration;
    
    Explosion[Num].ActiveTime = 0;                                                      // Reset the timer of the explosion
  }
}

// Activates an explosion with random parameters
void Activate_Explosion_Random()
{
  int Total = ExplosionsMax;                                                            // Get the max allowed number of explosions to be displayed
  int Num = -1;                                                                         // Set the explosions array slot as -1
  Serial.println("Random explosion activated");
  // This loop checks all the array slots until it finds the first unused slot so it can use it
  for(int i = 0; i < Total; i++)                                                        // Go through all the allowed explosion slots
  {
    if(Explosion[i].Active)                                                             // If that slot is already active and being used
    {
      continue;                                                                         // Restart the loop from the start
    }
    else                                                                                // If it is not being used
    {
      Serial.print("Found empty slot at ");
      Serial.println(i);
      Num = i;                                                                          // Set the slot number to the current unused slot
      break;                                                                            // Break the loop
    }
  }
  if(Num == -1)                                                                         // If no free slots were found
  {
    return;                                                                             // Finish the function without activating an explosion
  }
  // Next it checks if the laser is active, so that the explosions do not spawn too much inside the laser
  else if(Laser.Active)                                                                 // Check if the laser is active
  {
    if((Laser.TopLeftCoords[1] > 12) and ((Laser.TopLeftCoords[1] + Laser.Height) < 61))// If the laser has enough space for an explosion both above it and bellow it
    {
      int ExplosionAboveLaser = random(0,2);                                            // Randomly decide if the explosion is to be above the laser (1) or bellow it (0)
      int Radius;                                                                       // Create a variable to put the radius in
      int xCoord;                                                                       // Create a variable to put the horizontal position of the center in
      float Distance;                                                                   // Create a variable to put the distance between this explosion another explosion in (Distance between centers)
      bool WhileCheck;                                                                  // Create a boolean to work as the conditions for the do while loop
      int LoopDaLoopStartTime = millis();                                               // Create a variable that checks the start of the do while loop, so that it does not run for too long
      // The do while loops make a random position and radius within specified values, and then checks if the created circle is too close to other circles that already are active
      if(ExplosionAboveLaser)                                                           // If the circle is to be above the laser
      {
        do
        {
          if(Laser.TopLeftCoords[1] > 20)                                               // Check if there is enough space to make a circle with radius 20
          {
            Radius = random(12, 21);                                                    // If yes then set the max radius to 20
          }
          else
          {
            Radius = random(12, Laser.TopLeftCoords[1]);                                // If not than the max radius is the space remaining between the laser and the top of the screen, the minimum is always 12
          }
          xCoord = random((13 + Radius), (121 - Radius));                               // Set the center somewhere so that the circle with its radius do no exceed the left and right boundaries
          WhileCheck = true;                                                            // Set the check condition to true, so it repeats the loop
          for(int i = 0; i < Total; i++)                                                // For loop to check all explosions
          {
            if(Explosion[i].Active)                                                     // If the explosion is active
            {
              // Find the horizontal distance between the 2 explosions
              Distance = sqrt(((Explosion[i].CenterCoords[0] - xCoord) * (Explosion[i].CenterCoords[0] - xCoord)));
              if(Distance > max(Explosion[i].BigRad, Radius))                           // If the distance is bigger than the largest of the two radii (At least half the explosion is visible
              {
                WhileCheck = false;                                                     // Set the loop condition to false and proceed to checking the other explosions
              }
              else                                                                      // If it is not
              {
                WhileCheck = true;                                                      // Set the condition to true to make new random values and check all over again
                break;                                                                  // Break the for loop to stop checking the other values
              }
            }
            else                                                                        // If the explosion is not active
            {
              WhileCheck = false;                                                       // Set the condition to false
            }
          }
          Serial.println("LoopDaLoop");
          if((millis() - LoopDaLoopStartTime) > (LoopTime * 5))                         // Check to make sure that the loop has not been running for longer than 5 frames
          {
            Serial.println("Too many loops");
            return;                                                                     // If it has then stop the activation process and do not activate an explosion
          }
        } while(WhileCheck);                                                            // This loop stops if an explosion is made with an appropriate place and size
        Explosion[Num].CenterCoords[0] = xCoord;                                        // Set the x position of the explosion
        Explosion[Num].CenterCoords[1] = random(Radius, Laser.TopLeftCoords[1]);        // Set the y position of the explosion, so that at least half of it is visible over the laser and that it does not go out of boundary
        Explosion[Num].BigRad = Radius;                                                 // Set the radius of the explosion
      }
      else                                                                              // If the circle is bellow the laser
      {
        // Only the differences will be commented in this do while loop. For the rest read the first loop
        do
        {
          if((Laser.TopLeftCoords[1] + Laser.Height) < 42)                              // Same
          {
            Radius = random(12, 21);                                                    // Same
          }
          else
          {
            Radius = random(12, (64 - (Laser.TopLeftCoords[1] + Laser.Height)));        // The space remaining between the laser and the bottom of the screen instead
          }
          // All same from here
          xCoord = random((13 + Radius), (121 - Radius));
          WhileCheck = true;
          for(int i = 0; i < Total; i++)
          {
            if(Explosion[i].Active)
            {
              Distance = sqrt(((Explosion[i].CenterCoords[0] - xCoord) * (Explosion[i].CenterCoords[0] - xCoord)));
              if(Distance > max(Explosion[i].BigRad, Radius))
              {
                WhileCheck = false;
              }
              else
              {
                WhileCheck = true;
                break;
              }
            }
            else
            {
              WhileCheck = false;
            }
          }
          Serial.println("LoopDaLoop");
          if((millis() - LoopDaLoopStartTime) > (LoopTime * 5))
          {
            Serial.println("Too many loops");
            return;
          }
        } while(WhileCheck);
        Explosion[Num].CenterCoords[0] = xCoord;
        Explosion[Num].CenterCoords[1] = random((Laser.TopLeftCoords[1] + Laser.Height), 64 - Radius);
        Explosion[Num].BigRad = Radius;
      }
      Explosion[Num].Duration = random(2, 5) * 1000;                                    // Set the duration of the explosion between 2 and 4 seconds
      Explosion[Num].ActiveTime = 0;                                                    // Reset the active time of the explosion
      Explosion[Num].Active = 1;                                                        // Make the explosion active
      //Dump_Explosion(Num);                                                              // Dump the struct of the explosion in the terminal (Debug)
    }
    else if(Laser.TopLeftCoords[1] > 12)                                                // If there is not place both over and under the laser check if there is place only over
    {
      // If so do the exact same things as above, but with the explosions only showing over the laser
      int Radius;
      int xCoord;
      float Distance;
      bool WhileCheck;
      int LoopDaLoopStartTime = millis();
      do
      {
        if(Laser.TopLeftCoords[1] > 20)
        {
          Radius = random(12, 21);
        }
        else
        {
          Radius = random(12, Laser.TopLeftCoords[1]);
        }
        xCoord = random((13 + Radius), (121 - Radius));
        WhileCheck = true;
        for(int i = 0; i < Total; i++)
        {
          if(Explosion[i].Active)
          {
            Distance = sqrt(((Explosion[i].CenterCoords[0] - xCoord) * (Explosion[i].CenterCoords[0] - xCoord)));
            if(Distance > max(Explosion[i].BigRad, Radius))
            {
              WhileCheck = false;
            }
            else
            {
              WhileCheck = true;
              break;
            }
          }
          else
          {
            WhileCheck = false;
          }
        }
        Serial.println("LoopDaLoop");
        if((millis() - LoopDaLoopStartTime) > (LoopTime * 5))
        {
          Serial.println("Too many loops");
          return;
        }
      } while(WhileCheck);
      Explosion[Num].CenterCoords[0] = xCoord;
      Explosion[Num].CenterCoords[1] = random(Radius, Laser.TopLeftCoords[1]);
      Explosion[Num].BigRad = Radius;
      Explosion[Num].Duration = random(2, 5) * 1000;
      Explosion[Num].ActiveTime = 0;
      Explosion[Num].Active = 1;
      //Dump_Explosion(Num);
    }
    else if((Laser.TopLeftCoords[1] + Laser.Height) < 50)                               // If there is no place over the laser then check if there is place under it
    {
      // Do the same things as above, but the explosions only spawn under the laser
      int Radius;
      int xCoord;
      float Distance;
      bool WhileCheck;
      int LoopDaLoopStartTime = millis();
      do
      {
        if((Laser.TopLeftCoords[1] + Laser.Height) < 42)
        {
          Radius = random(12, 21);
        }
        else
        {
          Radius = random(12, (64 - (Laser.TopLeftCoords[1] + Laser.Height)));
        }
        xCoord = random((13 + Radius), (121 - Radius));
        WhileCheck = true;
        for(int i = 0; i < Total; i++)
        {
          if(Explosion[i].Active)
          {
            Distance = sqrt(((Explosion[i].CenterCoords[0] - xCoord) * (Explosion[i].CenterCoords[0] - xCoord)));
            if(Distance > max(Explosion[i].BigRad, Radius))
            {
              WhileCheck = false;
            }
            else
            {
              WhileCheck = true;
              break;
            }
          }
          else
          {
            WhileCheck = false;
          }
        }
        Serial.println("LoopDaLoop");
        if((millis() - LoopDaLoopStartTime) > (LoopTime * 5))
        {
          Serial.println("Too many loops");
          return;
        }
      } while(WhileCheck);
      Explosion[Num].CenterCoords[0] = xCoord;
      Explosion[Num].CenterCoords[1] = random((Laser.TopLeftCoords[1] + Laser.Height), 64 - Radius);
      Explosion[Num].BigRad = Radius;
      Explosion[Num].Duration = random(2, 5) * 1000;
      Explosion[Num].ActiveTime = 0;
      Explosion[Num].Active = 1;
      //Dump_Explosion(Num);
    }
    else                                                                                // If somehow there is not place to place the explosions (Which should not be possible, because the lasers always have at least 16 pixles above or bellow)
    {
      return;                                                                           // Do nothing
    }
  }
  else                                                                                  // If there is no laser
  {
    // Most of the process is the same, only difference is that now it checks the distance between this explosion and all the other explosions, and not just the horizontal distance
    int Radius;
    int xCoord;
    int yCoord;
    float Distance;
    bool WhileCheck;
    int LoopDaLoopStartTime = millis();
    do
    {
      Radius = random(12, 21);
      xCoord = random((13 + Radius), (121 - Radius));
      yCoord = random(Radius, (64 - Radius));
      WhileCheck = true;
      for(int i = 0; i < Total; i++)
      {
        if(Explosion[i].Active)
        {
          Distance = sqrt(((Explosion[i].CenterCoords[0] - xCoord) * (Explosion[i].CenterCoords[0] - xCoord)) + ((Explosion[i].CenterCoords[1] - yCoord) * (Explosion[i].CenterCoords[1] - yCoord)));
          if(Distance > max(Explosion[i].BigRad, Radius))
          {
            WhileCheck = false;
          }
          else
          {
            WhileCheck = true;
            break;
          }
        }
        else
        {
          WhileCheck = false;
        }
      }
      Serial.println("LoopDaLoop");
      if((millis() - LoopDaLoopStartTime) > (LoopTime * 5))
      {
        Serial.println("Too many loops");
        return;
      }
    } while(WhileCheck);
    Explosion[Num].CenterCoords[0] = xCoord;
    Explosion[Num].CenterCoords[1] = yCoord;
    Explosion[Num].BigRad = Radius;
    Explosion[Num].Duration = random(2, 5) * 1000;
    Explosion[Num].ActiveTime = 0;
    Explosion[Num].Active = 1;
    //Dump_Explosion(Num);
  }
}

// This function deactivates an explosion by returning its values to the initial struct values. Argument(The slot in the explosion array to deactivate)
void Deactivate_Explosion(int Num)
{
  Explosion[Num].Active = 0;
  Explosion[Num].DetectHit = false;
  Explosion[Num].CenterCoords[0] = -64;
  Explosion[Num].CenterCoords[1] = -64;
  Explosion[Num].BigRad = 0;
  Explosion[Num].SmallRad = 0;
  Explosion[Num].Duration = 0;
  Explosion[Num].ActiveTime = 0;
}

// This function displays the explosion warning then the explosion on the screen. Argument(The slot of the explosion in the array to be displayed)
void Display_Explosion(int Num)
{
  if(Explosion[Num].Active)                                                                                                   // If the explosion is active
  {
    Explosion[Num].ActiveTime += LoopTime;                                                                                    // Increase the activity time of the explosion
    if(Explosion[Num].ActiveTime < Explosion[Num].Speed)                                                                      // Is the explosion still in the warning state? (The time is less than its speed)
    {
      //Serial.println("Display 1");
      Explosion[Num].SmallRad = Explosion[Num].ActiveTime * Explosion[Num].BigRad / Explosion[Num].Speed;                     // Set the radius of the small explosion as a function of activity time, so that it expands until it reaches the big circle radius
      u8g2.drawCircle(Explosion[Num].CenterCoords[0], Explosion[Num].CenterCoords[1], Explosion[Num].SmallRad);               // Draw the small circle with its radius and the explosion's center
      u8g2.drawCircle(Explosion[Num].CenterCoords[0], Explosion[Num].CenterCoords[1], Explosion[Num].BigRad);                 // Draw the big circle with its radius and the explosion's center
    }
    else if(Explosion[Num].ActiveTime < (Explosion[Num].Duration + Explosion[Num].Speed))                                     // If it is done with the warning. Check if it has been active for less than its duration
    {
      //Serial.println("Display 2");
      Explosion[Num].DetectHit = true;                                                                                        // Allow for hit detection
      int AnimationOn = (Explosion[Num].ActiveTime % (Explosion[Num].AnimationSpeed * 2)) / Explosion[Num].AnimationSpeed;    // Check if the pixel need to removed from the radius due to the animation (Every "AnimationSpeed" milliseconds a pixel gets taken off, then after another "AnimationSpeed" milliseconds the pixel is back)
      int Radius = Explosion[Num].BigRad - AnimationOn;                                                                       // Make a local radius with the adjusted radius, so that the hit detector uses the real radius to detect
      u8g2.drawDisc(Explosion[Num].CenterCoords[0], Explosion[Num].CenterCoords[1], Radius);                                  // Draw a disc(Filled circle) with the explosions center and the adjusted radius
    }
    else                                                                                                                      // If the explosion active time has exceeded its duration
    {
      Deactivate_Explosion(Num);                                                                                              // Deactivate the explosion
      return;
    }
  }
  else                                                                                                                        // If thee explosion is not active
  {
    //Serial.println("Display 0");
    return;                                                                                                                   // Do not display anything
  }
}

// This function goes through the entire explosions array and gets them through the Display_Explosion function them one by one
void Draw_Explosions()
{
  int Total = ExplosionsMax;                                                                                                  // Get the maximum number of explosions
  for(int i = 0; i < Total; i++)                                                                                              // Cycle through all the explosions in the array to display
  {
    Display_Explosion(i);                                                                                                     // Run the funciton
  }
}



// Hitting functions
unsigned long LastHitTime = 0;                                                    // Records the last time the player was hit, so that it makes him invincible

// This function records the hit by removing one hitpoint off the player
void Record_Hit()
{
  if(Player.Type == 0)                                                            // If the player is invincible
  {
    return;                                                                       // Do nothing
  }
  else                                                                            // If he ain't
  {
    Player.Health -= 1;                                                           // Kill'em
    LastHitTime = GameTime;                                                       // And record when you killed'em
  }
}

// This function checks if the player was hit in the last 2 seconds to make him invincible
void Check_Invincible()
{
  if((GameTime - LastHitTime) > 2000)                                             // If it has been more than 2 seconds since the last hit
  {
    Serial.println("Not Invincible");
    Player.Type = 1;                                                              // Remove the player's invincibility status
    Player.Color = 2;                                                             // Return the bitmap color to normal (XoR mode)
  }
  else                                                                            // If it has been less than 2 seconds since the last hit
  {
    Serial.println("Invincible");
    Serial.print("Color is ");
    Serial.println(Player.Color);
    Player.Type = 0;                                                              // Set the player status to invincible
    Player.Color = ((millis() - LastHitTime) % 500) / 250;                        // Make the player flash black and white, where the colors change every 250 milliseconds
  }
}

// This function detects if a laser has hit a player
void Laser_Detect_Hit()
{
  if(Laser.Active == 0)                                                           // If the laser is not active
  {
    return;                                                                       // Do not do anything
  }
  else if(Player.Type == 0)                                                       // Else if the player is invincible
  {
    //Serial.println("Laser Detect Hit: Player is unbreakable");
    return;                                                                       // Do not do anything
  }
  else                                                                            // Otherwise start checking
  {
    // If the player's hitbox is inside the laser itself
    if((Player.TopLeftCoords[1] < (Laser.TopLeftCoords[1] + Laser.Height)) and (Player.BottomRightCoords[1] > Laser.TopLeftCoords[1]) and (Player.BottomRightCoords[0] > (Laser.TopLeftCoords[0] - 1)))
    {
      //Serial.println("Laser hit recorded");
      Record_Hit();                                                               // Record the hit
    }
    else                                                                          // Otherwise
    {
      //Serial.println("Laser hit not recorded");
      return;                                                                     // Ma boi ain't hit
    }
  }
}

// This function detects if an explosion has hit the player
void Explosion_Hit_Detect(int Num)
{
  if(Explosion[Num].Active == 0)                                                  // If the explosion is not active
  {
    return;                                                                       // Don't do anything
  }
  else if(Player.Type == 0)                                                       // If the player is invincible
  {
    //Serial.print("Explosion Detect Hit: Player is unbreakable ");
    //Serial.println(Num);
    return;                                                                       // Spaze Game Bizzare Adventure: Player is unbreakable
  }
  else if(Explosion[Num].DetectHit)                                               // If the explosion is to have it's hitbox detected
  {
    // Find the distance between the center of the explosion and the 4 edge points of the player's hitbox
    // The use the closest on of these distances to see if it is under the radius of the explosion. If it is record the hit
    // This calculation is not perfect, as if the players center top is the one that enters the circle, then it will not count until one of the edges enter the circle.
    
    float Distance[4];
    Distance[0] = sqrt(((Player.TopLeftCoords[0] - Explosion[Num].CenterCoords[0]) * (Player.TopLeftCoords[0] - Explosion[Num].CenterCoords[0])) + ((Player.TopLeftCoords[1] - Explosion[Num].CenterCoords[1]) * (Player.TopLeftCoords[1] - Explosion[Num].CenterCoords[1])));                  // Distance from center of explosion to top left of the ship hitbox
    Distance[1] = sqrt(((Player.BottomRightCoords[0] - Explosion[Num].CenterCoords[0]) * (Player.BottomRightCoords[0] - Explosion[Num].CenterCoords[0])) + ((Player.TopLeftCoords[1] - Explosion[Num].CenterCoords[1]) * (Player.TopLeftCoords[1] - Explosion[Num].CenterCoords[1])));          // Distance from center of explosion to top right of the ship hitbox
    Distance[2] = sqrt(((Player.TopLeftCoords[0] - Explosion[Num].CenterCoords[0]) * (Player.TopLeftCoords[0] - Explosion[Num].CenterCoords[0])) + ((Player.BottomRightCoords[1] - Explosion[Num].CenterCoords[1]) * (Player.BottomRightCoords[1] - Explosion[Num].CenterCoords[1])));          // Distance from center of explosion to bottom left of the ship hitbox
    Distance[3] = sqrt(((Player.BottomRightCoords[0] - Explosion[Num].CenterCoords[0]) * (Player.BottomRightCoords[0] - Explosion[Num].CenterCoords[0])) + ((Player.BottomRightCoords[1] - Explosion[Num].CenterCoords[1]) * (Player.BottomRightCoords[1] - Explosion[Num].CenterCoords[1])));  // Distance from center of explosion to bottom right of the ship hitbox
    //float MinDistance[3];
    //MinDistance[0] = min(Distance[0], Distance[1]);
    //MinDistance[1] = min(MinDistance[0], Distance[2]);
    //MinDistance[2] = min(MinDistance[1], Distance[3]);
    float MinDistance;
    MinDistance = min(Distance[0], Distance[1]);
    MinDistance = min(MinDistance, Distance[2]);
    MinDistance = min(MinDistance, Distance[3]);
    if(MinDistance < Explosion[Num].BigRad)
    {
      //Serial.println("Explosion hit recorded");
      Record_Hit();
      Check_Invincible();                                                         // Check invincibility to make the player invincible, because otherwise if the player is inside two explsions, then he will lose 2 hitpoints
    }
    else
    {
      //Serial.println("Explosion hit not recorded");
      return;
    }
  }
  else                                                                            // If thee explosion's hitbox is not to be detected
  {
    return;                                                                       // Don't record the hit
  }
}

// This function runs the detect hit functions from all the hazards
void Detect_Hit()
{
  //Serial.println("Detect Hit Ran 1");
  Laser_Detect_Hit();                                                             // Detect a hit from the laser
  //Serial.println("Detect Hit Ran 2");
  for(int i = 0; i < ExplosionsMax; i++)                                          // Cycle through all the explosions
  {
    Explosion_Hit_Detect(i);                                                      // Detect a hit from each explosion
  }
}
