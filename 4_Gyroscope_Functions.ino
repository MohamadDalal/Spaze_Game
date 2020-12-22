float ax, ay, az, gx, gy, gz, temp, Pitch, Roll, Yaw, PitchCal, RollCal, YawCal;    // Initialize the reading constants and fast calibrator constants
//float cal = 0.00;
//int calCounter = 0;
//int lightMode = 0;

//char dir[12];

// Adjusted version from Jens' barometer
// Starts the Gyroscope
void Gyro_Init()
{
  delay(100);
  while (Gyroscope.wakeup() == false)                         // Wake up the gyroscope and check that it is connected
  {
    Serial.print(millis());
    Serial.println("\tCould not connect to the gyroscope");   // If not print an error
    //Menu = 2; // Errors screen
    //SubMenu = 1; // Could not connect to Gyroscope
    Gyro_Error_Screen();                                      // Run the Gyro Error screen (Which has a while loop inside. Now that I think about it these are two while loops that do the same thing)
    delay(100);
  }
  Gyroscope.setAccelSensitivity(0);  // 0 = 2g, 1 = 4g, 2 = 8g, 4 = 16g
  Gyroscope.setGyroSensitivity(0);   // 0 = 250 degrees/s, 1 = 500 degrees/s, 2 = 1000 degrees/s, 3 = 2000 degrees/s

  Gyroscope.setThrottle(false);      // Throttling is a delay in the gyroscope reading. This is not enabled as the program itself has enough delay
  Serial.println("start...");

  // Reset the error values. They will be adjusted by the slow calibrator and the fast calibrator
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

// Adjusted version of the calibration example from the GY521 library. Didn't try to fully understand it, but it does some math
// This is the slow calibrator. It calibrates the error values for the acceleration and rotational speed gotten from the gyroscope
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

// This is the fast callibrator, it resets the Pitch, Yaw and Roll values to 0 so that you can reset the initial position when playing
// This reset is done by making a calibration value for them, which is subtracted from the actualy values in the read function
void Gyro_Fast_Cal()
{
  PitchCal = Gyroscope.getPitch();
  YawCal = Gyroscope.getYaw();
  RollCal = Gyroscope.getRoll(); 
}

// Read function to be run as much as possible
void Gyro_Read()
{
  Gyroscope.read();                             // initiate sampling
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
