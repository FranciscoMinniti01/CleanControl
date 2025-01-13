// INCLUDE ----------------------------------------------------------------------------------------------------

#include "MPU6050_module.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

Adafruit_MPU6050      mpu;
static motion_info_t  motion_info;

cartesian_t correction_factor;

storage_t Distance_axes_storage;
storage_t Distance_storage;
storage_t activeTime_storage;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void restorar_motion_info()
{
  set_data_storage( &Distance_axes_storage, 
                    (void*)&motion_info.Distance_axes, 
                    sizeof(motion_info.Distance_axes),
                    KEY_DISTAN_A);
  set_data_storage( &Distance_storage, 
                    (void*)&motion_info.Distance, 
                    sizeof(motion_info.Distance),
                    KEY_DISTAN_T);
  set_data_storage( &activeTime_storage, 
                    (void*)&motion_info.activeTime, 
                    sizeof(motion_info.activeTime),
                    KEY_ACTIVE_T);

  if(get_data(&Distance_axes_storage))  Serial.printf("INFO: get distance axes\n");
  if(get_data(&Distance_storage))       Serial.printf("INFO: get distance\n");
  if(get_data(&activeTime_storage))     Serial.printf("INFO: get active time\n");
}

void save_motion_info()
{
  if(!seve_data(&Distance_axes_storage)) Serial.printf("ERROR: can not seve distance axes\n");
  if(!seve_data(&Distance_storage))      Serial.printf("ERROR: can not seve distance\n");
  if(!seve_data(&activeTime_storage))    Serial.printf("ERROR: can not seve active time\n");
}

bool motion_init()
{
  Wire.begin(PIN_SDA,PIN_SCL);
  if( !mpu.begin() )
  {
    Serial.println("ERROR: Failed to find MPU6050 chip");
    return false;
  }
  else
  {
    Serial.println("INFO: MPU6050 motion sensor found");
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    //restorar_motion_info();
    delay(100);
    sensors_event_t a, g, temp;
    if (!mpu.getEvent(&a, &g, &temp)) {
      Serial.println("ERROR: Falló la lectura del sensor MPU6050.");
      return false;
    }
    correction_factor.X = a.acceleration.x;
    correction_factor.Y = a.acceleration.y;
    correction_factor.Z = a.acceleration.z;
    return true;
  }
}

void motion_control()
{
  static uint8_t    counter_time  = 0;
  static uint64_t   delta_time    = 0;

  // Delta tiempo
  float DT    = get_delta_time(delta_time)/1000000.0;
  delta_time  = get_time(); 
  if(DT < 0.00001) return;

  // Obtengo informacion del sensor
  sensors_event_t a, g, temp;
  if (!mpu.getEvent(&a, &g, &temp)) {
    Serial.println("ERROR: Falló la lectura del sensor MPU6050.");
    return;
  }
  float AX = a.acceleration.x - correction_factor.X;// - CORRECCION_AX;
  float AY = a.acceleration.y - correction_factor.Y;// - CORRECCION_AY;
  float AZ = a.acceleration.z - correction_factor.Z;// - CORRECCION_AZ; 

  // Aceleracion total
  motion_info.acceleration = sqrt(pow(AX, 2) + pow(AY, 2) + pow(AZ, 2));
  
  // Detectar movimiento
  if(motion_info.acceleration > 1.1) motion_info.is_move = true;
  else motion_info.is_move = false;
  if(motion_info.is_move) Serial.println("EN MOVIMIENTO");

  /*
  // Tiempo de actividad
  if(motion_info.is_move) motion_info.activeTime += DT;

  // Velocidad total
  motion_info.Speed += motion_info.acceleration * DT; 

  // Distancia total
  motion_info.Distance += motion_info.Speed * DT;

  #ifdef CALCULATE_MOVE_AXES
  // Velocidades de los ejes
  motion_info.Speed_axes.X += a.acceleration.x * DT;
  motion_info.Speed_axes.Y += a.acceleration.y * DT;
  motion_info.Speed_axes.Z += (a.acceleration.z - 9.81) * DT;

  // Distancia de los ejes
  motion_info.Distance_axes.X += motion_info.Speed_axes.X * DT;
  motion_info.Distance_axes.Y += motion_info.Speed_axes.Y * DT;
  motion_info.Distance_axes.Z += motion_info.Speed_axes.Z * DT;
  #endif//CALCULATE_MOVE_AXES

  // Calcular orientación (pitch y roll)
  #ifdef CALCULATE_ROTATION
  motion_info.Inclination_axes_a.X = atan2(a.acceleration.x, sqrt(pow(a.acceleration.y, 2) + pow(a.acceleration.z, 2))) * 180 / PI;
  motion_info.Inclination_axes_a.Y = atan2(a.acceleration.y, sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.z - 9.81, 2))) * 180 / PI;
  motion_info.Inclination_axes_a.Z = 0;

  motion_info.Inclination_axes_g.X += a.gyro.x * DT;
  motion_info.Inclination_axes_g.Y += a.gyro.y * DT;
  motion_info.Inclination_axes_g.Z += a.gyro.z * DT;
  #endif//CALCULATE_ROTATION
  */
  if(counter_time > COUNTER_TO_STORE_INFO)
  {
    //save_motion_info();
    #ifdef PRINT_MOTION_INFO
    Serial.printf("DEBUG: MOTION INFO\n");
    Serial.printf("     Delta time        = %.10f\n" ,DT);
    Serial.printf("     Aceleración cruda - X: %f, Y: %f, Z: %f\n", AX, AY, AZ);
    Serial.printf("     Aceleracion total = %f\n" ,motion_info.acceleration);
    Serial.printf("     En movimiento     = %s\n" ,motion_info.is_move ? "SI":"NO");
    /*Serial.printf("     Velocidad total   = %f\n" ,motion_info.Speed);
    Serial.printf("     Distancia total   = %f\n" ,motion_info.Distance);
    Serial.printf("     Tiempo total      = %d\n" ,motion_info.activeTime);
    Serial.printf("     Velocida: X = %f - Y = %f - Z = %f\n" ,motion_info.Speed_axes.X, motion_info.Speed_axes.Y, motion_info.Speed_axes.Z);
    Serial.printf("     Distancia: X = %f - Y = %f - Z = %f\n" ,motion_info.Distance_axes.X, motion_info.Distance_axes.Y, motion_info.Distance_axes.Z);
    Serial.printf("     Rotacion A: X = %f - Y = %f - Z = %f\n" ,motion_info.Inclination_axes_a.X, motion_info.Inclination_axes_a.Y, motion_info.Inclination_axes_a.Z);
    Serial.printf("     Rotacion G: X = %f - Y = %f - Z = %f\n" ,motion_info.Inclination_axes_g.X, motion_info.Inclination_axes_g.Y, motion_info.Inclination_axes_g.Z);*/
    #endif//PRINT_MOTION_INFO
    counter_time = 0;
  }
  else counter_time++;
}

motion_info_t* get_motion_info()
{
  return &motion_info;
}

// ----------------------------------------------------------------------------------------------------