// INCLUDE ----------------------------------------------------------------------------------------------------

#include "TimerManager.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

hw_timer_t* timer       = NULL;
hw_timer_t* total_timer = NULL;

static std::vector<my_timer_t*> timers_list;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

static void timer_callback()
{
  portENTER_CRITICAL_ISR(&timerMux);
  for(my_timer_t* t : timers_list)
  {
    t->counter ++;
    if(t->comparator == t->counter)
    {
      t->flag     = true;
      t->counter  = 0;
      if(t->callback != NULL) t->callback();
    }
  }
  portEXIT_CRITICAL_ISR(&timerMux);
}

bool timer_init()
{
  timer = timerBegin(TIME_FREQUENCY);
  total_timer = timerBegin(TIME_FREQUENCY);
  if(timer == NULL || total_timer == NULL)
  {
    Serial.println("ERROR: Timer init failed");
    return false;
  }
  timerAttachInterrupt(timer, &timer_callback);
  timerAlarm(timer, TIME_BASE, true, 0);
  return true;
}

void set_timer(my_timer_t* ptr, uint16_t comparator, cb_timer cb)
{
  timers_list.push_back(ptr);
  reset_timer(ptr,comparator);
  if(cb != NULL) ptr->callback = cb;
}

void reset_timer(my_timer_t* ptr, uint16_t comparator)
{
  ptr->comparator = comparator;
  ptr->counter     = 0;
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
  timers_list.clear();
  timerEnd(timer);
}

uint64_t get_time_us()
{ 
  return timerReadMicros(total_timer);
}

uint64_t get_delta_time_us(uint64_t ot)
{
  uint64_t nt = timerReadMicros(total_timer);
  if(nt<ot) return 0;
  return (nt-ot);
}

// ----------------------------------------------------------------------------------------------------

