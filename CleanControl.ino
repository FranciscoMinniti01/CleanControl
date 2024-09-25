// INCLUDES ----------------------------------------------------------------------------------------------------
#include "wifi_manager.h"
#include "time.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

hw_timer_t* timer_DataON    = NULL;
bool flag_DataON            = true;
hw_timer_t* timer_DataWifi  = NULL;
bool flag_DataWifi          = true;

WiFiManager_c wifimanager;

//----------------------------------------------------------------------------------------------------

void fun_DataON(){
    flag_DataON = true;
}

void fun_DataWifi() {
    flag_DataWifi = true;
}

void setup()
{
    Serial.begin(115200);

    timer_DataON = timerBegin(1000000);
    timerAttachInterrupt(timer_DataON, &fun_DataON);
    timerAlarm(timer_DataON, 10000000, true, 0);            //Llamada cada 10 seg

    timer_DataWifi = timerBegin(1000000);
    timerAttachInterrupt(timer_DataWifi, &fun_DataWifi);
    timerAlarm(timer_DataWifi, 10000000, true, 0);           //Llamada cada 10 seg

    Serial.println("\n\n\n\n\nAPP INIT");
}

void loop()
{
    wifimanager.WiFiStateMachine();

    if(wifimanager.getWifiStatus()){
        if(flag_DataON) {

            flag_DataON = false;
        }
    }

    if(wifimanager.getWifiStatus()){
        if(flag_DataWifi) {

            flag_DataWifi = false;
        }
    }
}



