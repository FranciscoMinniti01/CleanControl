// INCLUDE ----------------------------------------------------------------------------------------------------

#include "gpio_data.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

storage_t digital_storage[NUMBER_OF_DIGITAL_PIN][2];
storage_t analog_storage[NUMBER_OF_ANALOG_PIN];

digital_pin_t digital_pin[NUMBER_OF_DIGITAL_PIN];
analog_pin_t  analog_pin[NUMBER_OF_ANALOG_PIN];

my_time_t timer_input;
my_time_t timer_save;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void gpio_data_init()
{
  timer_input   = set_timer(TIME_10mS);
  timer_save    = set_timer(TIME_60S);

  // DIGITAL PIN -----------------------------------
  digital_pin[0].pin  = DIGITAL_PIN_CERO;
  digital_pin[1].pin  = DIGITAL_PIN_UNO;
  digital_pin[2].pin  = DIGITAL_PIN_DOS;
  // ANALOG PIN -----------------------------------
  analog_pin[0].pin   = ANALOG_PIN_CERO;
  // -----------------------------------

  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    pinMode(digital_pin[i].pin,INPUT);

    digital_storage[i][HIGH].data = (void*)&(digital_pin[i].total_time_state[HIGH]);
    digital_storage[i][HIGH].len  = sizeof(digital_pin[i].total_time_state[HIGH]);
    digital_storage[i][HIGH].key  = KEY_DIGTAL_TIME_ON + String(i);

    digital_storage[i][LOW].data = (void*)&(digital_pin[i].total_time_state[LOW]);
    digital_storage[i][LOW].len  = sizeof(digital_pin[i].total_time_state[LOW]);
    digital_storage[i][LOW].key  = KEY_DIGTAL_TIME_OFF + String(i);

    get_data(&digital_storage[i][HIGH]);
    get_data(&digital_storage[i][LOW]);
  }

  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    analog_storage[i].data = (void*)&(analog_pin[i].average);
    analog_storage[i].len  = sizeof(analog_pin[i].average);
    analog_storage[i].key  = KEY_ANALOG_AVERAGE + String(i);

    get_data(&analog_storage[i]);
  }
}

void gpio_data_control()
{
  static uint8_t auxiliar_digital = 0;

  static uint8_t  analog_read_counter = 0;
  static uint16_t analog_states[NUMBER_OF_ANALOG_PIN][COUNTER_COMPARATOR];
  static uint8_t  analog_states_index = 0;
  static uint16_t auxiliar_Average = 0;
  static bool  auxiliar_analog_flag = false;

  if(get_flag_timer(timer_input))
  {
    for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
    {
      auxiliar_digital = digitalRead(digital_pin[i].pin);
      if(auxiliar_digital == digital_pin[i].last_state)
      {
        digital_pin[i].counter ++;
        if(digital_pin[i].counter >= COUNTER_COMPARATOR)
        {
          digital_pin[i].state    = auxiliar_digital;
          digital_pin[i].time_state[digital_pin[i].state] = 0; // Es importante que este arriba de digital_pin[i].state = auxiliar_digital; ya que pone en cero el estado anterior
          digital_pin[i].counter  = 0;
        }
      }
      else
      {
        digital_pin[i].last_state = auxiliar_digital;
        digital_pin[i].counter = 0;
      }
      digital_pin[i].time_state[digital_pin[i].state] ++;
      digital_pin[i].total_time_state[digital_pin[i].state] ++;
    }

    if(analog_read_counter >= COUNTER_COMPARATOR)
    {
      for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
      {
        analog_states[i][analog_states_index] = analogRead(analog_pin[i].pin);
        for(uint8_t j = 0 ; j<COUNTER_COMPARATOR ; j++)
        {
          auxiliar_Average += analog_states[i][j];
        }
        if(auxiliar_analog_flag) analog_pin[i].average = auxiliar_Average/COUNTER_COMPARATOR;
      }
      analog_states_index++;
      if(analog_states_index >= COUNTER_COMPARATOR)
      {
        auxiliar_analog_flag = true;
        analog_states_index=0;
      }
      analog_read_counter = 0;
    }
    else analog_read_counter++;
  }

  if(get_flag_timer(timer_save))
  {
    for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
    {
      seve_data(&digital_storage[i][HIGH]);
      seve_data(&digital_storage[i][LOW]);
    }
    for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
    {
      seve_data(&analog_storage[i]);
    }
  }
}

// FUNCTIONS GET ----------------------------------------------------------------------------------------------------

digital_pin_t* get_digital_pin(uint8_t pin)
{
  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    if(pin == digital_pin[i].pin) return &digital_pin[i];
  }
  return NULL;
}

analog_pin_t* get_analog_pin(uint8_t pin)
{
    for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    if(pin == analog_pin[i].pin) return &analog_pin[i];
  }
  return NULL;
}

// ----------------------------------------------------------------------------------------------------




