#ifndef GPIO_DATA_H
#define GPIO_DATA_H

// INCLUDE --------------------------------------------------------------------------------------------

#include "Print.h"
#include "HardwareSerial.h"

#include "esp32-hal-gpio.h"
#include "esp32-hal-adc.h"

#include "../../src/Tools/TimerManager.h"
#include "../../src/Tools/Storage.h"


// GPIO DATA CONFIG -----------------------------------------------------------------------------------

// DIGITAL PIN --------------------------------------------------
#define NUMBER_OF_DIGITAL_PIN   3
#define DIGITAL_PIN_CERO        15
#define DIGITAL_PIN_UNO         16
#define DIGITAL_PIN_DOS         17
// ANALOG PIN --------------------------------------------------
#define NUMBER_OF_ANALOG_PIN    1
#define ANALOG_PIN_CERO         10
// --------------------------------------------------

#define TIME_TO_INPUT           TIME_10mS
#define TIME_TO_SAVE            TIME_60S
#define COUNTER_COMPARATOR      10


// DEFINES --------------------------------------------------------------------------------------------

#define KEY_DIGTAL_TIME_ON      "DA"
#define KEY_DIGTAL_TIME_OFF     "DD"
#define KEY_ANALOG_AVERAGE      "A"


// VARIABLES ------------------------------------------------------------------------------------------

typedef struct 
{
  uint8_t   pin;
  uint8_t   counter;
  bool      state;
  uint16_t  time_state[2];
  uint16_t  total_time_state[2];
  storage_t total_time_storage[2];
} digital_pin_t;

typedef struct
{
  uint8_t   pin;
  uint16_t  values[COUNTER_COMPARATOR];
  uint16_t  average;
  storage_t average_storage;
} analog_pin_t;


// FUNCTIONS ------------------------------------------------------------------------------------------

void GpioInit();
void GpioManager();
digital_pin_t* GetDigitalGpio(uint8_t pin);
analog_pin_t* GetAnalogGpio(uint8_t pin);


// ----------------------------------------------------------------------------------------------------

#endif//GPIO_DATA_H