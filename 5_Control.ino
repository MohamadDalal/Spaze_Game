unsigned long MNLastRan = 0;                    // This is used to not register menu navigation controlls too fast
unsigned long LBLastRan = 0;                    // This is used to not register left button presses too fast
unsigned long RBLastRan = 0;                    // This is used to not register right button presses too fast

float PitchDZMax = 15;                          // Vertical (Pitch) movement max deadzone
float PitchDZMin = 5;                           // Vertical (Pitch) movement min deadzone
float RollYawDZMax = 15;                        // Horizontal (Roll and Yaw) movement max deadzone
float RollYawDZMin = 5;                         // Horizontal (Roll and Yaw) movement min deadzone

float MoveVert = 0;                             // This is used to make the pixels move vertically with float speeds, as pixels need to moved by integer values
float MoveHor = 0;                              // This is used to make the pixels move horizontally with float speeds, as pixels need to moved by integer values

// Function to navigate through the menu using gyroscope and tilting motion
void Menu_Navigation()
{
  if(gy > 150)                                  // Check if the device has been tilted down, by checking for high y-axis positive rotation speed
  {
    if((millis() - MNLastRan) < 250)            // Check if the tilt has already been activated in the last 250ms, if yes then don't activate a new one
    {
      Serial.println("MN Millis effect");
      return;
    }
    Serial.println("Down it goes");
    MenuCursor = (MenuCursor + 1) % 3;          // Go through the list {0, 1, 2} forwards    
    MNLastRan = millis();                       // Set the last time this function has run for the millis chack
  }
  else if(gy < -150)                            // Check if the device has been tilted up, by checking for high y-axis negative rotation speed
  {
    if((millis() - MNLastRan) < 250)            // Check if the tilt has already been activated in the last 250ms, if yes then don't activate a new one
    {
      Serial.println("MN Millis effect");
      return;
    }
    Serial.println("Up it goes");
    MenuCursor = (3 - 1 + MenuCursor) % 3;      // Go through the list {0, 1, 2} backwards. I love this function, I was able to prove it by induction in my LiAl notebook.
    MNLastRan = millis();                       // Set the last time this function has run for the millis chack
  }
}

// Function to navigate through the pages using gyroscope and tilting motion. Uses the same millis check as the menu navigation, since these two function don't run in the same menu
void Page_Navigation(int PageCount)
{
  if(gy > 150)                                  // Check if the device has been tilted down, by checking for high y-axis positive rotation speed
  {
    if((millis() - MNLastRan) < 250)            // Check if the tilt has already been activated in the last 250ms, if yes then don't activate a new one
    {
      Serial.println("MN Millis effect");
      return;
    }
    Serial.println("Down it goes");
    Page = (Page + 1) % PageCount;              // Go through the cyclic set with PageCount cardinality forwards    
    MNLastRan = millis();                       // Set the last time this function has run for the millis check
  }
  else if(gy < -150)                            // Check if the device has been tilted up, by checking for high y-axis negative rotation speed
  {
    if((millis() - MNLastRan) < 250)            // Check if the tilt has already been activated in the last 250ms, if yes then don't activate a new one
    {
      Serial.println("MN Millis effect");
      return;
    }
    Serial.println("Up it goes");
    Page = (PageCount - 1 + Page) % PageCount;  // Go through the cyclic set with PageCount cardinality backwards. I love this function, I was able to prove its generality by induction
    MNLastRan = millis();                       // Set the last time this function has run for the millis chack
  }
}

// Function to check if the left button has been pressed
bool LB_Press()
{
  //Serial.println("LB_Press Ran");
  if((millis() - LBLastRan) < 250)              // Check if the left button has been registered in the last 250ms, if so then don't register this press
  {
    Serial.println("LB Millis effect");
    return false;
  }
  if(InvertButtons)
  {
    //Serial.print("GPIO35 is");
    //Serial.println(digitalRead(35));
    //Serial.println("Buttons inverted")
    if (digitalRead(35) == 0)                     // Is the button pressed (GPIO 35 is LOW)
    {
      Serial.println("LB Pressed");
      LBLastRan = millis();                       // Set the last time this function has run for the millis chack
      return true;                                // Return a true
    }
    else                                          // If it is not pressed (GPIO 35 is HIGH)
    {
      return false;                               // Return False
    }
  }
  else
  {
    //Serial.print("GPIO34 is");
    //Serial.println(digitalRead(34));
    if (digitalRead(34) == 0)                     // Is the button pressed (GPIO 34 is LOW)
    {
      Serial.println("LB Pressed");
      LBLastRan = millis();                       // Set the last time this function has run for the millis chack
      return true;                                // Return a true
    }
    else                                          // If it is not pressed (GPIO 34 is HIGH)
    {
      return false;                               // Return False
    }
  }
}

// Function to check if the right button has been pressed
bool RB_Press()
{
  if((millis() - RBLastRan) < 250)              // Check if the right button has been registered in the last 250ms, if so then don't register this press
  {
    Serial.println("RB Millis effect");
    return false;
  }
  if(not InvertButtons)
  {
    //Serial.print("GPIO35 is");
    //Serial.println(digitalRead(35));
    if (digitalRead(35) == 0)                     // Is the button pressed (GPIO 35 is LOW)
    {
      Serial.println("RB Pressed");
      RBLastRan = millis();                       // Set the last time this function has run for the millis chack
      return true;                                // Return a true
    }
    else                                          // If it is not pressed (GPIO 35 is HIGH)
    {
      return false;                               // Return False
    }
  }
  else
  {
    //Serial.print("GPIO34 is");
    //Serial.println(digitalRead(34));
    //Serial.println("Buttons inverted")
    if (digitalRead(34) == 0)                     // Is the button pressed (GPIO 34 is LOW)
    {
      Serial.println("RB Pressed");
      RBLastRan = millis();                       // Set the last time this function has run for the millis chack
      return true;                                // Return a true
    }
    else                                          // If it is not pressed (GPIO 34 is HIGH)
    {
      return false;                               // Return False
    }
  }
}

// Not for the button functions: They do not have anything to differentiate between hold and a single click, so turbo mode is always on (Holding = Multiple click as fast as possible)

// Detect if the game pausing movement has been activated (Currently if a force stronger than 1.5g has been detected)
bool Pause_Game()
{
  Gyro_Read();                                  // Read the data from the gyroscope
  if(az >1.5)
  //if(ax > 1.5 or ay > 1.5 or az >1.5)           // Is there an acceleration stronger than 1.5g?
  {
    return true;                                // If yes return true
  }
  else
  {
    return false;                               // If no return false
  }
}

// The comments for all moving functions are at the start of the Yaw moving function. I am not commenting all 6 almost identical procedures
// This function reads the Pitch rotation adjusted with the calibrators to detect vertical movement
// The way the pixles are moved is explained in a paper somewhere in my Linear Algebra notebook
void Game_Move_Vert()
{
  int MoveVertInt;
  float Speed = Player.Speed;
  float fps = 1000.0 / LoopTime;
  // Positive pitch is down
  // Negtive pitch is up
  Gyro_Read();

  // All 6 of these movements work the same, just using different angles, I commented one so that's enough for now
  if(Pitch > PitchDZMin)    // Down
  {
    Serial.println("Down the ship goes");
    /*
    if(Player.BottomRightCoords[1] >= 61)
    {
      Serial.println("Ship went too much down");
      return;
    }
    */
    if(MoveVert < 0)
    {
      MoveVert = 0;
    }
    if(Pitch > PitchDZMax)
    {
      //Serial.print("MoveVert before calcualtions is: ");
      //Serial.println(MoveVert);
      MoveVert += (Speed / fps);
      //MoveVert += (Speed / (1000 / LoopTime));
      MoveVertInt = MoveVert;
      //Serial.print("MoveVert after calcualtions 1 is: ");
      //Serial.println(MoveVert);
      if((Player.BottomRightCoords[1] + MoveVertInt) >= 63)
      {
        Serial.println("Ship went too much down");
        return;
      }
      //Serial.print("MoveVertInt is: ");
      //Serial.println(MoveVertInt);
      Player.TopLeftCoords[1] += MoveVertInt;
      Player.BottomRightCoords[1] += MoveVertInt;
      MoveVert -= MoveVertInt;
      //Serial.print("MoveVert after calcualtions 2 is: ");
      //Serial.println(MoveVert);
    }
    else
    {
      //Serial.print("MoveVert before calcualtions is: ");
      //Serial.println(MoveVert);
      MoveVert += ((Pitch - PitchDZMin)/(PitchDZMax - PitchDZMin)) * (Speed / fps);
      //MoveVert += ((Pitch - PitchDZMin)/(PitchDZMax - PitchDZMin)) * (Speed / (1000 / LoopTime));
      MoveVertInt = MoveVert;
      //Serial.print("MoveVert after calcualtions 1 is: ");
      //Serial.println(MoveVert);
      if((Player.BottomRightCoords[1] + MoveVertInt) >= 63)
      {
        Serial.println("Ship went too much down");
        return;
      }
      //Serial.print("MoveVertInt is: ");
      //Serial.println(MoveVertInt);
      Player.TopLeftCoords[1] += MoveVertInt;
      Player.BottomRightCoords[1] += MoveVertInt;
      MoveVert -= MoveVertInt;
      //Serial.print("MoveVert after calcualtions 2 is: ");
      //Serial.println(MoveVert);
    }
  }

  // All 6 of these movements work the same, just using different angles, I commented one so that's enough for now
  if(Pitch < -PitchDZMin)    // Up
  {
    Serial.println("Up the ship goes");
    /*
    if(Player.TopLeftCoords[1] <= 2)
    {
      Serial.println("Ship went too much up");
      return;
    }
    */
    if(MoveVert > 0)
    {
      MoveVert = 0;
    }
    if(Pitch < -PitchDZMax)
    {
      //Serial.print("MoveVert before calcualtions is: ");
      //Serial.println(MoveVert);
      MoveVert -= (Speed / fps);
      //MoveVert -= (Speed / (1000 / LoopTime));
      MoveVertInt = MoveVert;
      //Serial.print("MoveVert after calcualtions 1 is: ");
      //Serial.println(MoveVert);
      if((Player.TopLeftCoords[1] + MoveVertInt) <= 0)
      {
        Serial.println("Ship went too much up");
        return;
      }
      //Serial.print("MoveVertInt is: ");
      //Serial.println(MoveVertInt);
      Player.TopLeftCoords[1] += MoveVertInt;
      Player.BottomRightCoords[1] += MoveVertInt;
      MoveVert -= MoveVertInt;
      //Serial.print("MoveVert after calcualtions 2 is: ");
      //Serial.println(MoveVert);
    }
    else
    {
      //Serial.print("MoveVert before calcualtions is: ");
      //Serial.println(MoveVert);
      MoveVert += ((Pitch + PitchDZMin)/(PitchDZMax - PitchDZMin)) * (Speed / fps);
      //MoveVert += ((Pitch + PitchDZMin)/(PitchDZMax - PitchDZMin)) * (Speed / (1000 / LoopTime));
      MoveVertInt = MoveVert;
      //Serial.print("MoveVert after calcualtions 1 is: ");
      //Serial.println(MoveVert);
      if((Player.TopLeftCoords[1] + MoveVertInt) <= 0)
      {
        Serial.println("Ship went too much up");
        return;
      }
      //Serial.print("MoveVertInt is: ");
      //Serial.println(MoveVertInt);
      Player.TopLeftCoords[1] += MoveVertInt;
      Player.BottomRightCoords[1] += MoveVertInt;
      MoveVert -= MoveVertInt;
      //Serial.print("MoveVert after calcualtions 2 is: ");
      //Serial.println(MoveVert);
    }
  }
}

// This function reads the Roll rotation adjusted with the calibrators to detect Horizontal movement
// The way the pixles are moved is explained in a paper somewhere in my Linear Algebra notebook
void Game_Move_Roll_Hor()
{
  int MoveHorInt;
  float Speed = Player.Speed;
  float fps = 1000.0 / LoopTime;
  // Positive Roll is left
  // Negative Roll is right
  Gyro_Read();

  // All 6 of these movements work the same, just using different angles, I commented one so that's enough for now
  if(Roll > RollYawDZMin)    // Left
  {
    Serial.println("Left the ship goes with Roll");
    /*
    if(Player.TopLeftCoords[0] <= 2)
    {
      Serial.println("Ship went too much left ");
      return;
    }
    */
    if(MoveHor > 0)
    {
      MoveHor = 0;
    }
    if(Roll > RollYawDZMax)
    {
      MoveHor -= (Speed / fps);
      //MoveHor -= (Speed / (1000 / LoopTime));
      MoveHorInt = int(MoveHor);
      if((Player.TopLeftCoords[0] + MoveHorInt) <= 13)
      {
        Serial.println("Ship went too much left ");
        return;
      }
      Player.TopLeftCoords[0] += MoveHorInt;
      Player.BottomRightCoords[0] += MoveHorInt;
      MoveHor -= MoveHorInt;
    }
    else
    {
      MoveHor -= ((Roll - RollYawDZMin)/(RollYawDZMax - RollYawDZMin)) * (Speed / fps);
      //MoveHor -= ((Roll - RollYawDZMin)/(RollYawDZMax - RollYawDZMin)) * (Speed / (1000 / LoopTime));
      MoveHorInt = int(MoveHor);
      if((Player.TopLeftCoords[0] + MoveHorInt) <= 13)
      {
        Serial.println("Ship went too much left ");
        return;
      }
      Player.TopLeftCoords[0] += MoveHorInt;
      Player.BottomRightCoords[0] += MoveHorInt;
      MoveHor -= MoveHorInt;
    }
  }

  // All 6 of these movements work the same, just using different angles, I commented one so that's enough for now
  if(Roll < -RollYawDZMin)    // Right
  {
    Serial.println("Right the ship goes with Roll");
    /*
    if(Player.BottomRightCoords[0] >= 125)
    {
      Serial.println("Ship went too much right");
      return;
    }
    */
    if(MoveHor < 0)
    {
      MoveHor = 0;
    }
    if(Roll < -RollYawDZMax)
    {
      MoveHor += (Speed / fps);
      //MoveHor += (Speed / (1000 / LoopTime));
      MoveHorInt = int(MoveHor);
      if((Player.BottomRightCoords[0] + MoveHorInt) >= 120)
      {
        Serial.println("Ship went too much right");
        return;
      }
      Player.TopLeftCoords[0] += MoveHorInt;
      Player.BottomRightCoords[0] += MoveHorInt;
      MoveHor -= MoveHorInt;
    }
    else
    {
      MoveHor -= ((Roll + RollYawDZMin)/(RollYawDZMax - RollYawDZMin)) * (Speed / fps);
      //MoveHor -= ((Roll + RollYawDZMin)/(RollYawDZMax - RollYawDZMin)) * (Speed / (1000 / LoopTime));
      MoveHorInt = int(MoveHor);
      if((Player.BottomRightCoords[0] + MoveHorInt) >= 120)
      {
        Serial.println("Ship went too much right");
        return;
      }
      Player.TopLeftCoords[0] += MoveHorInt;
      Player.BottomRightCoords[0] += MoveHorInt;
      MoveHor -= MoveHorInt;
    }
  }
}

// This function reads the Yaw rotation adjusted with the calibrators to detect Horizontal movement
// The way the pixles are moved is explained in a paper somewhere in my Linear Algebra notebook
void Game_Move_Yaw_Hor()
{
  int MoveHorInt;                                               // Used to round down the moving value into an integer
  float Speed = Player.Speed;                                   // Convert the player speed to a local float
  float fps = 1000.0 / LoopTime;                                // Get the actual fps to work with, so that the player has the same speed in all fps options
  // Positive Yaw is left
  // Negative Yaw is right
  Gyro_Read;                                                    // Read the gyroscope data
  
  if(Yaw > RollYawDZMin)                                        // Is the Yaw angle higher than the min deadzone? (Left)
  {
    Serial.println("Left the ship goes with Yaw");
    /*
    if(Player.TopLeftCoords[0] <= 2)                            // Is the player at the left edge?  
    {
      Serial.println("Ship went too much left ");
      return;                                                   // If so return without moving a pixle
    }
    */
    if(MoveHor > 0)                                             // Was the last movement to the right? (A positive pixle moving value)
    {
      MoveHor = 0;                                              // Reset the pixle moving value
    }
    if(Yaw > RollYawDZMax)                                      // If the Yaw angle is higher than the max deadzone
    {
      MoveHor -= (Speed / fps);                                 // Full speed to the left (No angle ratio calculation)
      //MoveHor -= (Speed / (1000 / LoopTime));
      MoveHorInt = int(MoveHor);                                // Make an integer, because pixles move in integers (If the moving thing is more than 1 then round down the value, else no moevement (Value is 0))
      if((Player.TopLeftCoords[0] + MoveHorInt) <= 13)          // Check if the ship will surpass the left boundary after moving
      {
        Serial.println("Ship went too much left ");
        return;                                                 // If so then don't move the ship. This is a better method than just preventing movement at the start fo the function, as was done before
      }
      Player.TopLeftCoords[0] += MoveHorInt;                    // Move the value of the top left pixle (This is the one used in the draw function)
      Player.BottomRightCoords[0] += MoveHorInt;                // Move the value of the bottom right pixle (This is the one used in the edge detection)
      MoveHor -= MoveHorInt;                                    // If it actually moved some pixles, then remove this pixles from the movement value, so it does not move extra pixles next time this function is called
    }
    else                                                        // If it is between the two deadzones
    {
      MoveHor -= ((Yaw - RollYawDZMin)/(RollYawDZMax - RollYawDZMin)) * (Speed / fps);    // Make a linear ration from the angle and mutiply it with the speed
      //MoveHor -= ((Yaw - RollYawDZMin)/(RollYawDZMax - RollYawDZMin)) * (Speed / (1000 / LoopTime));
      
      // This is literally the same lines as above, just read the comments above
      MoveHorInt = int(MoveHor);
      if((Player.TopLeftCoords[0] + MoveHorInt) <= 13)
      {
        Serial.println("Ship went too much left ");
        return;
      }
      Player.TopLeftCoords[0] += MoveHorInt;
      Player.BottomRightCoords[0] += MoveHorInt;
      MoveHor -= MoveHorInt;
    }
  }

  // All 6 of these movements work the same, just using different angles, I commented one so that's enough
  if(Yaw < -RollYawDZMin)    // Right
  {
    Serial.println("Right the ship goes with Yaw");
    /*
    if(Player.BottomRightCoords[0] >= 125)
    {
      Serial.println("Ship went too much right");
      return;
    }
    */
    if(MoveHor < 0)
    {
      MoveHor = 0;
    }
    if(Yaw < -RollYawDZMax)
    {
      MoveHor += (Speed / fps);
      //MoveHor += (Speed / (1000 / LoopTime));
      MoveHorInt = int(MoveHor);
      if((Player.BottomRightCoords[0] + MoveHorInt) >= 120)
      {
        Serial.println("Ship went too much right");
        return;
      }
      Player.TopLeftCoords[0] += MoveHorInt;
      Player.BottomRightCoords[0] += MoveHorInt;
      MoveHor -= MoveHorInt;
    }
    else
    {
      MoveHor -= ((Yaw + RollYawDZMin)/(RollYawDZMax - RollYawDZMin)) * (Speed / fps);
      //MoveHor -= ((Yaw + RollYawDZMin)/(RollYawDZMax - RollYawDZMin)) * (Speed / (1000 / LoopTime));
      MoveHorInt = int(MoveHor);
      if((Player.BottomRightCoords[0] + MoveHorInt) >= 120)
      {
        Serial.println("Ship went too much right");
        return;
      }
      Player.TopLeftCoords[0] += MoveHorInt;
      Player.BottomRightCoords[0] += MoveHorInt;
      MoveHor -= MoveHorInt;
    }
  }
}
