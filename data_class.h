#ifndef DATA_CLASS_H
#define DATA_CLASS_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include "esp32-hal-gpio.h"
#include "esp32-hal-adc.h"
#include <Preferences.h>

#include "config.h"

// DEFINES ----------------------------------------------------------------------------------------------------

#define KEY_DIGTAL_TIME_UP    "DA"
#define KEY_DIGTAL_TIME_DOWN  "DD"
#define KEY_DIGTAL_TIME_AVE   "A"

typedef struct {
  uint8_t pin;
  bool last;
  uint8_t counter;
  bool state;
  uint16_t time_counter[2];
  uint16_t total_time[2];
} digital_pin_t;

typedef struct {
  uint8_t pin;
  uint16_t Average;
} analog_pin_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

bool SaveData();
void input_control();
void data_input_init();

digital_pin_t* get_digital_pin(uint8_t index);
analog_pin_t* get_analog_pin(uint8_t index);

#endif//DATA_CLASS_H