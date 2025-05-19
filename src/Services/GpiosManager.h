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
uint8_t DigitalPinConfig[NUMBER_OF_DIGITAL_PIN] = {15,16,17};

// ANALOG PIN --------------------------------------------------
#define NUMBER_OF_ANALOG_PIN    1
uint8_t AnalogPinConfig[NUMBER_OF_ANALOG_PIN] = {10};

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
  uint8_t   Pin;
  uint8_t   Counter;
  bool      State;
  uint32_t  TimeState[2];
  uint32_t  TotalTimeState[2];
  storage_t TotalTimeStorage[2];
} DigitalPin_t;

typedef struct
{
  uint8_t   Pin;
  uint16_t  Samples[COUNTER_COMPARATOR];
  uint32_t  Sum;
  uint8_t   Index;
  uint8_t   Count;
  uint16_t  Average;
  storage_t AverageStorage;
} AnalogPin_t;


// FUNCTIONS ------------------------------------------------------------------------------------------

void GpioInit();
void GpioManager();
DigitalPin_t* GetDigitalGpio(uint8_t Pin);
AnalogPin_t* GetAnalogGpio(uint8_t Pin);


// ----------------------------------------------------------------------------------------------------

#endif//GPIO_DATA_H