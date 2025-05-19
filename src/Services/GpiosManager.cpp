
// INCLUDE --------------------------------------------------------------------------------------------

#include "../../src/Services/GpiosManager.h"


// VARIABLES ------------------------------------------------------------------------------------------

DigitalPin_t DigitalPin[NUMBER_OF_DIGITAL_PIN];
AnalogPin_t  AnalogPin[NUMBER_OF_ANALOG_PIN];

my_timer_t TimerInput;
my_timer_t TimerSave;


// FUNCTIONS ------------------------------------------------------------------------------------------

void GpioInit()
{
  set_timer(&TimerInput, TIME_TO_INPUT, NULL);
  set_timer(&TimerSave,  TIME_TO_SAVE,  NULL);

  for(uint8_t i = 0 ; i < NUMBER_OF_DIGITAL_PIN ; i++)
  {
    DigitalPin[i].Pin = DigitalPinConfig[i];

    pinMode(DigitalPin[i].Pin, INPUT);

    SET_STORAGE(  DigitalPin[i].TotalTimeStorage[0],
                  DigitalPin[i].TotalTimeState[0],
                  KEY_DIGTAL_TIME_ON + String(i) )

    get_storage(  &(DigitalPin[i].TotalTimeStorage[0]) );

    SET_STORAGE(  DigitalPin[i].TotalTimeStorage[1],
                  DigitalPin[i].TotalTimeState[1],
                  KEY_DIGTAL_TIME_OFF + String(i) )

    get_storage(  &(DigitalPin[i].TotalTimeStorage[1]) );
  }

  for(uint8_t i = 0 ; i < NUMBER_OF_ANALOG_PIN ; i++)
  {
    AnalogPin[i].Pin = AnalogPinConfig[i];

    SET_STORAGE(  AnalogPin[i].AverageStorage,
                  AnalogPin[i].Average,
                  KEY_ANALOG_AVERAGE + String(i) )

    get_storage(  &(AnalogPin[i].AverageStorage) );
  }
}

void DigitalDataControl()
{
  bool AuxiliarState = 0;

  for(uint8_t i = 0 ; i < NUMBER_OF_DIGITAL_PIN ; i++)
  {
    AuxiliarState = digitalRead(DigitalPin[i].Pin);

    if(AuxiliarState != DigitalPin[i].State)
    {
      DigitalPin[i].Counter ++;
      if(DigitalPin[i].Counter >= COUNTER_COMPARATOR)
      {
        DigitalPin[i].State    = AuxiliarState;
        DigitalPin[i].TimeState[DigitalPin[i].State] = 0;
        DigitalPin[i].Counter  = 0;
      }
    }
    else DigitalPin[i].Counter = 0;

    DigitalPin[i].TimeState[DigitalPin[i].State] += 1;
    DigitalPin[i].TotalTimeState[DigitalPin[i].State] += 1;
  }
}

void AnalogDataControl()
{
  for (uint8_t i = 0; i < NUMBER_OF_ANALOG_PIN; i++)
  {
    uint16_t new_value = analogRead(AnalogPin[i].pin);
    uint8_t  idx       = AnalogPin[i].index;

    AnalogPin[i].sum -= AnalogPin[i].values[idx];

    AnalogPin[i].values[idx] = new_value;
    AnalogPin[i].sum += new_value;

    AnalogPin[i].index = (idx + 1) % COUNTER_COMPARATOR;

    if (AnalogPin[i].count < COUNTER_COMPARATOR) AnalogPin[i].count++;

    AnalogPin[i].average = AnalogPin[i].sum / AnalogPin[i].count;
  }
}

bool SaveData()
{
  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    if(!save_storage( &(DigitalPin[i].total_time_storage[0]) )) return false;
    if(!save_storage( &(DigitalPin[i].total_time_storage[1]) )) return false;
  }
  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    if(!save_storage( &(AnalogPin[i].average_storage) )) return false;
  }
  return true;
}

void GpioManager()
{
  if(get_flag_timer(&TimerInput))
  {
    DigitalDataControl();
    AnalogDataControl();
  }
  if(get_flag_timer(&TimerSave))
  {
    if(!SaveData()) Serial.println("ERROR: save gpio data failed");
  }
}


// GET FUNCTIONS --------------------------------------------------------------------------------------

uint32_t GetDigitalValues(uint8_t Pin, uint8_t Value);
{
  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    if(Pin == DigitalPin[i].Pin)
    {
      switch(Value)
      {
        case STATE:
          return (uint32_t)DigitalPin[i].state; 
          break;

        case TIME_ON:
          return DigitalPin[i].time_state[1]; 
          break;
        
        case TIME_OFF:
          return DigitalPin[i].time_state[0]; 
          break;

        case TOTAL_TIME_ON:
          return DigitalPin[i].total_time_state[1]; 
          break;
        
        case TOTAL_TIME_OFF:
          return DigitalPin[i].total_time_state[0];
          break;
      }
    }
  }
  return NULL;
}

uint16_t GetAnalogAverage(uint8_t Pin)
{
  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    if(Pin == AnalogPin[i].pin) return AnalogPin[i].average;
  }
  return 0;
}


// ----------------------------------------------------------------------------------------------------




