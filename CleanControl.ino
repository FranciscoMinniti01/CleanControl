#include "wifi_manager.h"

WiFiManager_c wifimanager;

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    wifimanager.WiFiStateMachine();
}



