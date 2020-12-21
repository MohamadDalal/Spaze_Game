unsigned long MNLastRan = 0;

// Function to navigate through the menu using gyroscope and tilting motion
void Menu_Navigation()
{
  if(gy > 150) //Down
  {
    if((millis() - MNLastRan) < 250)
    {
      Serial.println("Millis effect");
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
      Serial.println("Millis effect");
      return;
    }
    Serial.println("Up it goes");
    MenuCursor = (3 - 1 + MenuCursor) % 3;
    MNLastRan = millis();
  }
}
