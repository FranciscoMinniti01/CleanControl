#ifndef GPIO_DATA_H
#define GPIO_DATA_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include "esp32-hal-gpio.h"
#include "esp32-hal-adc.h"
#include <Preferences.h>

#include "config.h"

// CONFIGURATION ----------------------------------------------------------------------------------------------------

// DIGITAL PIN -----------------------------------
#define NUMBER_OF_DIGITAL_PIN 3
#define DIGITAL_PIN_CERO      15
#define DIGITAL_PIN_UNO       16
#define DIGITAL_PIN_DOS       17
// ANALOG PIN -----------------------------------
#define NUMBER_OF_ANALOG_PIN  1
#define ANALOG_PIN_CERO       10
// -----------------------------------

#define COUNTER_COMPARATOR    10

// DEFINES ----------------------------------------------------------------------------------------------------

#define KEY_DIGTAL_TIME_ON    "DA"
#define KEY_DIGTAL_TIME_OFF   "DD"
#define KEY_ANALOG_AVERAGE    "A"

typedef struct {
  uint8_t pin;
  bool last_state;
  uint8_t counter;
  bool state;
  uint16_t time_state[2];
  uint16_t total_time_state[2];
} digital_pin_t;

typedef struct {
  uint8_t pin;
  uint16_t average;
} analog_pin_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void gpio_data_init();
void gpio_data_control();

// GET FUNCTIONS ----------------------------------------------------------------------------------------------------

digital_pin_t* get_digital_pin(uint8_t pin);
analog_pin_t* get_analog_pin(uint8_t pin);

// ----------------------------------------------------------------------------------------------------

#endif//GPIO_DATA_H