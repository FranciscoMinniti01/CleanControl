#ifndef MPU6050_MODULE_H
#define MPU6050_MODULE_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

#include "storage.h"

// CONFIG ----------------------------------------------------------------------------------------------------

#define PRINT_MOTION_INFO
#define CALCULATE_ROTATION
#define CALCULATE_MOVE_AXES
#define COUNTER_TO_STORE_INFO 10

// DEFINES ----------------------------------------------------------------------------------------------------

#define KEY_DISTAN_A  "KDA"
#define KEY_DISTAN_T  "KDT"
#define KEY_ACTIVE_T  "KAT"

// STRUCTURES AND TYPEDEF ----------------------------------------------------------------------------------------------------

storage_t Distance_axes_storage;
storage_t Distance_storage;
storage_t activeTime_storage;

typedef struct {
  float   X;
  float   Y;
  float   Z;
}cartesian_t;

typedef struct {
  bool          is_move;
  cartesian_t   Speed_axes;
  cartesian_t   Distance_axes;
  cartesian_t   Inclination_axes_g;
  cartesian_t   Inclination_axes_a;
  float         acceleration;
  float         Speed;
  float         Distance;
  uint64_t      activeTime;
}motion_info_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

bool motion_init();
void motion_control();
motion_info_t* get_motion_info();

// ----------------------------------------------------------------------------------------------------

#endif//MPU6050_MODULE_H