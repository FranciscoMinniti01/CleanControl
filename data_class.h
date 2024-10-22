#ifndef DATA_CLASS_H
#define DATA_CLASS_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include "esp32-hal-gpio.h"
#include "esp32-hal-adc.h"
#include <Preferences.h>

#include "config.h"

// DEFINES ----------------------------------------------------------------------------------------------------

#define DATA_NAME_SPACE   "data"
#define DATA_KEY_DIGTAL   "td"
#define DATA_KEY_ANALOG   "ta"

typedef struct {
    uint8_t pin;
    bool last;
    uint8_t counter;
    bool state;
    uint16_t time_counter;
} digital_pin_t;

typedef struct {
    uint8_t pin;
    uint16_t Average;
} analog_pin_t;

bool SaveData();
void input_control();
void data_input_init();

#endif//DATA_CLASS_H