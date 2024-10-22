// INCLUDE ----------------------------------------------------------------------------------------------------

#include "data_class.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

digital_pin_t digital_pin[NUMBER_OF_DIGITAL_PIN];
analog_pin_t  analog_pin[NUMBER_OF_ANALOG_PIN];

int16_t timer_input;
int16_t timer_save;

Preferences pref;

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
    analog_pin[0].pin = ANALOG_PIN_UNO;
    // -----------------------------------

    for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++) {
        pinMode(digital_pin[i].pin,INPUT);    
    }
}

void input_control()
{
    static uint8_t auxiliar_digital = 0;

    static uint8_t  analogRead_counter = 0;
    static uint16_t analog_states[NUMBER_OF_ANALOG_PIN][COUNTER_COMPARATOR];
    static uint8_t  analog_states_index = 0;
    static uint16_t auxiliar_Average = 0;

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
                    digital_pin[i].state = auxiliar_digital;
                    digital_pin[i].counter = 0;
                    digital_pin[i].time_counter = 0;
                }
            }
            else {
                digital_pin[i].last = auxiliar_digital;
                digital_pin[i].counter = 0;
            }
            if(digital_pin[i].state) digital_pin[i].time_counter ++;
        }

        if(analogRead_counter >= COUNTER_COMPARATOR)
        {
            for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
            {
                analog_states[i][analog_states_index] = analogRead(analog_pin[i].pin);
                for(uint8_t j = 0 ; j<COUNTER_COMPARATOR ; j++){
                    auxiliar_Average += analog_states[i][j];
                    analog_pin[i].Average = auxiliar_Average/COUNTER_COMPARATOR;
                }
            }
            analog_states_index++;
            if(analog_states_index >= COUNTER_COMPARATOR) analog_states_index=0;
            analogRead_counter = 0;
        }else analogRead_counter++;
    }

    if(get_flag_timer(timer_save))
    {
        if(!SaveData()) Serial.println("ERROR: Save data failed");
    }
}


bool SaveData()
{
    bool err = true;
    if( !pref.begin(DATA_NAME_SPACE) ) //Serial.println("ERROR: Preferences begin failed");

    for(uint8_t i = 0 ; i<NUMBER_OF_DIGITAL_PIN ; i++)
    {
        if(!pref.putBytes( (std::string(DATA_KEY_DIGTAL) + std::to_string(i)).c_str() , &(digital_pin[i].time_counter) , sizeof( digital_pin[i].time_counter )))
        {
            //Serial.println("ERROR: Save digital %d data",i);
            err = false;
        }
    }

    for(uint8_t i = 0 ; i<NUMBER_OF_ANALOG_PIN ; i++)
    {
        if(!pref.putBytes( (std::string(DATA_KEY_ANALOG) + std::to_string(i)).c_str() , &( analog_pin[i].Average ) , sizeof( analog_pin[i].Average )))
        {
            //Serial.println("ERROR: Save analog %d data",i);
            err = false;
        }
    }

    pref.end();
    return err;
}





