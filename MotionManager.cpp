// INCLUDE ----------------------------------------------------------------------------------------------------

#include "MotionManager.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

Adafruit_MPU6050        mpu;
static motion_info_t    motion;
static uint64_t         delta_time;
static MovingAverage_t  MovingAverage; 

// PRIVATE FUNCTIONS ----------------------------------------------------------------------------------------------------

bool ReadSensorInfo()
{
  sensors_event_t a, g, temp;
  if(!mpu.getEvent(&a, &g, &temp))
  {
    Serial.println("ERROR: MPU6050 sensor reading failed.");
    return false;
  }
  
  #if defined(ENABLE_VERTICAL_AXIS_X) && !defined(ENABLE_VERTICAL_AXIS_Y) && !defined(ENABLE_VERTICAL_AXIS_Z)
  motion.A_raw.X = a.acceleration.z;
  motion.A_raw.Y = a.acceleration.y;
  motion.A_raw.Z = a.acceleration.x;
  return true;
  #elif !defined(ENABLE_VERTICAL_AXIS_X) && defined(ENABLE_VERTICAL_AXIS_Y) && !defined(ENABLE_VERTICAL_AXIS_Z)
  motion.A_raw.X = a.acceleration.x;
  motion.A_raw.Y = a.acceleration.z;
  motion.A_raw.Z = a.acceleration.y;
  return true;
  #elif !defined(ENABLE_VERTICAL_AXIS_X) && !defined(ENABLE_VERTICAL_AXIS_Y) && defined(ENABLE_VERTICAL_AXIS_Z)
  motion.A_raw.X = a.acceleration.x;
  motion.A_raw.Y = a.acceleration.y;
  motion.A_raw.Z = a.acceleration.z;
  return true;
  #else
  Serial.println("ERROR: VERTICAL AXIS NOT CONFIGURED");
  return false;
  #endif
}

bool Calibration()
{
  int Count = 0;
  int AttemptCounter = 0;

  cartesian_t Sum = {0,0,0};
  cartesian_t Sum2 = {0,0,0};
  cartesian_t StdDev = {0,0,0};

  cartesian_t Last = {0,0,0};

  bool IsMove; 

  uint64_t ControlFunTime = get_time_us();

  while (Count < CONFIG_CAL_NUM_SAMPLES)
  {
    AttemptCounter ++;
    if(AttemptCounter > CONFIG_CAL_MAX_ATTEMPTS) return false;

    IsMove = mpu.getMotionInterruptStatus();
    Serial.printf("Movimiento = %s\n",(IsMove ? "YES" : "NO") );
    if(IsMove) continue;

    if(!ReadSensorInfo()) continue;

    if (Count > 0)
    {
      bool XStable = abs(motion.A_raw.X - Last.X) <= CONFIG_CALIBRATION_TOLERANCE;
      bool YStable = abs(motion.A_raw.Y - Last.Y) <= CONFIG_CALIBRATION_TOLERANCE;
      bool ZStable = abs(motion.A_raw.Z - Last.Z) <= CONFIG_CALIBRATION_TOLERANCE;
      if (!(XStable && YStable && ZStable))
      {
        delay(10);
        continue;
      }
    }

    Last.X = motion.A_raw.X;
    Last.Y = motion.A_raw.Y;
    Last.Z = motion.A_raw.Z;

    Sum.X += motion.A_raw.X;
    Sum.Y += motion.A_raw.Y;
    Sum.Z += motion.A_raw.Z;

    Sum2.X += pow(motion.A_raw.X, 2);
    Sum2.Y += pow(motion.A_raw.Y, 2);
    Sum2.Z += pow(motion.A_raw.Z, 2);

    #ifdef ENABLE_CALIBRATION_PRINT
    Serial.printf ("Calibration Aceleration(%d): X=%f - Y=%f - Z=%f\n", Count, motion.A_raw.X, motion.A_raw.Y, motion.A_raw.Z);
    #endif//ENABLE_CALIBRATION_PRINT

    Count++;
    AttemptCounter = 0;

    delay(10);
  }

  motion.A_Correction.X = Sum.X / CONFIG_CAL_NUM_SAMPLES;
  motion.A_Correction.Y = Sum.Y / CONFIG_CAL_NUM_SAMPLES;
  motion.A_Correction.Z = Sum.Z / CONFIG_CAL_NUM_SAMPLES;

  // Calcular desviación estándar
  StdDev.X = sqrt((Sum2.X / CONFIG_CAL_NUM_SAMPLES) - pow(motion.A_Correction.X, 2));
  StdDev.Y = sqrt((Sum2.Y / CONFIG_CAL_NUM_SAMPLES) - pow(motion.A_Correction.Y, 2));
  StdDev.Z = sqrt((Sum2.Z / CONFIG_CAL_NUM_SAMPLES) - pow(motion.A_Correction.Z, 2));

  // Verificar si la calibración es válida
  if (StdDev.X > CONFIG_CALIBRATION_TOLERANCE || StdDev.Y > CONFIG_CALIBRATION_TOLERANCE || StdDev.Z > CONFIG_CALIBRATION_TOLERANCE)
  {
    Serial.println("ERROR: High variability in calibration.");
    return false;
  }

  #ifdef ENABLE_CALIBRATION_PRINT
  Serial.println("Calibration Complete:");
  Serial.printf ("     Calibration Time = %.10f ms\n", get_delta_time_us(ControlFunTime)/1000.0 );
  Serial.printf ("     Correction Factor:      X=%f - Y=%f - Z=%f\n", motion.A_Correction.X, motion.A_Correction.Y, motion.A_Correction.Z);
  Serial.printf ("     Aceleration Raw:        X=%f - Y=%f - Z=%f\n", motion.A_raw.X, motion.A_raw.Y, motion.A_raw.Z);
  Serial.printf ("     Corrected Acceleration: X=%f - Y=%f - Z=%f\n", motion.A_raw.X-motion.A_Correction.X, motion.A_raw.Y-motion.A_Correction.Y, motion.A_raw.Z-motion.A_Correction.Z);
  #endif//ENABLE_CALIBRATION_PRINT

  return true;
}

/*bool UpdateMovingAverage()
{
  if(!ReadSensorInfo()) return false;

  MovingAverage.Sum.X -= MovingAverage.Buffer[MovingAverage.Index].X;
  MovingAverage.Buffer[MovingAverage.Index].X = motion.A_raw.X - motion.A_Correction.X;
  MovingAverage.Sum.X += motion.A_raw.X - motion.A_Correction.X;

  MovingAverage.Sum.Y -= MovingAverage.Buffer[MovingAverage.Index].Y;
  MovingAverage.Buffer[MovingAverage.Index].Y = motion.A_raw.Y - motion.A_Correction.Y;
  MovingAverage.Sum.Y += motion.A_raw.Y - motion.A_Correction.Y;

  MovingAverage.Sum.Z -= MovingAverage.Buffer[MovingAverage.Index].Z;
  MovingAverage.Buffer[MovingAverage.Index].Z = motion.A_raw.Z - motion.A_Correction.Z;
  MovingAverage.Sum.Z += motion.A_raw.Z - motion.A_Correction.Z;
  
  MovingAverage.Index = (MovingAverage.Index + 1) % CONFIG_NUM_SAMPLES;
  MovingAverage.Count = min(MovingAverage.Count + 1, CONFIG_NUM_SAMPLES);
  
  motion.A.X = MovingAverage.Sum.X / MovingAverage.Count;
  motion.A.Y = MovingAverage.Sum.Y / MovingAverage.Count;
  motion.A.Z = MovingAverage.Sum.Z / MovingAverage.Count;

  return true;
}*/

/*void PrintMotionConfig()
{
  #ifdef ENABLE_PRINT_MOTION_CONFIG
  Serial.printf("MPU6050 Driver configuration:\n");
  Serial.printf("     Accelerometer Range:  %d\n"   , mpu.getAccelerometerRange() );
  Serial.printf("     Gyroscope Range:      %d\n"   , mpu.getGyroRange() );
  Serial.printf("     Filter BandWidth:     %d\n"   , mpu.getFilterBandwidth() );
  Serial.printf("     High Pass Filter:     %d\n"   , mpu.getHighPassFilter() );
  Serial.printf("     Sample Rate Divisor:  %d\n"   , mpu.getSampleRateDivisor() );
  Serial.printf("     Clock:                %d\n"   , mpu.getClock() );
  #endif//ENABLE_PRINT_MOTION_CONFIG
}*/

void PrintMotionInfo()
{
  #ifdef ENABLE_PRINT_MOTION_INFO
  static uint16_t counter_to_print = 0;
  if(counter_to_print > 250)
  {
    Serial.printf("\nMove info:\n");
    
    //Serial.printf("     Active Time  = %d s\n"    , motion.ActiveTime);
    //Serial.printf("     Movimiento   = %s\n"      , (motion.IsMove ? "YES" : "NO") );
    
    Serial.printf("     AX = %f\n"                , motion.A.X);
    Serial.printf("     AY = %f\n"                , motion.A.Y);
    Serial.printf("     AZ = %f\n"                , motion.A.Z);
    Serial.printf("     Acceleration = %f\n"      , motion.Acceleration);

    /*#ifdef ENABLE_CALCULATE_AXES
    Serial.printf("     SX = %f\n"                , motion.S.X);
    Serial.printf("     SY = %f\n"                , motion.S.Y);
    Serial.printf("     SZ = %f\n"                , motion.S.Z);
    #endif//ENABLE_CALCULATE_AXES
    Serial.printf("     Speed = %f\n"             , motion.Speed);

    #ifdef ENABLE_CALCULATE_AXES
    Serial.printf("     DX = %f\n"                , motion.D.X);
    Serial.printf("     DY = %f\n"                , motion.D.Y);
    Serial.printf("     DZ = %f\n"                , motion.D.Z);
    #endif//ENABLE_CALCULATE_AXES
    Serial.printf("     Distance = %f\n"          , motion.Distance);*/

    counter_to_print = 0;
  }
  else counter_to_print++;
  #endif//ENABLE_PRINT_MOTION_INFO
}

// PUBLIC FUNCTIONS ---------------------------------------------------------------------------------------------------- 

bool MotionInit()
{
  if( !Wire.begin(CONFIG_PIN_SDA,CONFIG_PIN_SCL) )
  {
    Serial.println("ERROR: Init wire Failed");
    return false;
  }
  if( !mpu.begin() )
  {
    Serial.println("ERROR: Could not find MPU6050 chip");
    return false;
  }
  
  mpu.setAccelerometerRange(CONFIG_ACCELEROMETER_RANGE);
  mpu.setGyroRange(CONFIG_GYROSCOPE_RANGE);
  mpu.setFilterBandwidth(CONFIG_DOWN_PASS_FILTER);
  mpu.setHighPassFilter(CONFIG_HIGH_PASS_FILTER);
  mpu.setSampleRateDivisor(CONFIG_SAMPLE_DIVISOR);

  #if defined(ENABLE_VERTICAL_AXIS_X) && !defined(ENABLE_VERTICAL_AXIS_Y) && !defined(ENABLE_VERTICAL_AXIS_Z)
  mpu.setClock(MPU6050_PLL_GYROX);
  #elif !defined(ENABLE_VERTICAL_AXIS_X) && defined(ENABLE_VERTICAL_AXIS_Y) && !defined(ENABLE_VERTICAL_AXIS_Z)
  mpu.setClock(MPU6050_PLL_GYROY);
  #elif !defined(ENABLE_VERTICAL_AXIS_X) && !defined(ENABLE_VERTICAL_AXIS_Y) && defined(ENABLE_VERTICAL_AXIS_Z)
  mpu.setClock(MPU6050_PLL_GYROZ);
  #else
  Serial.println("ERROR: VERTICAL AXIS NOT CONFIGURED");
  return false;
  #endif

  mpu.setMotionDetectionThreshold(CONFIG_MOTION_THRESHOLD);
  mpu.setMotionDetectionDuration(CONFIG_MOTION_DURATION);
  mpu.setMotionInterrupt(CONFIG_MOTION_INTERRUP);
  delay(100);

  Calibration();

  //PrintMotionConfig();

  delta_time = get_time_us();

  return true;
}

void MotionControl()
{
  // ACCELERATION --------------------------------------------------

  /*if(!UpdateMovingAverage()) return;
  motion.Acceleration = sqrt(pow(motion.A.X, 2) + pow(motion.A.Y, 2) + pow(motion.A.Z, 2));*/

  // DELTA TIME --------------------------------------------------

  /*float DT = get_delta_time_us(delta_time);
  if(DT < CONFIG_MIN_DELTA_TIME) return;
  DT /= 1000000.0;
  delta_time = get_time_us();*/

  // SPEED --------------------------------------------------
  /*if(abs(motion.Acceleration) > 0.1f)
  {
    float alpha = 0.7;
    static float lastSpeed = 0.0;
    #ifdef ENABLE_CALCULATE_AXES
    motion.S.X = motion.A.X * DT;
    motion.S.Y = motion.A.Y * DT;
    motion.S.Z = motion.A.Z * DT;
    #endif//ENABLE_CALCULATE_AXES

    motion.Speed = alpha * (motion.Speed + motion.Acceleration * DT) + (1 - alpha) * lastSpeed;
    lastSpeed = motion.Speed;
  }
  else motion.Speed *= 0.98;

  // DISTANCE --------------------------------------------------
  if(abs(motion.Speed) > 0.05f)
  {
    #ifdef ENABLE_CALCULATE_AXES
    motion.D.X = motion.S.X * DT;
    motion.D.Y = motion.S.Y * DT;
    motion.D.Z = motion.S.Z * DT;
    #endif//ENABLE_CALCULATE_AXES

    motion.Distance += motion.Speed * DT;
  }*/

  // MOVE FLAG --------------------------------------------------
  
  int weight = 0;
  bool MotionInterrupt = mpu.getMotionInterruptStatus();
  static bool LastMotionInterrupt;
  //bool SpeedThreshold = (motion.Speed > CONFIG_SPEED_DETEC_MOVE ? true : false);
  //static bool LastSpeedThreshold;
  if (MotionInterrupt)      weight += 3;  // Mayor peso a la interrupción actual
  if (LastMotionInterrupt)  weight += 1;
  //if (SpeedThreshold)       weight += 3;  // Mayor peso a la velocidad actual
  //if (LastSpeedThreshold)   weight += 1;
  motion.IsMove = (weight >= 4);
  static uint16_t counter_to_print = 0;
  if(counter_to_print > 250)
  {
    Serial.printf("\n     Movimiento          = %s\n"      , (motion.IsMove ? "YES" : "NO") );
    Serial.printf("     MotionInterrupt     = %s\n"      , (MotionInterrupt ? "YES" : "NO") );
    //Serial.printf("     SpeedThreshold      = %s\n"      , (SpeedThreshold ? "YES" : "NO") );
    Serial.printf("     LastMotionInterrupt = %s\n"      , (LastMotionInterrupt ? "YES" : "NO") );
    //Serial.printf("     LastSpeedThreshold  = %s\n"      , (LastSpeedThreshold ? "YES" : "NO") );
    counter_to_print = 0;
  }
  else counter_to_print++;
  LastMotionInterrupt = MotionInterrupt;
  //LastSpeedThreshold = SpeedThreshold;
 
  // ACTIVE TIME --------------------------------------------------
  /*
  static float control_active_time = 0;
  if(motion.IsMove) control_active_time += DT;
  if(control_active_time >= 1)
  {
    motion.ActiveTime += (uint64_t)control_active_time;
    control_active_time = 0;
  }
  */
  // --------------------------------------------------

  //if(counter_to_print > 500) Serial.printf("Function time control = %.10f s\n" , get_delta_time_us(control_fun_time)/1000000.0 );

  //PrintMotionInfo();
}

/*motion_info_t* get_motion_info()
{
  return &motion;
}*/

// ----------------------------------------------------------------------------------------------------