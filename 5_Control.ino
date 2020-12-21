unsigned long MNLastRan = 0;
unsigned long LBLastRan = 0;
unsigned long RBLastRan = 0;

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
