#include "wifi_manager.h"
#include "influxdb_client.h"

#include "time.h"
hw_timer_t * timer = NULL;

bool mainflag = true;
bool funflag  = true;

WiFiManager_c wifimanager;

void fun(){
    funflag = true;
}

void setup()
{
    Serial.begin(115200);
    timer = timerBegin(1000000);
    timerAttachInterrupt(timer, &fun);
    timerAlarm(timer, 10000000, true, 0);
}

void loop()
{
    wifimanager.WiFiStateMachine();

    if(wifimanager.getWifiStatus()){
        if(mainflag){
            connect();
            mainflag=false;
        }
    }else{
        mainflag=true;
    }

    if(wifimanager.getWifiStatus()){
      if(funflag){
          sendData(wifimanager.getRSSI());
          funflag = false;
      }
    }
}



