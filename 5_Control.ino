unsigned long MNLastRan = 0;
unsigned long LBLastRan = 0;
unsigned long RBLastRan = 0;

float PitchDZMax = 15;
float PitchDZMin = 5;
float RollYawDZMax = 30;
float RollYawDZMin = 10;

float MoveVert = 0;
float MoveHor = 0;

// Function to navigate through the menu using gyroscope and tilting motion
void Menu_Navigation()
{
  if(gy > 150) //Down
  {
    if((millis() - MNLastRan) < 250)
    {
      Serial.println("MN Millis effect");
      return;
    }
    Serial.println("Down it goes");
    MenuCursor = (MenuCursor + 1) % 3;           // Suggestion is to make it a modulus calculation where it is mod the number of items in the menu, then it can go around the items
    MNLastRan = millis();
  }
  else if(gy < -150) //Up
  {
    if((millis() - MNLastRan) < 250)
    {
      Serial.println("MN Millis effect");
      return;
    }
    Serial.println("Up it goes");
    MenuCursor = (3 - 1 + MenuCursor) % 3;
    MNLastRan = millis();
  }
}

bool LB_Press()
{
  //Serial.println("LB_Press Ran");
  if((millis() - LBLastRan) < 250)
  {
    Serial.println("LB Millis effect");
    return false;
  }
  //Serial.print("GPIO32 is");
  //Serial.println(digitalRead(32));
  if (digitalRead(32) == 1)
  {
    Serial.println("LB Pressed");
    LBLastRan = millis();
    return true;
  }
  else
  {
    return false;
  }
}

bool RB_Press()
{
  if((millis() - RBLastRan) < 250)
  {
    Serial.println("RB Millis effect");
    return false;
  }
  if (digitalRead(33) == 1)
  {
    Serial.println("RB Pressed");
    RBLastRan = millis();
    return true;
  }
  else
  {
    return false;
  }
}

void Game_Move_Vert()
{
  int MoveVertInt;
  // Positive pitch is down
  // Negtive pitch is up
  Gyro_Read();
  
  if(Pitch > PitchDZMin)    // Down Switched to UP
  {
    Serial.println("Down the ship goes");
    if(Player.BottomRightCoords[1] >= 63)
    {
      Serial.println("Ship went too much down");
      return;
    }
    if(MoveVert > 0)
    {
      MoveVert = 0;
    }
    if(Pitch > PitchDZMax)
    {
      MoveVert += (Player.Speed / 10);
      MoveVertInt = int(MoveVert);
      Player.TopLeftCoords[1] += MoveVertInt;
      Player.BottomRightCoords[1] += MoveVertInt;
      MoveVert -= MoveVertInt;
    }
    else
    {
      MoveVert += ((Pitch - PitchDZMin)/(PitchDZMax - PitchDZMin)) * (Player.Speed / 10);
      MoveVertInt = int(MoveVert);
      Player.TopLeftCoords[1] += MoveVertInt;
      Player.BottomRightCoords[1] += MoveVertInt;
      MoveVert -= MoveVertInt;
    }
  }

  
  if(Pitch < -PitchDZMin)    // Up Switched to Down
  {
    Serial.println("Up the ship goes");
    if(Player.TopLeftCoords[1] <= 0)
    {
      Serial.println("Ship went too much up");
      return;
    }
    if(MoveVert < 0)
    {
      MoveVert = 0;
    }
    if(Pitch < -PitchDZMax)
    {
      MoveVert -= (Player.Speed / 10);
      MoveVertInt = int(MoveVert);
      Player.TopLeftCoords[1] += MoveVertInt;
      Player.BottomRightCoords[1] += MoveVertInt;
      MoveVert -= MoveVertInt;
    }
    else
    {
      MoveVert += ((Pitch + PitchDZMin)/(PitchDZMax - PitchDZMin)) * (Player.Speed / 10);
      MoveVertInt = int(MoveVert);
      Player.TopLeftCoords[1] += MoveVertInt;
      Player.BottomRightCoords[1] += MoveVertInt;
      MoveVert -= MoveVertInt;
    }
  }
}


void Game_Move_Hor()
{
  // Positive Roll is left
  // Negative Roll is right
  // Yaw is unused Might make an option to change controlls from Roll to Yaw
  Gyro_Read;
}
