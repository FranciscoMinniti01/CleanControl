// INCLUDES ----------------------------------------------------------------------------------------------------
#include "wifi_manager.h"
#include "time.h"

//----------------------------------------------------------------------------------------------------

hw_timer_t* timer_ON   = NULL; 
hw_timer_t* timer_StateOnOff   = NULL;


WiFiManager_c wifimanager;

//----------------------------------------------------------------------------------------------------

void Fun_timer_ON()
{
}

void Fun_timer_StateOnOff()
{
}

void setup()
{
    Serial.begin(115200);

    timer_MinOn = timerBegin(1000000);
    timerAttachInterrupt(timer_MinOn, &fun);
    timerAlarm(timer_MinOn, 10000000, true, 0);

    Serial.println("\n\n\n\n\nAPP INIT");
}

void loop()
{
    wifimanager.WiFiStateMachine();

    if(wifimanager.getWifiStatus()){

    }

    if(wifimanager.getWifiStatus()){

    }
}



