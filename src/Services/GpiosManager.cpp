
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
    DigitalPin[i].Pin = ModuleDigitalPinConfig[i];

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
    AnalogPin[i].Pin = ModuleAnalogPinConfig[i];

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
    uint16_t new_value = analogRead(AnalogPin[i].Pin);
    uint8_t  idx       = AnalogPin[i].Index;

    AnalogPin[i].Sum -= AnalogPin[i].Samples[idx];

    AnalogPin[i].Samples[idx] = new_value;
    AnalogPin[i].Sum += new_value;

    AnalogPin[i].Index = (idx + 1) % COUNTER_COMPARATOR;

    if (AnalogPin[i].Count < COUNTER_COMPARATOR) AnalogPin[i].Count++;

    AnalogPin[i].Average = AnalogPin[i].Sum / AnalogPin[i].Count;
  }
}

bool SaveData()
{
  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    if(!save_storage( &(DigitalPin[i].TotalTimeStorage[0]) )) return false;
    if(!save_storage( &(DigitalPin[i].TotalTimeStorage[1]) )) return false;
  }
  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    if(!save_storage( &(AnalogPin[i].AverageStorage) )) return false;
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

uint32_t GetDigitalValues(uint8_t Pin, uint8_t Value)
{
  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    if(Pin == DigitalPin[i].Pin)
    {
      switch(Value)
      {
        case STATE:
          return (uint32_t)DigitalPin[i].State; 
          break;

        case TIME_ON:
          return DigitalPin[i].TimeState[1]; 
          break;
        
        case TIME_OFF:
          return DigitalPin[i].TimeState[0]; 
          break;

        case TOTAL_TIME_ON:
          return DigitalPin[i].TotalTimeState[1]; 
          break;
        
        case TOTAL_TIME_OFF:
          return DigitalPin[i].TotalTimeState[0];
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
    if(Pin == AnalogPin[i].Pin) return AnalogPin[i].Average;
  }
  return 0;
}


// ----------------------------------------------------------------------------------------------------




