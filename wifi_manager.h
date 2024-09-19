#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <string>

#include "utils.h"
#include "web_server.h"
#include "config.h"

//----------------------------------------------------------------------------------------------------

#define DEBUG_WIFI

#define WIFI_NAME_SPACE "wifi"
#define WIFI_KEY_PASW   "pasw_"
#define WIFI_KEY_SSID   "ssid_"

//----------------------------------------------------------------------------------------------------

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)

//----------------------------------------------------------------------------------------------------

enum StateWifiManager
{
    WIFI_CREDENTIALS_CHECK,
    WIFI_AP_INIT,
    WIFI_AP_WAIT_INIT,
    WIFI_AP_SERVER_INIT,
    WIFI_AP_WAIT_CONNECTION,
    WIFI_AP_DISCONNECTION,
    


    WiFi_state_STA
};

class WiFiManager_c
{
    public:
        WiFiManager();
        void WiFiStateMachine()

    private:
        StateWifiManager WifiState;
        char ssids[MAX_CREDENCIALES][MAX_LEN_CREDENCIALES];       // Array para almacenar los nombres de redes
        char passwords[MAX_CREDENCIALES][MAX_LEN_CREDENCIALES];   // Array para almacenar las contraseñas

        void intentarReconexion();
        void iniciarAccessPoint();
        void guardarCredenciales(const char* ssid, const char* password);
        void cargarCredencialesGuardadas();

        bool GetCredentials()
        void SaveCredentials(const char* ssid, const char* password)
};

#endif // WIFI_MANAGER_H

