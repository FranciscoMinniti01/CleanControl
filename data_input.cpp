// INCLUDE ----------------------------------------------------------------------------------------------------

#include "data_class.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

storage_t digital_storage[NUMBER_OF_DIGITAL_PIN][2];
storage_t analog_storage[NUMBER_OF_ANALOG_PIN];

digital_pin_t digital_pin[NUMBER_OF_DIGITAL_PIN];
analog_pin_t  analog_pin[NUMBER_OF_ANALOG_PIN];

my_time_t timer_input;
my_time_t timer_save;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void data_input_init()
{
  timer_input   = set_timer(TIME_10mS);
  timer_save   = set_timer(TIME_60S);

  // DIGITAL PIN -----------------------------------
  digital_pin[0].pin = DIGITAL_PIN_CERO;
  digital_pin[1].pin = DIGITAL_PIN_UNO;
  digital_pin[2].pin = DIGITAL_PIN_DOS;
  // ANALOG PIN -----------------------------------
  analog_pin[0].pin = ANALOG_PIN_CERO;
  // -----------------------------------

  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    pinMode(digital_pin[i].pin,INPUT);

    digital_storage[i][0].data = (void*)&(digital_pin[i].total_time[0]);
    digital_storage[i][0].len  = sizeof(digital_pin[i].total_time[0]);
    digital_storage[i][0].key  = KEY_DIGTAL_TIME_DOWN;

    digital_storage[i][1].data = (void*)&(digital_pin[i].total_time[1]);
    digital_storage[i][1].len  = sizeof(digital_pin[i].total_time[1]);
    digital_storage[i][1].key  = KEY_DIGTAL_TIME_UP;

    get_data(&digital_storage[i][0]);
    get_data(&digital_storage[i][1]);

  }

  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    analog_storage[i].data = (void*)&(analog_pin[i].Average);
    analog_storage[i].len  = sizeof(uint16_t);
    analog_storage[i].key  = KEY_DIGTAL_TIME_AVE;

    get_data(&analog_storage[i]);
  }
}

void input_control()
{
  static uint8_t auxiliar_digital = 0;

  static uint8_t  analogRead_counter = 0;
  static uint16_t analog_states[NUMBER_OF_ANALOG_PIN][COUNTER_COMPARATOR];
  static uint8_t  analog_states_index = 0;
  static uint16_t auxiliar_Average = 0;
  static bool  auxiliar_analog_flag = false;

  if(get_flag_timer(timer_input))
  {
    for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
    {
      auxiliar_digital = digitalRead(digital_pin[i].pin);
      if(auxiliar_digital == digital_pin[i].last)
      {
        digital_pin[i].counter ++;
        if(digital_pin[i].counter >= COUNTER_COMPARATOR)
        {
          digital_pin[i].time_counter[digital_pin[i].state] = 0; // Es importante que este arriba de digital_pin[i].state = auxiliar_digital; ya que pone en cero el estado anterior
          digital_pin[i].state = auxiliar_digital;
          digital_pin[i].counter = 0;
        }
      }
      else
      {
        digital_pin[i].last = auxiliar_digital;
        digital_pin[i].counter = 0;
      }
      digital_pin[i].time_counter[digital_pin[i].state] ++;
      digital_pin[i].total_time[digital_pin[i].state] ++;
    }

    if(analogRead_counter >= COUNTER_COMPARATOR)
    {
      for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
      {
        analog_states[i][analog_states_index] = analogRead(analog_pin[i].pin);
        for(uint8_t j = 0 ; j<COUNTER_COMPARATOR ; j++)
        {
          auxiliar_Average += analog_states[i][j];
        }
        if(auxiliar_analog_flag) analog_pin[i].Average = auxiliar_Average/COUNTER_COMPARATOR;
      }
      analog_states_index++;
      if(analog_states_index >= COUNTER_COMPARATOR)
      {
        auxiliar_analog_flag = true;
        analog_states_index=0;
      }
      analogRead_counter = 0;
    }
    else analogRead_counter++;
  }

  if(get_flag_timer(timer_save))
  {
    for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
    {
      seve_data(&digital_storage[i][0]);
      seve_data(&digital_storage[i][1]);
    }
    for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
    {
      seve_data(&analog_storage[i]);
    }
  }
}

// FUNCTIONS GET ----------------------------------------------------------------------------------------------------

digital_pin_t* get_digital_pin(uint8_t index)
{
  return &digital_pin[index];
}

analog_pin_t* get_analog_pin(uint8_t index)
{
  return &analog_pin[index];
}




