// INCLUDE ----------------------------------------------------------------------------------------------------

#include "gpio_data.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

digital_pin_t digital_pin[NUMBER_OF_DIGITAL_PIN];
analog_pin_t  analog_pin[NUMBER_OF_ANALOG_PIN];

my_timer_t timer_input;
my_timer_t timer_save;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void gpio_data_init()
{
  set_timer(&timer_input, TIME_TO_INPUT);
  set_timer(&timer_save,  TIME_TO_SAVE);

  // DIGITAL PIN -----------------------
  digital_pin[0].pin  = DIGITAL_PIN_CERO;
  digital_pin[1].pin  = DIGITAL_PIN_UNO;
  digital_pin[2].pin  = DIGITAL_PIN_DOS;
  // ANALOG PIN ------------------------
  analog_pin[0].pin   = ANALOG_PIN_CERO;
  // -----------------------------------

  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    pinMode(digital_pin[i].pin,INPUT);

    set_data_storage( &(digital_pin[i].total_time_storage[0]),
                      (void*)&(digital_pin[i].total_time_state[0]),
                      sizeof(digital_pin[i].total_time_state[0]),
                      KEY_DIGTAL_TIME_ON + String(i)  );

    set_data_storage( &(digital_pin[i].total_time_storage[1]),
                      (void*)&(digital_pin[i].total_time_state[1]),
                      sizeof(digital_pin[i].total_time_state[1]),
                      KEY_DIGTAL_TIME_OFF + String(i)  );

    get_data( &(digital_pin[i].total_time_storage[0]) );
    get_data( &(digital_pin[i].total_time_storage[1]) );
  }

  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    set_data_storage( &(analog_pin[i].average_storage),
                      (void*)&(analog_pin[i].average),
                      sizeof(analog_pin[i].average),
                      KEY_ANALOG_AVERAGE + String(i)  );

    get_data( &(analog_pin[i].average_storage) );
  }
}

void digital_data_control()
{
  bool auxiliar_digital_state = 0;

  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    auxiliar_digital_state = digitalRead(digital_pin[i].pin);
    if(auxiliar_digital_state == digital_pin[i].last_state)
    {
      digital_pin[i].counter ++;
      if(digital_pin[i].counter >= COUNTER_COMPARATOR)
      {
        digital_pin[i].state    = auxiliar_digital_state;
        digital_pin[i].time_state[digital_pin[i].state] = 0; // Es importante que este arriba de digital_pin[i].state = auxiliar_digital_state; ya que pone en cero el estado anterior
        digital_pin[i].counter  = 0;
      }
    }
    else
    {
      digital_pin[i].last_state = auxiliar_digital_state;
      digital_pin[i].counter = 0;
    }
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
      analog_read_index=0;
    }

    analog_read_counter = 0;
  }
  else analog_read_counter++;
}

bool save_data_control()
{
  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    if(!seve_data( &(digital_pin[i].total_time_storage[0]) )) return false;
    if(!seve_data( &(digital_pin[i].total_time_storage[1]) )) return false;
  }
  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    if(!seve_data( &(analog_pin[i].average_storage) )) return false;
  }
  return true;
}

void gpio_data_control()
{
  if(get_flag_timer(&timer_input))
  {
    digital_data_control();
    analog_data_control();
  }
  if(get_flag_timer(&timer_save))
  {
    if(save_data_control()) Serial.println("ERROR: save gpio data failed");
  }
}

// FUNCTIONS GET ----------------------------------------------------------------------------------------------------

bool get_digital_pin(uint8_t pin)
{
  for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
  {
    if(pin == digital_pin[i].pin) return digital_pin[i].state;
  }
  return NULL;
}

uint16_t get_analog_pin(uint8_t pin)
{
  for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
  {
    if(pin == analog_pin[i].pin) return analog_pin[i].average;
  }
  return 0;
}

// ----------------------------------------------------------------------------------------------------




