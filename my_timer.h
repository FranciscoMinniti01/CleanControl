#ifndef MY_TIMER_H
#define MY_TIMER_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include "time.h"
#include "esp32-hal-timer.h"

#include "config.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

typedef struct {
    uint16_t conter;
    uint16_t comparator;
    bool flag;
    int16_t index = -1;
} my_timer_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

bool timer_init();
int16_t set_timer(uint16_t comparator);
bool get_flag_timer(uint16_t index);
bool timer_end();

#endif//MY_TIMER_H