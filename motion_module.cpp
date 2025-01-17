// INCLUDE ----------------------------------------------------------------------------------------------------

#include "MPU6050_module.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

Adafruit_MPU6050        mpu;
static motion_info_t    motion_info;
static cartesian_t      A_measurements[COUNTER_TO_CALCULATE];

storage_t Distance_axes_storage;
storage_t Distance_storage;
storage_t activeTime_storage;

static my_timer_t timer_to_save;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void restorar_motion_info()
{
  set_data_storage( &Distance_axes_storage, 
                    (void*)&motion_info.D, 
                    sizeof(motion_info.D),
                    KEY_DISTAN_A);
  set_data_storage( &Distance_storage, 
                    (void*)&motion_info.Distance, 
                    sizeof(motion_info.Distance),
                    KEY_DISTAN_T);
  set_data_storage( &activeTime_storage, 
                    (void*)&motion_info.ActiveTime, 
                    sizeof(motion_info.ActiveTime),
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

void Calibration()
{
  const int requiredStableSamples = 10;     // Cantidad de muestras estables requeridas
  const float tolerance           = 0.05;   // Tolerancia porcentual (5%)
  const int delayTimeMs           = 10;     // Retardo entre mediciones (en ms)

  int countMeasurements = 0;
  float sumX = 0, sumY = 0, sumZ = 0;
  float previousX = 0, previousY = 0, previousZ = 0;

  while (countMeasurements < requiredStableSamples)
  {
    // Leer los datos del sensor
    sensors_event_t a, g, temp;
    if (!mpu.getEvent(&a, &g, &temp)) Serial.println("ERROR: Falló la lectura del sensor MPU6050.");

    // Verificar estabilidad en cada eje
    if (countMeasurements > 0)                // Comparar solo a partir del segundo ciclo
    {
      bool xStable = abs(a.acceleration.x - previousX) / abs(previousX + 1e-6) <= tolerance;
      bool yStable = abs(a.acceleration.y - previousY) / abs(previousY + 1e-6) <= tolerance;
      bool zStable = abs(a.acceleration.z - previousZ) / abs(previousZ + 1e-6) <= tolerance;
      if (!(xStable && yStable && zStable))
      {
        delay(delayTimeMs);
        continue;
      }
    }

    // Sumar valores y actualizar el conteo si es estable
    sumX += a.acceleration.x;
    sumY += a.acceleration.y;
    sumZ += a.acceleration.z;
    countMeasurements++;

    // Guardar valores previos
    previousX = a.acceleration.x;
    previousY = a.acceleration.y;
    previousZ = a.acceleration.z;

    Serial.printf("DEBUG: Aceleracion: X=%f - Y=%f - Z=%f\n", previousX, previousY, previousZ);

    // Retardo entre mediciones
    delay(delayTimeMs);
  }

  // Calcular promedio de valores estables
  motion_info.A_corrections.X = sumX / requiredStableSamples;
  motion_info.A_corrections.Y = sumY / requiredStableSamples;
  motion_info.A_corrections.Z = sumZ / requiredStableSamples;

  Serial.println("INFO: Calibración completada: ");
  Serial.printf ("INFO:      Factor de Correccion:  X=%f - Y=%f - Z=%f\n", motion_info.A_corrections.X, motion_info.A_corrections.Y, motion_info.A_corrections.Z);
  Serial.printf ("INFO:      Aceleracion corregida: X=%f - Y=%f - Z=%f\n", previousX-motion_info.A_corrections.X, previousY-motion_info.A_corrections.Y, previousZ-motion_info.A_corrections.Z);
}


/*void calibrateMPU()
{
    const int numSamples  = 100;        // Cantidad de mediciones a realizar
    const float tolerance = 0.05;       // Tolerancia porcentual para validar que está quieto (5%)
    const int delayTimeMs = 10;         // Retardo entre mediciones (en ms)

    float sumX = 0, sumY = 0, sumZ = 0;
    float previousX = 0, previousY = 0, previousZ = 0;
    bool isStable = true;

    for (int i = 0; i < numSamples; ++i) {
        // Leer los datos del sensor
        sensors_event_t a, g, temp;
        if (!mpu.getEvent(&a, &g, &temp)) Serial.println("ERROR: Falló la lectura del sensor MPU6050.");

        // Verificar variación con tolerancia
        if (i > 0) { // Comparar solo a partir del segundo ciclo
            if (abs(a.acceleration.x - previousX) / abs(previousX + 1e-6) > tolerance ||
                abs(a.acceleration.y - previousY) / abs(previousY + 1e-6) > tolerance ||
                abs(a.acceleration.z - previousZ) / abs(previousZ + 1e-6) > tolerance)
            {
                isStable = false;
                //i=0
                break;
            }
        }

        // Acumular valores para el promedio
        sumX += a.acceleration.x;
        sumY += a.acceleration.y;
        sumZ += a.acceleration.z;

        // Guardar valores previos
        previousX = a.acceleration.x;
        previousY = a.acceleration.y;
        previousZ = a.acceleration.z;

        // Retardo antes de la próxima lectura
        delay(delayTimeMs);
    }

    if (isStable) {
        // Calcular promedio y asignar correcciones
        A_corrections.X = sumX / numSamples;
        A_corrections.Y = sumY / numSamples;
        A_corrections.Z = (sumZ / numSamples) - 9.8; // Corregir para aceleración gravitacional
        Serial.println("Calibración completada exitosamente.");
    } else {
        Serial.println("ERROR: El sensor no estaba estable durante la calibración.");
    }
}*/

/*static void Set_Acceleration_Corrections()
{
  uint8_t count = 0;
  bool    flag  = false;
  sensors_event_t a, g, temp;

  while(!flag)
  {
    if(!mpu.getEvent(&a, &g, &temp)) Serial.println("ERROR: Falló la lectura del sensor MPU6050.");
    A_corrections.X += a.acceleration.x;
    A_corrections.Y += a.acceleration.y;
    A_corrections.Z += a.acceleration.z;
    delay(10);
  }

  Serial.printf(" Aceleración cruda - X: %f, Y: %f, Z: %f\n", a.acceleration.x, a.acceleration.y, a.acceleration.z);
  Serial.printf(" Aceleración cruda - X: %f, Y: %f, Z: %f\n", a.acceleration.x-motion_info.A_corrections.X, a.acceleration.y-motion_info.A_corrections.Y, a.acceleration.z-motion_info.A_corrections.Z);
}*/

bool motion_init()
{
  //TwoWire *wire;
  if( Wire.begin(PIN_SDA,PIN_SCL) ) Serial.println("DEBUG: init wire true");
  else Serial.println("DEBUG: init wire false");
  
  if( !mpu.begin() )
  {
    Serial.println("ERROR: Failed to find MPU6050 chip");
    return false;
  }
  Serial.println("INFO: MPU6050 motion sensor found");

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  set_timer(&timer_to_save, TIME_TO_SAVE, NULL);

  //restorar_motion_info();

  Calibration();

  return true;
}

void motion_control()
{
  static uint8_t    counter_time      = 0;
  static uint8_t    counter_to_print  = 0;
  static uint64_t   delta_time        = 0;
  static uint64_t   measurements_time = 0;
  float  DT;

  /*Serial.printf("Tiempo entre mediciones = %.10f\n",get_delta_time(measurements_time));
  measurements_time = get_time();*/

  sensors_event_t a, g, temp;
  if (!mpu.getEvent(&a, &g, &temp)) {
    Serial.println("ERROR: Falló la lectura del sensor MPU6050.");
    return;
  }

  A_measurements[counter_time].X = a.acceleration.z - motion_info.A_corrections.Z;
  A_measurements[counter_time].Y = a.acceleration.y - motion_info.A_corrections.Y;
  A_measurements[counter_time].Z = a.acceleration.x - motion_info.A_corrections.X;

  if(counter_time == COUNTER_TO_CALCULATE)
  {
    for(uint8_t i = 0 ; i < COUNTER_TO_CALCULATE ; i++)
    {
      motion_info.A.X += A_measurements[counter_time].X;
      motion_info.A.Y += A_measurements[counter_time].Y;
      motion_info.A.Z += A_measurements[counter_time].Z;
    }
    motion_info.A.X /= COUNTER_TO_CALCULATE;
    motion_info.A.Y /= COUNTER_TO_CALCULATE;
    motion_info.A.Z /= COUNTER_TO_CALCULATE;
  }

  if(counter_time > COUNTER_TO_CALCULATE)
  {
    // Delta tiempo
    DT = get_delta_time(delta_time)/1000000.0;
    delta_time  = get_time(); 
    if(DT < 0.00001) return;

    // Aceleracion total
    motion_info.Acceleration = sqrt(pow(motion_info.A.X, 2) + pow(motion_info.A.Y, 2) + pow(motion_info.A.Z, 2));
    
    // Detectar movimiento
    if(motion_info.Acceleration > 1.1) motion_info.FlagMove = true;
    else motion_info.FlagMove = false;
    static bool lastflagmove = motion_info.FlagMove ? false : true;
    if(motion_info.FlagMove != lastflagmove) Serial.println("EN MOVIMIENTO");
    lastflagmove = motion_info.FlagMove;

    /*
    // Tiempo de actividad
    if(motion_info.FlagMove) motion_info.activeTime += DT;

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
  }
  else counter_time++;
  
  if(counter_to_print > 1000)
  {
    #ifdef PRINT_MOTION_INFO
    Serial.printf("DEBUG: MOTION INFO\n");
    Serial.printf("     Delta time        = %.10f\n" ,DT);
    Serial.printf("     Aceleración - X: %f, Y: %f, Z: %f\n", motion_info.A.X, motion_info.A.Y, motion_info.A.Z);
    Serial.printf("     Aceleracion total = %f\n" ,motion_info.Acceleration);
    Serial.printf("     En movimiento     = %s\n" ,motion_info.FlagMove ? "SI":"NO");
    /*Serial.printf("     Velocidad total   = %f\n" ,motion_info.Speed);
    Serial.printf("     Distancia total   = %f\n" ,motion_info.Distance);
    Serial.printf("     Tiempo total      = %d\n" ,motion_info.activeTime);
    Serial.printf("     Velocida: X = %f - Y = %f - Z = %f\n" ,motion_info.Speed_axes.X, motion_info.Speed_axes.Y, motion_info.Speed_axes.Z);
    Serial.printf("     Distancia: X = %f - Y = %f - Z = %f\n" ,motion_info.Distance_axes.X, motion_info.Distance_axes.Y, motion_info.Distance_axes.Z);
    Serial.printf("     Rotacion A: X = %f - Y = %f - Z = %f\n" ,motion_info.Inclination_axes_a.X, motion_info.Inclination_axes_a.Y, motion_info.Inclination_axes_a.Z);
    Serial.printf("     Rotacion G: X = %f - Y = %f - Z = %f\n" ,motion_info.Inclination_axes_g.X, motion_info.Inclination_axes_g.Y, motion_info.Inclination_axes_g.Z);*/
    #endif//PRINT_MOTION_INFO
    counter_to_print = 0;
  }
  else counter_to_print++;

  if( get_flag_timer( &timer_to_save ) )
  {
    save_motion_info();
  }
}

motion_info_t* get_motion_info()
{
  return &motion_info;
}

// ----------------------------------------------------------------------------------------------------