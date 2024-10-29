// INCLUDES ----------------------------------------------------------------------------------------------------

#include "wifi_manager.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

WebServer server(80);

State_Wifi_t  WifiState = WIFI_INIT;
bool wifi_error         = false;
bool wifi_is_connected  = false;
bool wifi_AP_init       = false;
bool wifi_server_init   = false;
bool wifi_STA_init      = false;

#define MAX_HDMI_ROOT 2
hdmi_root_t*    hdmi_root[MAX_HDMI_ROOT];
credentials_t*  credentials[MAX_CREDENCIALES];

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
// GENERAL ----------------------------------------------------------------------------------------------------
        case ARDUINO_EVENT_WIFI_READY:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: WIFI_READY");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: Completed scan for access points");    
            #endif
            break;
// STATION MODE ----------------------------------------------------------------------------------------------------
        case ARDUINO_EVENT_WIFI_STA_START:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: WiFi client started");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: WiFi clients stopped");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: Connected to access point");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: Disconnected from WiFi access point");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: Authentication mode of access point has changed");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            #ifdef(DEBUG_WIFI)
            Serial.print("EVENT: Obtained IP address: ");
            Serial.println(WiFi.localIP());
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: Lost IP address and IP address is reset to 0");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: STA IPv6 is preferred");    
            #endif
            break;

// ERRORES CREO ----------------------------------------------------------------------------------------------------
        
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: WiFi Protected Setup (WPS): succeeded in enrollee mode");    
            #endif
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: WiFi Protected Setup (WPS): failed in enrollee mode");    
            #endif
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: WiFi Protected Setup (WPS): timeout in enrollee mode");    
            #endif
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: WiFi Protected Setup (WPS): pin code in enrollee mode");    
            #endif
            break;

// ACCES POINT MODE ----------------------------------------------------------------------------------------------------
        
        case ARDUINO_EVENT_WIFI_AP_START:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: AP STARTED");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: AP STOPPED");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: AP Client connected");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: AP Client disconnected");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: AP Assigned IP address to client");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: AP Received probe request");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            #ifdef(DEBUG_WIFI)
            Serial.println("EVENT: AP IPv6 is preferred");    
            #endif
            break;
        default: break;
    }
}

void WiFi_manager()
{
  WiFiStateMachine();
  if(wifi_server_init) server.handleClient();
  if(wifi_STA_init) wifiMulti.run();
}

void WiFiStateMachine()
{
  switch (WifiState)
  {
    case WIFI_INIT:
      #ifdef(DEBUG_WIFI)
      Serial.println("EVENT: WIFI_READY");    
      #endif
      
      WiFi.onEvent(WiFiEvent);
      if(!WiFi.mode(WIFI_AP_STA)) Serial.println("ERROR: WiFi mode set failed");
      
      WifiState = WIFI_AP_INIT;
      break; 

    case WIFI_AP_INIT:
      if(!WiFi.softAPConfig(local_ip_AP, gateway_AP, subnet_AP)) Serial.println("ERROR: soft ap config failed");
      if(!WiFi.softAP(SSID_AP, PASSWORD_AP)) Serial.println("ERROR: AP init failed");
      Serial.println(WiFi.softAPIP());
      //if(!WiFi.softAP(SSID_AP, PASSWORD_AP, CHANNEL_AP, SSID_HIDDEN_AP, MAX_CONNECTION_AP)) Serial.println("ERROR: AP init failed");
      //Function used to enable the IPv6 support. bool softAPenableIpV6();
      
      wifi_AP_init = true;
      WifiState = WIFI_AP_SERVER_INIT;
      break; 
        
    case WIFI_AP_SERVER_INIT:
      bool flag = false;

      for(uint8_t i = 0 ; i<MAX_HDMI_ROOT ; i++)
      {
        if(hdmi_root[i] != NULL)
        {
          server.on(hdmi_root[i]->root, hdmi_root[i]->request, hdmi_root[i]->fun);
          flag = true;
        }
      }

      if(flag)
      {
        server.begin();
      
        wifi_server_init = true;
        WifiState = WIFI_STA_INIT;
      }
      break; 

    case WIFI_STA_INIT:
      bool flag = false;

      for (int i = 0; i < MAX_CREDENCIALES; i++)
      {
        if(credentials[i] != NULL)
        {
          if(!wifiMulti.addAP(credentials[i].ssids,credentials[i].passwords)) Serial.printf("ERROR: add credential %d failed\n",i);
          flag = true
        }
      }

      if(flag)
      {
        wifi_STA_init = true;
        WifiState = WIFI_READY;
      }
      break;
    
    case WIFI_READY:
      break;
    
    case WIFI_STA_DEINIT:
      if(!WiFi.disconnect()) Serial.println("ERROR: STA disconnect failed");

      wifi_STA_init = false;
      WifiState = WIFI_AP_SERVER_DEINIT;
      break; 
    
    case WIFI_AP_SERVER_DEINIT:
      server.close();
      server.stop();

      wifi_server_init = false;
      WifiState = WIFI_AP_DEINIT;
      break; 

    case WIFI_AP_DEINIT:
      softAPdisconnect(false);

      wifi_AP_init = false;
      WifiState = WIFI_DEINIT;
      break; 

    case WIFI_DEINIT:
      WiFi.mode(WIFI_MODE_NULL);

      WifiState = WIFI_INIT;
      break;
  }
}




