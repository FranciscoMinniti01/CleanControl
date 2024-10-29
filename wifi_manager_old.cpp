// INCLUDES ----------------------------------------------------------------------------------------------------

#include "wifi_manager.h"
/*
// VARIABLES ----------------------------------------------------------------------------------------------------

StateWifiManager WifiState;

bool isconnected  = false;
bool isError      = false;
bool isAPstart    = false;
bool isCredential = false;
bool isParamData  = false;

WiFiMulti wifiMulti; 
WebServer server(80);

IPAddress local_ip_AP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
IPAddress gateway_AP(GATEWAY_IP_1, GATEWAY_IP_2, GATEWAY_IP_3, GATEWAY_IP_4);
IPAddress subnet_AP(SUBNET_IP_1, SUBNET_IP_2, SUBNET_IP_3, SUBNET_IP_4);

int16_t credential_storage[MAX_CREDENCIALES][2];
char ssids[MAX_CREDENCIALES][MAX_LEN_CREDENCIALES];
char passwords[MAX_CREDENCIALES][MAX_LEN_CREDENCIALES];

// PARAMETROS ESPECIALES
int16_t special_param_storage[NUM_SPECIAL_PARAM];
char machine_id[MAX_LEN_SPECIAL_PARAM];
char client_id[MAX_LEN_SPECIAL_PARAM];

// FUNCTIONS WEB SERVER ----------------------------------------------------------------------------------------------------
*/

/*
// FUNCTIONS GET ----------------------------------------------------------------------------------------------------

float getRSSI() { return WiFi.RSSI(); }

bool getWifiStatus() { return isconnected; }

bool getWiFiError() { return isError; }

const char* getSSID()
{
    strncpy(ssid, WiFi.SSID().c_str(), sizeof(ssid) - 1);  // Copia segura, dejando espacio para el terminador nulo
    ssid[sizeof(ssid) - 1] = '\0';                         // Asegura que el array esté terminado en null
    return ssid;
}

// FUNCTIONS ----------------------------------------------------------------------------------------------------


void WiFiManager_c()
{
  WifiState = WIFI_INIT;

  WiFi.onEvent(WiFiEvent);

  for (int i = 0; i < MAX_CREDENCIALES; i++)
  {
    for (int j = 0; j < MAX_LEN_CREDENCIALES; j++)
    {
      ssids[i][j] = 0;
      passwords[i][j] = 0;
    }

    credential_storage[i][0] = set_data(ssids[i]    , MAX_LEN_CREDENCIALES);
    credential_storage[i][1] = set_data(passwords[i], MAX_LEN_CREDENCIALES);
  }

  for (int i = 0; i < MAX_CREDENCIALES; i++)
  {
    if( get_data(credential_storage[i][0]) )
    {
      if( get_data(credential_storage[i][1]) ) isCredential = true;
    }
  }

  // PARAMETROS ESPECIALES
  special_param_storage[MACHINE_ID_INDEX]  = set_data(machine_id  ,MAX_LEN_SPECIAL_PARAM);
  special_param_storage[CLIENT_ID_INDEX]   = set_data(client_id   ,MAX_LEN_SPECIAL_PARAM);
  
  for(int i = OFSET ; i < OFSET+NUM_SPECIAL_PARAM ; i++)
  {
    if( get_data(special_param_storage[i]) ) isParamData = true;
    else
    {
      isParamData = false
      break;
    }
  }

}*/



