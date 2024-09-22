//#include "wifi_manager.h"
#include "utils.h"
//#include <Preferences.h>

////WiFiManager_c wifimanager;

//Preferences preferencess;

char ssids[3][30];
bool i = true;

void setup() {
  Serial.begin(115200);
  Serial.println("TEST");
}

void looop()
{
    //wifimanager.WiFiStateMachine();
    nsv_PrepareSpace("credentials");
    if(i)
    {
      strcpy(ssids[0], "Francisco");
      strcpy(ssids[1], "Francis");
      strcpy(ssids[2], "Fran");
      for (int i = 0; i < 3; i++) {
          String ssidString = String(ssids[i]);
          Serial.println("SSID " + String(i) + ": " + ssidString);
      }
      nsv_PutData("ssid", static_cast<void*>(ssids[1]) , sizeof(ssids[1]) );
      i = false;
      nsv_CloseSpace();
    }
}

/*void loop(){
    if( i ){
        if( !preferencess.begin("credentials") )  Serial.println("DEBUG: open space failed");
        strcpy(ssids[0], "Francisco");
        strcpy(ssids[1], "Francis");
        strcpy(ssids[2], "Fran");
        for (int i = 0; i < 3; i++) {
            String ssidString = String(ssids[i]);
            Serial.println("SSID " + String(i) + ": " + ssidString);
        }
        if( preferencess.putBytes("ssid", static_cast<void*>(ssids[1]) , sizeof(ssids[1]) ) == 0){
        Serial.println("DEBUG: putBytes failed");   } 
        i = false;
        preferencess.end();
    }
}*/



