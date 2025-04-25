// INCLUDE ----------------------------------------------------------------------------------------------------

#include "MotionManager.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

Adafruit_MPU6050            MPU;
static MotionInfo_t         Motion;
static uint64_t             DeltaTime;
static CartesianAverage_t   AccelerationAverage;
static bool testflag = false;

static Cartesian        Correction;
static Cartesian        RawAcceleration;

// PRIVATE FUNCTIONS ----------------------------------------------------------------------------------------------------

bool ReadSensorInfo()
{
  sensors_event_t a, g, temp;
  if(!MPU.getEvent(&a, &g, &temp))
  {
    Serial.println("ERROR: MPU6050 sensor reading failed.");
    return false;
  }

  #if defined(ENABLE_VERTICAL_AXIS_X) && !defined(ENABLE_VERTICAL_AXIS_Y) && !defined(ENABLE_VERTICAL_AXIS_Z)
  RawAcceleration[X] = a.acceleration.z;
  RawAcceleration[Y] = a.acceleration.y;
  RawAcceleration[Z] = a.acceleration.x;
  return true;
  #elif !defined(ENABLE_VERTICAL_AXIS_X) && defined(ENABLE_VERTICAL_AXIS_Y) && !defined(ENABLE_VERTICAL_AXIS_Z)
  RawAcceleration[X] = a.acceleration.x;
  RawAcceleration[Y] = a.acceleration.z;
  RawAcceleration[Z] = a.acceleration.y;
  return true;
  #elif !defined(ENABLE_VERTICAL_AXIS_X) && !defined(ENABLE_VERTICAL_AXIS_Y) && defined(ENABLE_VERTICAL_AXIS_Z)
  RawAcceleration[X] = a.acceleration.x;
  RawAcceleration[Y] = a.acceleration.y;
  RawAcceleration[Z] = a.acceleration.z;
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

  Cartesian Sum = {0,0,0};
  Cartesian Sum2 = {0,0,0};
  Cartesian StdDev = {0,0,0};
  Cartesian Last = {0,0,0};

  uint64_t ControlFunTime = get_time_us();

  while (Count < CONFIG_CAL_NUM_SAMPLES)
  {
    AttemptCounter ++;
    if(AttemptCounter > CONFIG_CAL_MAX_ATTEMPTS) return false;

    bool IsMove = MPU.getMotionInterruptStatus();
    #ifdef ENABLE_PRINT_CALIBRATION
    Serial.printf("Movimiento = %s\n",(IsMove ? "YES" : "NO") );
    #endif//ENABLE_PRINT_CALIBRATION
    if(IsMove) continue;

    if(!ReadSensorInfo()) continue;

    if (Count > 0)
    {
      for(int i = 0 ; i<3 ; i++)
      {
        bool Stable = abs(RawAcceleration[i] - Last[i]) <= CONFIG_CAL_TOLERANCE;
        if(!Stable)
        {
          delay(10);
          continue;
        }
      }
    }

    for(int i = 0 ; i<3 ; i++) Last[i] = RawAcceleration[i];

    for(int i = 0 ; i<3 ; i++) Sum[i] += RawAcceleration[i];

    for(int i = 0 ; i<3 ; i++) Sum2[i] += pow(RawAcceleration[i], 2);

    #ifdef ENABLE_PRINT_CALIBRATION
    Serial.printf ("Calibration Aceleration(%d): X=%f - Y=%f - Z=%f\n", Count, RawAcceleration[X], RawAcceleration[Y], RawAcceleration[Z]);
    #endif//ENABLE_PRINT_CALIBRATION

    Count++;
    AttemptCounter = 0;

    delay(10);
  }

  for(int i = 0 ; i<3 ; i++) Correction[i] = Sum[i] / CONFIG_CAL_NUM_SAMPLES;

  // Calcular desviación estándar
  for(int i = 0 ; i<3 ; i++)
  {
    StdDev[i] = sqrt((Sum2[i] / CONFIG_CAL_NUM_SAMPLES) - pow(Correction[i], 2)); //Revisar
    if(StdDev[i] > CONFIG_CAL_TOLERANCE)
    {
      Serial.println("ERROR: High variability in calibration.");
      return false;
    }
  }

  #ifdef ENABLE_PRINT_CALIBRATION
  Serial.printf ("\nCalibration Complete:\n");
  Serial.printf ("     Calibration Time = %.10f ms\n", get_delta_time_us(ControlFunTime)/1000.0 );
  Serial.printf ("     Correction Factor:      X=%f - Y=%f - Z=%f\n", Correction[X], Correction[Y], Correction[Z]);
  Serial.printf ("     Aceleration Raw:        X=%f - Y=%f - Z=%f\n", RawAcceleration[X], RawAcceleration[Y], RawAcceleration[Z]);
  Serial.printf ("     Corrected Acceleration: X=%f - Y=%f - Z=%f\n", RawAcceleration[X]-Correction[X], RawAcceleration[Y]-Correction[Y], RawAcceleration[Z]-Correction[Z]);
  #endif//ENABLE_PRINT_CALIBRATION

  return true;
}

bool UpdateAccelerationAverage()
{
  if(!ReadSensorInfo()) return false;

  for(int i = 0 ; i<3 ; i++)
  {
    AccelerationAverage.Sum[i] -= AccelerationAverage.Buffer[AccelerationAverage.Index][i];
    AccelerationAverage.Buffer[AccelerationAverage.Index][i] = RawAcceleration[i] - Correction[i];
    AccelerationAverage.Sum[i] += RawAcceleration[i] - Correction[i];
  }
  
  AccelerationAverage.Index = (AccelerationAverage.Index + 1) % CONFIG_NUM_SAMPLES;
  AccelerationAverage.Count = min(AccelerationAverage.Count + 1, CONFIG_NUM_SAMPLES);
  
  for(int i = 0 ; i<3 ; i++) Motion.A[i] = AccelerationAverage.Sum[i] / AccelerationAverage.Count;

  return true;
}

void PrintMotionConfig()
{
  #ifdef ENABLE_PRINT_MOTION_CONFIG
  Serial.printf("\nMPU6050 Driver configuration:\n");
  Serial.printf("     Accelerometer Range:  %d\n"   , mpu.getAccelerometerRange() );
  Serial.printf("     Gyroscope Range:      %d\n"   , mpu.getGyroRange() );
  Serial.printf("     Filter BandWidth:     %d\n"   , mpu.getFilterBandwidth() );
  Serial.printf("     High Pass Filter:     %d\n"   , mpu.getHighPassFilter() );
  Serial.printf("     Sample Rate Divisor:  %d\n"   , mpu.getSampleRateDivisor() );
  Serial.printf("     Clock:                %d\n"   , mpu.getClock() );
  #endif//ENABLE_PRINT_MOTION_CONFIG
}

void PrintMotionInfo()
{
  #ifdef ENABLE_PRINT_MOTION_INFO
  Serial.printf("\nMotion Info:\n");
  
  Serial.printf("     Active Time  = %d s\n"    , Motion.ActiveTime);
  Serial.printf("     Movimiento   = %s\n"      , (Motion.IsMove ? "YES" : "NO") );
  
  Serial.printf("     AX = %f\n"                , Motion.A[X]);
  Serial.printf("     AY = %f\n"                , Motion.A[Y]);
  Serial.printf("     AZ = %f\n"                , Motion.A[Z]);
  Serial.printf("     Acceleration = %f\n"      , Motion.Acceleration);

  Serial.printf("     %S\n",testflag?"entre":"no entre");

  #ifdef ENABLE_CALCULATE_AXES
  Serial.printf("     SX = %f\n"                , Motion.S[X]);
  Serial.printf("     SY = %f\n"                , Motion.S[Y]);
  Serial.printf("     SZ = %f\n"                , Motion.S[Z]);
  #endif//ENABLE_CALCULATE_AXES
  Serial.printf("     Speed = %f\n"             , Motion.Speed);

  #ifdef ENABLE_CALCULATE_AXES
  Serial.printf("     DX = %f\n"                , Motion.D[X]);
  Serial.printf("     DY = %f\n"                , Motion.D[Y]);
  Serial.printf("     DZ = %f\n"                , Motion.D[Z]);
  #endif//ENABLE_CALCULATE_AXES
  Serial.printf("     Distance = %f\n"          , Motion.Distance);
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
  if( !MPU.begin() )
  {
    Serial.println("ERROR: Could not find MPU6050 chip");
    return false;
  }
  
  MPU.setAccelerometerRange(CONFIG_ACCELEROMETER_RANGE);
  MPU.setGyroRange(CONFIG_GYROSCOPE_RANGE);
  MPU.setFilterBandwidth(CONFIG_DOWN_PASS_FILTER);
  MPU.setHighPassFilter(CONFIG_HIGH_PASS_FILTER);
  MPU.setSampleRateDivisor(CONFIG_SAMPLE_DIVISOR);

  #if defined(ENABLE_VERTICAL_AXIS_X) && !defined(ENABLE_VERTICAL_AXIS_Y) && !defined(ENABLE_VERTICAL_AXIS_Z)
  MPU.setClock(MPU6050_PLL_GYROX);
  #elif !defined(ENABLE_VERTICAL_AXIS_X) && defined(ENABLE_VERTICAL_AXIS_Y) && !defined(ENABLE_VERTICAL_AXIS_Z)
  MPU.setClock(MPU6050_PLL_GYROY);
  #elif !defined(ENABLE_VERTICAL_AXIS_X) && !defined(ENABLE_VERTICAL_AXIS_Y) && defined(ENABLE_VERTICAL_AXIS_Z)
  MPU.setClock(MPU6050_PLL_GYROZ);
  #else
  Serial.println("ERROR: VERTICAL AXIS NOT CONFIGURED");
  return false;
  #endif

  MPU.setMotionDetectionThreshold(CONFIG_MOTION_THRESHOLD);
  MPU.setMotionDetectionDuration(CONFIG_MOTION_DURATION);
  MPU.setMotionInterrupt(CONFIG_MOTION_INTERRUP);
  delay(100);

  Calibration();

  PrintMotionConfig();

  DeltaTime = get_time_us();

  return true;
}

void MotionControl()
{
  // ACCELERATION --------------------------------------------------
  if(!UpdateAccelerationAverage()) return;
  Motion.Acceleration = sqrt(pow(Motion.A[X], 2) + pow(Motion.A[Y], 2) + pow(Motion.A[Z], 2));

  // DELTA TIME --------------------------------------------------
  float DT = get_delta_time_us(DeltaTime);
  if(DT < CONFIG_MIN_DELTA_TIME) return;
  DT /= 1000000.0;
  DeltaTime = get_time_us();

  // SPEED --------------------------------------------------
  if(abs(Motion.Acceleration) > CONFIG_ACCELERATION_THRESHOLD)
  {
    testflag = true;
    float alpha = 0.7;
    static float lastSpeed = 0.0;

    #ifdef ENABLE_CALCULATE_AXES
    Motion.S[X] = Motion.A[X] * DT;
    Motion.S[Y] = Motion.A[Y] * DT;
    Motion.S[Z] = Motion.A[Z] * DT;
    #endif//ENABLE_CALCULATE_AXES

    Motion.Speed = alpha * (Motion.Speed + Motion.Acceleration * DT) + (1 - alpha) * lastSpeed;
    lastSpeed = Motion.Speed;
  }
  else{
    Motion.Speed *= 0.98;
    testflag = false;
  }

  // DISTANCE --------------------------------------------------
  if(abs(Motion.Speed) > CONFIG_SPEED_THRESHOLD)
  {
    #ifdef ENABLE_CALCULATE_AXES
    Motion.D[X] += Motion.S[X] * DT;
    Motion.D[Y] += Motion.S[Y] * DT;
    Motion.D[Z] += Motion.S[Z] * DT;
    #endif//ENABLE_CALCULATE_AXES

    Motion.Distance += Motion.Speed * DT;
  }

  // MOVE FLAG --------------------------------------------------
  int weight = 0;
  bool MotionInterrupt = MPU.getMotionInterruptStatus();
  static bool LastMotionInterrupt;
  bool SpeedThreshold = ( (abs(Motion.Speed) > CONFIG_SPEED_THRESHOLD) ? true : false);
  static bool LastSpeedThreshold;
  if (MotionInterrupt)      weight += 3;  // Mayor peso a la interrupción actual
  if (LastMotionInterrupt)  weight += 1;
  if (SpeedThreshold)       weight += 3;  // Mayor peso a la velocidad actual
  if (LastSpeedThreshold)   weight += 1;
  Motion.IsMove = (weight >= 4);
  /*static uint16_t counter_to_print = 0;
  if(counter_to_print > 250)
  {
    Serial.printf("\n     Movimiento          = %s\n"      , (motion.IsMove ? "YES" : "NO") );
    Serial.printf("     MotionInterrupt     = %s\n"      , (MotionInterrupt ? "YES" : "NO") );
    //Serial.printf("     SpeedThreshold      = %s\n"      , (SpeedThreshold ? "YES" : "NO") );
    Serial.printf("     LastMotionInterrupt = %s\n"      , (LastMotionInterrupt ? "YES" : "NO") );
    //Serial.printf("     LastSpeedThreshold  = %s\n"      , (LastSpeedThreshold ? "YES" : "NO") );
    counter_to_print = 0;
  }
  else counter_to_print++;*/
  LastMotionInterrupt = MotionInterrupt;
  LastSpeedThreshold = SpeedThreshold;

  // ACTIVE TIME --------------------------------------------------
  static float control_active_time = 0;
  if(Motion.IsMove) control_active_time += DT;
  if(control_active_time >= 1)
  {
    Motion.ActiveTime += (uint64_t)control_active_time;
    control_active_time = 0;
  }

  // --------------------------------------------------

  //if(counter_to_print > 500) Serial.printf("Function time control = %.10f s\n" , get_delta_time_us(control_fun_time)/1000000.0 );

  static uint8_t count = 0;
  if(count++ > 100)
  {
    PrintMotionInfo();
    count = 0;
  }
  
}

MotionInfo_t* GetMotionInfo()
{
  return &Motion;
}

// ----------------------------------------------------------------------------------------------------