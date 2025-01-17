#ifndef MPU6050_MODULE_H
#define MPU6050_MODULE_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

#include "my_timer.h"
#include "storage.h"

// CONFIG ----------------------------------------------------------------------------------------------------

#define PRINT_MOTION_INFO
#define CALCULATE_ROTATION
#define CALCULATE_MOVE_AXES
#define COUNTER_TO_CALCULATE          10
#define TIME_TO_SAVE                  TIME_60S

#define PIN_SDA                       21
#define PIN_SCL                       20
#define MPU6050_ACCELEROMETER_RANGE   MPU6050_RANGE_4_G
#define MPU6050_DEGREES_RANGE         MPU6050_RANGE_250_DEG
#define MPU6050_BANDWIDTH_RANGE       MPU6050_BAND_5_HZ

#define CORRECCION_AZ                 12.8f
#define CORRECCION_AY                 -0.7f
#define CORRECCION_AX                 -1.7f

// DEFINES ----------------------------------------------------------------------------------------------------

#define KEY_DISTAN_A  "KDA"
#define KEY_DISTAN_T  "KDT"
#define KEY_ACTIVE_T  "KAT"

// STRUCTURES AND TYPEDEF ----------------------------------------------------------------------------------------------------

typedef struct {
  float   X;
  float   Y;
  float   Z;
}cartesian_t;

typedef struct {
  cartesian_t   A_corrections;
  cartesian_t   AccelerationAxes;
  
  cartesian_t   A;
  cartesian_t   S;
  cartesian_t   D;

  bool          FlagMove;
  uint64_t      ActiveTime;
  float         Acceleration;
  float         Speed;
  float         Distance;
  cartesian_t   Inclination;
  //cartesian_t   Inclination_axes_g;
}motion_info_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

bool motion_init();
void motion_control();
motion_info_t* get_motion_info();

// ----------------------------------------------------------------------------------------------------

#endif//MPU6050_MODULE_H