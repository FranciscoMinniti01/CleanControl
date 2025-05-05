
// INCLUDE --------------------------------------------------------------------------------------------

#include "../../src/Services/GpiosManager.h"


// VARIABLES ------------------------------------------------------------------------------------------

digital_pin_t digital_pin[NUMBER_OF_DIGITAL_PIN];
analog_pin_t  analog_pin[NUMBER_OF_ANALOG_PIN];

my_timer_t timer_input;
my_timer_t timer_save;


// FUNCTIONS ------------------------------------------------------------------------------------------

void GpioInit()
{
  set_timer(&timer_input, TIME_TO_INPUT, NULL);
  set_timer(&timer_save,  TIME_TO_SAVE, NULL);

  // DIGITAL PIN --------------------------------------------------
  digital_pin[0].pin  = DIGITAL_PIN_CERO;
  digital_pin[1].pin  = DIGITAL_PIN_UNO;
  digital_pin[2].pin  = DIGITAL_PIN_DOS;
  // ANALOG PIN --------------------------------------------------
  analog_pin[0].pin   = ANALOG_PIN_CERO;
  // --------------------------------------------------

  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    pinMode(digital_pin[i].pin,INPUT);

    set_storage( &(digital_pin[i].total_time_storage[0]),
                      (void*)&(digital_pin[i].total_time_state[0]),
                      sizeof(digital_pin[i].total_time_state[0]),
                      KEY_DIGTAL_TIME_ON + String(i)  );

    set_storage( &(digital_pin[i].total_time_storage[1]),
                      (void*)&(digital_pin[i].total_time_state[1]),
                      sizeof(digital_pin[i].total_time_state[1]),
                      KEY_DIGTAL_TIME_OFF + String(i)  );

    get_storage( &(digital_pin[i].total_time_storage[0]) );
    get_storage( &(digital_pin[i].total_time_storage[1]) );
  }

  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    set_storage( &(analog_pin[i].average_storage),
                      (void*)&(analog_pin[i].average),
                      sizeof(analog_pin[i].average),
                      KEY_ANALOG_AVERAGE + String(i)  );

    get_storage( &(analog_pin[i].average_storage) );
  }
}

void digital_data_control()
{
  bool auxiliar_digital_state = 0;

  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    auxiliar_digital_state = digitalRead(digital_pin[i].pin);

    //Serial.println("ACA 2");

    if(auxiliar_digital_state != digital_pin[i].state)
    {
      //Serial.println("ACA 3");
      digital_pin[i].counter ++;
      if(digital_pin[i].counter >= COUNTER_COMPARATOR)
      {
        //Serial.println("ACA 4");
        digital_pin[i].state    = auxiliar_digital_state;
        digital_pin[i].time_state[digital_pin[i].state] = 0;
        digital_pin[i].counter  = 0;
      }
    }
    else digital_pin[i].counter = 0;

    digital_pin[i].time_state[digital_pin[i].state] ++;
    digital_pin[i].total_time_state[digital_pin[i].state] ++;
  }
}

void analog_data_control()
{
  static uint8_t  analog_read_counter = 0;
  static uint8_t  analog_read_index   = 0;
  static uint16_t auxiliar_Average    = 0;
  static bool  auxiliar_analog_flag   = false;

  if(analog_read_counter >= COUNTER_COMPARATOR)
  {
    for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
    {
      analog_pin[i].values[analog_read_index] = analogRead(analog_pin[i].pin);
      for(uint16_t j : analog_pin[i].values) auxiliar_Average += analog_pin[i].values[j];
      if(auxiliar_analog_flag) analog_pin[i].average = auxiliar_Average/COUNTER_COMPARATOR;
    }
    analog_read_index++;

    if(analog_read_index >= COUNTER_COMPARATOR)
    {
      auxiliar_analog_flag = true;
      analog_read_index    = 0;
    }

    analog_read_counter = 0;
  }
  else analog_read_counter++;
}

bool save_data_control()
{
  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    if(!save_storage( &(digital_pin[i].total_time_storage[0]) )) return false;
    if(!save_storage( &(digital_pin[i].total_time_storage[1]) )) return false;
  }
  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    if(!save_storage( &(analog_pin[i].average_storage) )) return false;
  }
  return true;
}

void GpioManager()
{
  if(get_flag_timer(&timer_input))
  {
    //Serial.println("ACA 1");
    digital_data_control();
    analog_data_control();
  }
  if(get_flag_timer(&timer_save))
  {
    if(!save_data_control()) Serial.println("ERROR: save gpio data failed");
  }
}


// GET FUNCTIONS --------------------------------------------------------------------------------------

digital_pin_t* GetDigitalGpio(uint8_t pin)
{
  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    if(pin == digital_pin[i].pin) return &digital_pin[i];
  }
  return NULL;
}

analog_pin_t* GetAnalogGpio(uint8_t pin)
{
  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    if(pin == analog_pin[i].pin) return &analog_pin[i];
  }
  return 0;
}


// ----------------------------------------------------------------------------------------------------




