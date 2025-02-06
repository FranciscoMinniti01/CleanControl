#ifndef MY_TIMER_H
#define MY_TIMER_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include "Print.h"
#include "HardwareSerial.h"
#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"

#include "time.h"
#include "esp32-hal-timer.h"

// TIMER CONFIG ----------------------------------------------------------------------------------------------------

#define TIME_FREQUENCY        1000000
#define TIME_BASE             10000

// DEFINES ----------------------------------------------------------------------------------------------------

#define TIME_10mS             1
#define TIME_60S              6000
#define TIME_30S              3000
#define TIME_10S              1000
#define TIME_1S               100

typedef void (*cb_timer)();

// VARIABLES ----------------------------------------------------------------------------------------------------

typedef struct
{
    uint16_t      counter;
    uint16_t      comparator;
    bool          flag;
    cb_timer      callback;
}my_timer_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

bool timer_init();
void timer_deinit();

void set_timer(my_timer_t* ptr, uint16_t comparator, cb_timer cb);
void reset_timer(my_timer_t* ptr, uint16_t comparator);
bool get_flag_timer(my_timer_t* ptr);

uint64_t get_time();
uint64_t get_delta_time(uint64_t ot);

// ----------------------------------------------------------------------------------------------------

#endif//MY_TIMER_H