#include <Arduino.h>
#include <string>
#include "Print.h"
#include "HardwareSerial.h"

#include "../../src/Tools/TimerManager.h"
#include "../../src/Tools/TimerManager.cpp"

#include "../../MotionManager.h"
#include "../../MotionManager.cpp"

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n\n\n\n\n\n---------- MPU6050 TEST MAIN ----------");

  timer_init();
  MotionInit();
}

void loop()
{
  while(true)
  {
    MotionControl();
  }
}