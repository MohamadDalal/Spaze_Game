// The reset function for when the game ends and the stats function to calculate scores and what not

/*                                        This part is for running the game. It is not finished yet, as planning it took too much time.
unsigned long LastLevelChange = 0;
unsigned long LastExplosionActive = 0;
unsigned long LastLaserActive = 0;
unsigned long LastVertLaserActive = 0;

void Run_Hazards()
{
  int ExplosionsWithLaser = 3;
  if((GameTime - LastLevelChange) > 60000)
  {
    Level ++;
    LastLevelChange = GameTime;
  }
  switch(Level)
  {
    case 0:                                                           // 6 seconds nothing. 24 seconds of only 16 pixels lasers. 30 seconds with 3 explosions too
      if(GameTime < 6000)
      {
        return;
      }
      else if(GameTime < 30000)
      {
        if((GameTime - LastLaserActive) > 6000)
        {
          Activate_Laser_Random(0);
          LastLaserActive = GameTime;
        }
      }
      else
      {
        ExplosionsMax = 3;
        if((GameTime - LastLaserActive) > 6000)
        {
          Activate_Laser_Random(0);
          LastLaserActive = GameTime;
        }
        if((GameTime - LastExplosionActive) > 2000)
        {
          Activate_Explosion_Random();
          LastExplosionActive = GameTime;
        }
      }
      return;
    case 1:                                                           // Still 16 pixel lasers which spawn less frequently so explosions have a chance to spawn 5. Spawn only 3 explosions when lasers active.
      if(Laser.Active)
      {
        ExplosionsMax = ExplosionsWithLaser;
      }
      else
      {
        ExplosionsMax = 5;
      }
      if((GameTime - LastLaserActive) > 10000)
      {
        //int BigLaser = random(0, 2);
        Activate_Laser_Random(0);
        LastLaserActive = GameTime;
      }
      if((GameTime - LastExplosionActive) > 2000)
      {
        Activate_Explosion_Random();
        LastExplosionActive = GameTime;
      }
      return;
    case 2:                                                           // Introduction to the vertical laser. It has the slowest speed (25), and 32 safe and fire pixels.
      if((GameTime - LastVertLaserActive) > 30000)
      {
        Activate_VertLaser_Random();
        LastVertLaserActive = GameTime;
      }
      return;
    case 3:                                                           // Back to the laser. Now it can go up to 32 pixels. Same frequency as level 1 and same explosions.
      if(Laser.Active)
      {
        ExplosionsMax = ExplosionsWithLaser;
      }
      else
      {
        ExplosionsMax = 5;
      }
      if((GameTime - LastLaserActive) > 10000)
      {
        //int BigLaser = random(0, 2);
        Activate_Laser_Random(0);
        LastLaserActive = GameTime;
      }
      if((GameTime - LastExplosionActive) > 2000)
      {
        Activate_Explosion_Random();
        LastExplosionActive = GameTime;
      }
      return;
    case 4:                                                           // The vertical laser is back and the same accompanied by 3 explosions.
      ExplosionsMax = 3;
      if((GameTime - LastVertLaserActive) > 30000)
      {
        Activate_VertLaser_Random();
        LastVertLaserActive = GameTime;
      }
      if((GameTime - LastExplosionActive) > 2000)
      {
        Activate_Explosion_Random();
        LastExplosionActive = GameTime;
      }
      return;
    default:
      if((Level % 5) == 0)                                            // Explosion rounds. 10 explosions that spawn a second from each other
      {
        ExplosionsMax = 10;
        if((GameTime - LastExplosionActive) > 1000)
        {
          Activate_Explosion_Random();
          LastExplosionActive = GameTime;
        }
      }
      else if((Level % 2) == 1)                                       // Laser rounds. The level decides how often a big laser spawns. Explosion count is at 5
      {
        
      }
      else                                                            // Vertical laser round. Speed increases with level. Explosion count increases with level (Max 8)
      {
        
      }
      return;
  }
}
*/

// Reset the game when going back to the main menu
void Reset_Game()
{
  GameTime = 0;                             // Reset the game time, as we have left the game
  Deactivate_Laser();                       // Deactivate the lasers
  Deactivate_VertLaser();                   // Deactivate the vertical laser
  ExplosionsMax = 10;                       // Set the explosions to the max number, so all can get properly deactivated
  for(int i = 0; i < ExplosionsMax; i++)    // Go through all the explosions and disable them
  {
    Deactivate_Explosion(i);
  }
}

/* When the game is done, make a way to calculate score and display it in the game over screen
void Calculate_Score()
{
  
}
*/
