#ifndef MPU6050_MODULE_H
#define MPU6050_MODULE_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

#include "src/Tools/TimerManager.h"

// CONFIG ----------------------------------------------------------------------------------------------------

#define ENABLE_VERTICAL_AXIS_X
//#define ENABLE_VERTICAL_AXIS_Y
//#define ENABLE_VERTICAL_AXIS_Z

//#define ENABLE_PRINT_MOTION_CONFIG
#define ENABLE_PRINT_MOTION_INFO
#define ENABLE_PRINT_CALIBRATION
//#define ENABLE_MOTION_INTERRUPT
//#define ENABLE_CALCULATE_AXES

#define CONFIG_PIN_SDA                        21
#define CONFIG_PIN_SCL                        20
#define CONFIG_ACCELEROMETER_RANGE            MPU6050_RANGE_2_G         // MPU6050_RANGE_2_G o MPU6050_RANGE_4_G 
#define CONFIG_GYROSCOPE_RANGE                MPU6050_RANGE_250_DEG     // MPU6050_RANGE_250_DEG o MPU6050_RANGE_500_DEG 
#define CONFIG_DOWN_PASS_FILTER               MPU6050_BAND_260_HZ       // MPU6050_BAND_184_HZ o MPU6050_BAND_260_HZ
#define CONFIG_HIGH_PASS_FILTER               MPU6050_HIGHPASS_DISABLE  // MPU6050_HIGHPASS_DISABLE o MPU6050_HIGHPASS_5_HZ
#define CONFIG_SAMPLE_DIVISOR                 0
#define CONFIG_MOTION_INTERRUP                true
#define CONFIG_MOTION_THRESHOLD               1
#define CONFIG_MOTION_DURATION                1


#define CONFIG_SPEED_DETEC_MOVE               0.1f
#define CONFIG_MIN_DELTA_TIME                 1000


#define CONFIG_CAL_NUM_SAMPLES                10
#define CONFIG_CAL_MAX_ATTEMPTS               30
#define CONFIG_CAL_TOLERANCE                  0.5f

#define CONFIG_NUM_SAMPLES                    10

// DEFINES ----------------------------------------------------------------------------------------------------

#define KEY_DISTAN_A  "KDA"
#define KEY_DISTAN_T  "KDT"
#define KEY_ACTIVE_T  "KAT"

#define X 0
#define Y 1
#define Z 2

// STRUCTURES - TYPEDEF ----------------------------------------------------------------------------------------------------

typedef float Cartesian[3];

typedef struct {
  Cartesian       Buffer[CONFIG_NUM_SAMPLES] = {0};  
  Cartesian       Sum   = {0,0,0};
  uint8_t         Index = 0;
  uint8_t         Count = 0;
} MovingAverage_t;

typedef struct {
  Cartesian     A;
  Cartesian     S;
  Cartesian     D;

  float         Acceleration;
  float         Speed;
  float         Distance;

  bool          IsMove;
  uint64_t      ActiveTime;
}MotionInfo_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

bool MotionInit();
void MotionControl();
motion_info_t* get_motion_info();

// ----------------------------------------------------------------------------------------------------

#endif//MPU6050_MODULE_H