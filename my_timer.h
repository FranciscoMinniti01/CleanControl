#ifndef MY_TIMER_H
#define MY_TIMER_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include "time.h"
#include "esp32-hal-timer.h"
#include "config.h"

// TIMER CONFIG ----------------------------------------------------------------------------------------------------

#define MAX_NUM_TIMERS        5
#define TIME_FREC             1000000
#define BASE_TIME_10mS        10000

#define TIME_10mS             1
#define TIME_60S              6000
#define TIME_30S              3000
#define TIME_10S              1000
#define TIME_1S               100

// VARIABLES ----------------------------------------------------------------------------------------------------

typedef int16_t my_time_t;

typedef struct {
    uint16_t  conter;
    uint16_t  comparator;
    bool      flag;
    int16_t   index = -1;
} my_timer_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

bool timer_init();
bool set_timer(uint16_t comparator);
bool get_flag_timer(time_t index);

// ----------------------------------------------------------------------------------------------------

#endif//MY_TIMER_H