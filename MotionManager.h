#ifndef MPU6050_MODULE_H
#define MPU6050_MODULE_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

#include "src/Tools/Storage.h"
#include "src/Tools/TimerManager.h"

// CONFIG ----------------------------------------------------------------------------------------------------

#define ENABLE_VERTICAL_AXIS_X
//#define ENABLE_VERTICAL_AXIS_Y
//#define ENABLE_VERTICAL_AXIS_Z

#define ENABLE_PRINT_MOTION_CONFIG
#define ENABLE_PRINT_MOTION_INFO
//#define ENABLE_CALCULATE_AXES

#define CONFIG_PIN_SDA                        21
#define CONFIG_PIN_SCL                        20
#define CONFIG_ACCELEROMETER_RANGE            MPU6050_RANGE_2_G 
#define CONFIG_GYROSCOPE_RANGE                MPU6050_RANGE_250_DEG
#define CONFIG_BANDWIDTH_RANGE                MPU6050_BAND_260_HZ
#define CONFIG_PASS_FILTER                    MPU6050_HIGHPASS_DISABLE
#define CONFIG_SAMPLE_DIVISOR                 0
#define CONFIG_MOTION_INTERRUP                true
#define CONFIG_MOTION_THRESHOLD               1
#define CONFIG_MOTION_DURATION                1

#define CONFIG_SPEED_DETEC_MOVE               0.1f
#define CONFIG_MIN_DELTA_TIME_CALCULATE       1000
#define CONFIG_NUM_SAMPLES                    5
#define CONFIG_CALIBRATION_TOLERANCE          0.5f

// DEFINES ----------------------------------------------------------------------------------------------------

#define KEY_DISTAN_A  "KDA"
#define KEY_DISTAN_T  "KDT"
#define KEY_ACTIVE_T  "KAT"

// STRUCTURES - TYPEDEF ----------------------------------------------------------------------------------------------------

typedef struct {
  float   X;
  float   Y;
  float   Z;
}cartesian_t;

typedef struct {
  cartesian_t   A_raw;
  cartesian_t   A_corrections;
  
  cartesian_t   A;
  cartesian_t   S;
  cartesian_t   D;

  float         Acceleration;
  float         Speed;
  float         Distance;

  bool          IsMove;
  uint64_t      ActiveTime;
}motion_info_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

bool motion_init();
void motion_control();
motion_info_t* get_motion_info();

// ----------------------------------------------------------------------------------------------------

#endif//MPU6050_MODULE_H