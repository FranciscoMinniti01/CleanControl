// INCLUDE ----------------------------------------------------------------------------------------------------

#include "my_timer.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

hw_timer_t* timer    = NULL;
bool flag            = true;

my_timer_t my_timers[MAX_NUM_TIMERS];

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void timer_callback()
{
  for(uint16_t i = 0 ; i<MAX_NUM_TIMERS ; i++ )
  {
    if(my_timers[i].index < 0) continue;
    my_timers[i].conter ++;
    if(my_timers[i].comparator == my_timers[i].conter)
    {
      my_timers[i].flag = true;
      my_timers[i].conter = 0;
    }
  }
}

bool timer_init()
{
  timer = timerBegin(TIME_FREC);
  if(timer == NULL)
  {
      Serial.println("ERROR: Timer init failed");
      return false;
  }
  timerAttachInterrupt(timer, &timer_callback);
  timerAlarm(timer, BASE_TIME_10mS, true, 0);
  return true;
}

bool set_timer(uint16_t comparator)
{
  for(uint16_t i = 0 ; i<MAX_NUM_TIMERS ; i++ )
  {
    if(my_timers[i].index < 0)
    {
      my_timers[i].index = i;
      my_timers[i].comparator = comparator;
      my_timers[i].flag = false;
      my_timers[i].conter = 0;
      return true;
    }
  }
  return false;
}

bool get_flag_timer(time_t index)
{
  if(index >= 0 && index<MAX_NUM_TIMERS)
  {
    bool flag = my_timers[index].flag;
    my_timers[index].flag = false;
    return flag;
  } else return false;
}

// ----------------------------------------------------------------------------------------------------

