// INCLUDE ----------------------------------------------------------------------------------------------------

#include "MPU6050_module.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

Adafruit_MPU6050        mpu;
static motion_info_t    motion;
static cartesian_t      samples[CONFIG_NUM_SAMPLES];
static cartesian_t      samples_sum;
static uint64_t         delta_time;
static uint64_t         control_fun_time;

// PRIVATE FUNCTIONS ----------------------------------------------------------------------------------------------------

bool read_sensor_info()
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

bool calibration()
{
  int count = 0;
  cartesian_t sum2 = {0,0,0};
  cartesian_t stdDev = {0,0,0}; 
  bool IsMove;

  control_fun_time = get_time();

  while (count < CONFIG_NUM_SAMPLES)
  {
    IsMove = mpu.getMotionInterruptStatus();
    Serial.printf("Movimiento = %s\n",(IsMove ? "YES" : "NO") );
    if(IsMove) continue;

    if(!read_sensor_info()) continue;

    if (count > 0)
    {
      bool xStable = abs(motion.A_raw.X - samples[count-1].X) <= CONFIG_CALIBRATION_TOLERANCE;
      bool yStable = abs(motion.A_raw.Y - samples[count-1].Y) <= CONFIG_CALIBRATION_TOLERANCE;
      bool zStable = abs(motion.A_raw.Z - samples[count-1].Z) <= CONFIG_CALIBRATION_TOLERANCE;
      if (!(xStable && yStable && zStable))
      {
        delay(10);
        continue;
      }
    }

    samples[count].X = motion.A_raw.X;
    samples[count].Y = motion.A_raw.Y;
    samples[count].Z = motion.A_raw.Z;

    samples_sum.X += motion.A_raw.X;
    samples_sum.Y += motion.A_raw.Y;
    samples_sum.Z += motion.A_raw.Z;

    sum2.X += pow(motion.A_raw.X, 2);
    sum2.Y += pow(motion.A_raw.Y, 2);
    sum2.Z += pow(motion.A_raw.Z, 2);

    Serial.printf ("Calibration Aceleration(%d): X=%f - Y=%f - Z=%f\n", count, samples[count].X, samples[count].Y, samples[count].Z);

    count++;

    delay(10);
  }

  motion.A_corrections.X = samples_sum.X / CONFIG_NUM_SAMPLES;
  motion.A_corrections.Y = samples_sum.Y / CONFIG_NUM_SAMPLES;
  motion.A_corrections.Z = samples_sum.Z / CONFIG_NUM_SAMPLES;

  // Calcular desviación estándar
  stdDev.X = sqrt((samples_sum2.X / (CONFIG_NUM_SAMPLES - 1)) - pow(motion.A_corrections.X, 2) * (CONFIG_NUM_SAMPLES / (CONFIG_NUM_SAMPLES - 1)));
  stdDev.Y = sqrt((samples_sum2.Y / (CONFIG_NUM_SAMPLES - 1)) - pow(motion.A_corrections.Y, 2) * (CONFIG_NUM_SAMPLES / (CONFIG_NUM_SAMPLES - 1)));
  stdDev.Z = sqrt((samples_sum2.Z / (CONFIG_NUM_SAMPLES - 1)) - pow(motion.A_corrections.Z, 2) * (CONFIG_NUM_SAMPLES / (CONFIG_NUM_SAMPLES - 1)));

  // Verificar si la calibración es válida
  if (stdDev.X > CONFIG_CALIBRATION_TOLERANCE || stdDev.Y > CONFIG_CALIBRATION_TOLERANCE || stdDev.Z > CONFIG_CALIBRATION_TOLERANCE)
  {
    Serial.println("ERROR: High variability in calibration. Try again.");
    return false;
  }

  samples_sum.X = 0;
  samples_sum.Y = 0;
  samples_sum.Z = 0;

  for(uint8_t i = 0 ; i<CONFIG_NUM_SAMPLES ; i++)
  {
    samples[i].X -= motion.A_corrections.X;
    samples[i].Y -= motion.A_corrections.Y;
    samples[i].Z -= motion.A_corrections.Z;

    samples_sum.X += samples[i].X;
    samples_sum.Y += samples[i].Y;
    samples_sum.Z += samples[i].Z;
  }

  Serial.println("Calibration Complete:");
  Serial.printf ("     Calibration Time = %.10f ms\n", get_delta_time(control_fun_time)/1000.0 );
  Serial.printf ("     Correction Factor:      X=%f - Y=%f - Z=%f\n", motion.A_corrections.X, motion.A_corrections.Y, motion.A_corrections.Z);
  Serial.printf ("     Aceleration Raw:        X=%f - Y=%f - Z=%f\n", motion.A_raw.X, motion.A_raw.Y, motion.A_raw.Z);
  Serial.printf ("     Corrected Acceleration: X=%f - Y=%f - Z=%f\n", motion.A_raw.X-motion.A_corrections.X, motion.A_raw.Y-motion.A_corrections.Y, motion.A_raw.Z-motion.A_corrections.Z);
  Serial.printf ("     Aceleration Average:    X=%f - Y=%f - Z=%f\n", samples_sum.X/CONFIG_NUM_SAMPLES , samples_sum.Y/CONFIG_NUM_SAMPLES, samples_sum.Z/CONFIG_NUM_SAMPLES);

  return true;
}

void print_motion_config()
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
}

void print_motion_info()
{
  #ifdef ENABLE_PRINT_MOTION_INFO
  static uint16_t counter_to_print = 0;
  if(counter_to_print > 250)
  {
    Serial.printf("Movement information:\n");
    
    Serial.printf("     Control Time = %.10f s\n" , get_delta_time(control_fun_time)/1000000.0 );
    Serial.printf("     Active Time  = %d s\n"    , motion.ActiveTime);
    Serial.printf("     Movimiento   = %s\n"      , (motion.IsMove ? "YES" : "NO") );
    
    Serial.printf("     AX = %f\n"                , motion.A.X);
    Serial.printf("     AY = %f\n"                , motion.A.Y);
    Serial.printf("     AZ = %f\n"                , motion.A.Z);
    Serial.printf("     Acceleration = %f\n"      , motion.Acceleration);

    #ifdef ENABLE_CALCULATE_AXES
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
    Serial.printf("     Distance = %f\n"          , motion.Distance);

    counter_to_print = 0;
  }
  else counter_to_print++;
  #endif//ENABLE_PRINT_MOTION_INFO
}

// PUBLIC FUNCTIONS ---------------------------------------------------------------------------------------------------- 

bool motion_init()
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
  mpu.setFilterBandwidth(CONFIG_BANDWIDTH_RANGE);
  mpu.setHighPassFilter(CONFIG_PASS_FILTER);
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

  mpu.setMotionInterrupt(CONFIG_MOTION_INTERRUP);
  mpu.setMotionDetectionThreshold(CONFIG_MOTION_THRESHOLD);
  mpu.setMotionDetectionDuration(CONFIG_MOTION_DURATION);
  delay(100);

  calibration();

  print_motion_config();

  delta_time = get_time();

  return true;
}

void motion_control()
{
  static uint8_t counter = 0;

  control_fun_time = get_time();

  if(!read_sensor_info()) return;

  // ACCELERATION --------------------------------------------------
  samples_sum.X -= samples[counter].X;
  samples[counter].X = motion.A_raw.X - motion.A_corrections.X;
  samples_sum.X += samples[counter].X;
  motion.A.X = samples_sum.X / CONFIG_NUM_SAMPLES;

  samples_sum.Y -= samples[counter].Y;
  samples[counter].Y = motion.A_raw.Y - motion.A_corrections.Y;
  samples_sum.Y += samples[counter].Y;
  motion.A.Y = samples_sum.Y / CONFIG_NUM_SAMPLES;

  samples_sum.Z -= samples[counter].Z;
  samples[counter].Z = motion.A_raw.Z - motion.A_corrections.Z;
  samples_sum.Z += samples[counter].Z;
  motion.A.Z = samples_sum.Z / CONFIG_NUM_SAMPLES;

  motion.Acceleration = sqrt(pow(motion.A.X, 2) + pow(motion.A.Y, 2) + pow(motion.A.Z, 2));

  // DELTA TIME --------------------------------------------------
  float DT = get_delta_time(delta_time);
  if(DT < CONFIG_MIN_DELTA_TIME_CALCULATE) return;
  DT /= 1000000.0;
  delta_time = get_time();

  // SPEED --------------------------------------------------
  if(abs(motion.Acceleration) > 0.1f)
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
  }

  // MOVE FLAG --------------------------------------------------
  int weight = 0;
  bool MotionInterrupt = mpu.getMotionInterruptStatus();
  bool SpeedThreshold = (motion.Speed > CONFIG_SPEED_DETEC_MOVE ? true : false);
  static bool LastMotionInterrupt;
  static bool LastSpeedThreshold;
  if (MotionInterrupt)      weight += 3;  // Mayor peso a la interrupción actual
  if (SpeedThreshold)       weight += 3;  // Mayor peso a la velocidad actual
  if (LastMotionInterrupt)  weight += 1;
  if (LastSpeedThreshold)   weight += 1;
  motion.IsMove = (weight >= 4);
  static uint16_t counter_to_print = 0;
  if(counter_to_print > 250)
  {
    Serial.printf("     Movimiento          = %s\n"      , (motion.IsMove ? "YES" : "NO") );
    Serial.printf("     MotionInterrupt     = %s\n"      , (motion.IsMove ? "YES" : "NO") );
    Serial.printf("     SpeedThreshold      = %s\n"      , (motion.IsMove ? "YES" : "NO") );
    Serial.printf("     LastMotionInterrupt = %s\n"      , (motion.IsMove ? "YES" : "NO") );
    Serial.printf("     LastSpeedThreshold  = %s\n"      , (motion.IsMove ? "YES" : "NO") );
    counter_to_print = 0;
  }
  else counter_to_print++;
  LastMotionInterrupt = MotionInterrupt;
  LastSpeedThreshold = SpeedThreshold;
 
  // ACTIVE TIME --------------------------------------------------
  static float control_active_time = 0;
  if(motion.IsMove) control_active_time += DT;
  if(control_active_time >= 1)
  {
    motion.ActiveTime += (uint64_t)control_active_time;
    control_active_time = 0;
  }

  // --------------------------------------------------

  if(++counter >= CONFIG_NUM_SAMPLES) counter = 0;

  print_motion_info();
}

motion_info_t* get_motion_info()
{
  return &motion;
}

// ----------------------------------------------------------------------------------------------------