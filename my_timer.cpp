// INCLUDE ----------------------------------------------------------------------------------------------------

#include "my_timer.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

hw_timer_t* timer    = NULL;
bool flag            = true;

std::vector<my_timer_t*> my_timers;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void timer_callback()
{
  for(my_timer_t* t : my_timers)
  {
    t->conter ++;
    if(t->comparator == t->conter)
    {
      t->flag    = true;
      t->conter  = 0;
      if(t->cb != NULL) t->cb();
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
  timerAlarm(timer, BASE_TIME, true, 0);
  return true;
}

void set_timer(my_timer_t* ptr, uint16_t comparator, cb_timer cb)
{
  my_timers.push_back(ptr);
  reset_timer(ptr,comparator);
  if(cb != NULL) ptr->cb = cb;
}

void reset_timer(my_timer_t* ptr, uint16_t comparator)
{
  ptr->comparator = comparator;
  ptr->conter     = 0;
  ptr->flag       = false;
}

bool get_flag_timer(my_timer_t* ptr)
{
  bool flag = ptr->flag;
  ptr->flag = false;
  return flag;
}

void timer_deinit()
{
  my_timers.clear();
  timerEnd(timer);
}

// ----------------------------------------------------------------------------------------------------

