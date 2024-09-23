#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <WiFiMulti.h>
#include <string>
#include <Preferences.h>


#include "utils.h"
#include "config.h"

//----------------------------------------------------------------------------------------------------

#define WIFI_NAME_SPACE "credentials"
#define WIFI_KEY_PASW   "pasw"
#define WIFI_KEY_SSID   "ssid"

//----------------------------------------------------------------------------------------------------

#define DEBUG_WIFI

// CONFIG ACCESS POINT ----------------------------------------------------------------------------------------------------

#define CHANNEL_AP        1
#define SSID_HIDDEN_AP    0

#define LOCAL_IP_1 192
#define LOCAL_IP_2 168
#define LOCAL_IP_3 4
#define LOCAL_IP_4 1

#define GATEWAY_IP_1 192
#define GATEWAY_IP_2 168
#define GATEWAY_IP_3 4
#define GATEWAY_IP_4 1

#define SUBNET_IP_1 255
#define SUBNET_IP_2 255
#define SUBNET_IP_3 255
#define SUBNET_IP_4 0

//----------------------------------------------------------------------------------------------------

enum StateWifiManager
{
    WIFI_CREDENTIALS_CHECK,
    WIFI_AP_INIT,
    WIFI_AP_WAIT_INIT,
    WIFI_AP_SERVER_INIT,
    WIFI_AP_WAIT_CONNECTION,
    WIFI_AP_DISCONNECTION,

    WIFI_STA_INIT,
    WIFI_STA_CONNECTING,
    WIFI_STA_READY
};

class WiFiManager_c
{
    public:
        void WiFiManager();
        void WiFiStateMachine();

    private:
        StateWifiManager WifiState;
        WiFiMulti wifiMulti;
        char ssids[MAX_CREDENCIALES][MAX_LEN_CREDENCIALES];
        char passwords[MAX_CREDENCIALES][MAX_LEN_CREDENCIALES];

        bool GetCredentials();
        bool SaveCredentials();//const char* ssid, const char* password);

        void WiFiEventCB(WiFiEvent_t event, WiFiEventInfo_t info);
        void ServerManager();
};

#endif // WIFI_MANAGER_H

