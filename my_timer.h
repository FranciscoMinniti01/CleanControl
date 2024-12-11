#ifndef MY_TIMER_H
#define MY_TIMER_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include "Print.h"
#include "HardwareSerial.h"

#include "time.h"
#include "esp32-hal-timer.h"
#include <vector>

// TIMER CONFIG ----------------------------------------------------------------------------------------------------

#define TIME_FREC             1000000
#define BASE_TIME             10000

// DEFINES ----------------------------------------------------------------------------------------------------

#define TIME_10mS             1
#define TIME_60S              6000
#define TIME_30S              3000
#define TIME_10S              1000
#define TIME_1S               100

// VARIABLES ----------------------------------------------------------------------------------------------------

typedef struct
{
    uint16_t  conter;
    uint16_t  comparator;
    bool      flag;
} my_timer_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

bool timer_init();
void set_timer(my_timer_t* ptr, uint16_t comparator);
void reset_timer(my_timer_t* ptr, uint16_t comparator);
bool get_flag_timer(my_timer_t* ptr);
void delete_timer(my_timer_t* ptr);
void timer_deinit();

// ----------------------------------------------------------------------------------------------------

#endif//MY_TIMER_H