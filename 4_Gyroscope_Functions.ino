float ax, ay, az, gx, gy, gz, temp, Pitch, Roll, Yaw, PitchCal, RollCal, YawCal;
//float cal = 0.00;
//int calCounter = 0;
//int lightMode = 0;

//char dir[12];

// Adjusted version from Jens' barometer
void Gyro_Init()
{
  delay(100);
  while (Gyroscope.wakeup() == false)
  {
    Serial.print(millis());
    Serial.println("\tCould not connect to the gyroscope");
    //Menu = 2; // Errors screen
    //ErrorMenu = 1; // Could not connect to Gyroscope
    u8g2.clearBuffer();
    u8g2.drawStr(0, 16, "No Gyroscope detected");
    u8g2.drawStr(8, 32, "Please check that all");
    u8g2.drawStr(8, 48, "wires are connected");
    u8g2.sendBuffer();
    delay(1000);
  }
  Gyroscope.setAccelSensitivity(0);  // 2g
  Gyroscope.setGyroSensitivity(0);   // 250 degrees/s

  Gyroscope.setThrottle(false);
  Serial.println("start...");

  Gyroscope.axe = 0;
  Gyroscope.aye = 0;
  Gyroscope.aze = 0;
  Gyroscope.gxe = 0;
  Gyroscope.gye = 0;
  Gyroscope.gze = 0;
  PitchCal = 0;
  YawCal = 0;
  RollCal = 0;
}

// Adjusted version of the calibration example from the GY521 library
void Gyro_Slow_Cal()
{ 
  //Gyroscope.setThrottle(false);
  for(int i = 0; i < 100; i++)
  {
    
    ax = ay = az = 0;
    gx = gy = gz = 0;
    temp = 0;
    for (int i = 0; i < 20; i++)
    {
      Gyroscope.read();
      ax -= Gyroscope.getAngleX();
      ay -= Gyroscope.getAngleY();
      az -= Gyroscope.getAngleZ();
      gx -= Gyroscope.getGyroX();
      gy -= Gyroscope.getGyroY();
      gz -= Gyroscope.getGyroZ();
      temp += Gyroscope.getTemperature();
    }

    // adjust calibration errors so table should get all zero's.
    Gyroscope.axe += ax * 0.05;
    Gyroscope.aye += ay * 0.05;
    Gyroscope.aze += az * 0.05;
    Gyroscope.gxe += gx * 0.05;
    Gyroscope.gye += gy * 0.05;
    Gyroscope.gze += gz * 0.05;

    delay(50);
  }  
  //MPU6050sensor.setThrottle();
}

// Resets the Pitch, Yaw and Roll values to 0
void Gyro_Fast_Cal()
{
  PitchCal = Gyroscope.getPitch();
  YawCal = Gyroscope.getYaw();
  RollCal = Gyroscope.getRoll(); 
}

// Read function to be run as much as possible
void Gyro_Read()
{
  Gyroscope.read(); // initiate sampling
  ax = Gyroscope.getAccelX();
  ay = Gyroscope.getAccelY();
  az = Gyroscope.getAccelZ();
  gx = Gyroscope.getGyroX();
  gy = Gyroscope.getGyroY();
  gz = Gyroscope.getGyroZ();
  temp = Gyroscope.getTemperature();
  Pitch = Gyroscope.getPitch() - PitchCal;
  Yaw = Gyroscope.getYaw() - YawCal;
  Roll = Gyroscope.getRoll() - RollCal;
}
