
#include <Arduino.h>
#include "../../src/Tools/Storage.h"
#include "../../src/Tools/Storage.cpp"
#include "../../src/Tools/TimerManager.h"
#include "../../src/Tools/TimerManager.cpp"
#include "../../src/Services/GpiosManager.h"
#include "../../src/Services/GpiosManager.cpp"

unsigned long last_print = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n\n\n\n\n\n-------------------- TEST GPIO --------------------");

  timer_init();
  GpioInit();
}

void loop()
{
  GpioManager();

  if(millis() - last_print > 2000)
  {
    last_print = millis();

    Serial.println("\n------------ Digitales ------------");
    for (uint8_t i = 0; i < NUMBER_OF_DIGITAL_PIN; i++)
    {
      digital_pin_t* dp = GetDigitalGpio(digital_pin[i].pin);
      if(dp) Serial.printf("PIN %d: Estado=%d, t_ON=%u, t_OFF=%u\n", dp->pin, dp->state, dp->time_state[1], dp->time_state[0]);
    }

    Serial.println("\n------------ Analógicos ------------");
    for (uint8_t i = 0; i < NUMBER_OF_ANALOG_PIN; i++)
    {
      analog_pin_t* ap = GetAnalogGpio(analog_pin[i].pin);
      if(ap) Serial.printf("PIN A%d: Promedio=%u\n", ap->pin, ap->average);
    }
  }
}
