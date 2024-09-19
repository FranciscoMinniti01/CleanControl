#include "wifi_manager.h"

WiFiServer server(80);

WiFiManager_c::WiFiManager()
{
    WifiState = Estado_Inicializacion;
    WiFi.onEvent(WiFiEventCB);
}

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
    #if !defined(DEBUG) && !defined(DEBUG_WIFI)
    Serial.printf("[WiFi-event] event: %d\n", event);
    #endif

    switch (event)
    {
// GENERAL ----------------------------------------------------------------------------------------------------
        case ARDUINO_EVENT_WIFI_READY:
            Serial.println("WIFI_READY");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
// STATION MODE ----------------------------------------------------------------------------------------------------
        case ARDUINO_EVENT_WIFI_STA_START:
            Serial.println("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            Serial.println("STA IPv6 is preferred");
            break;

// ERRORES CREO ----------------------------------------------------------------------------------------------------
        
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;

// ACCES POINT MODE ----------------------------------------------------------------------------------------------------
        
        case ARDUINO_EVENT_WIFI_AP_START:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("WiFi access point started");
            #endif
            WifiState = WIFI_AP_SERVER_INIT;
            break;

        case ARDUINO_EVENT_WIFI_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            Serial.println("AP IPv6 is preferred");
            break;

        default: break;
    }
}

void WiFiManager::WiFiStateMachine()
{
    switch (WifiState)
    {
        case WIFI_CREDENTIALS_CHECK:
            if(GetCredentials()) WifiState = WiFi_start_STA
            else WifiState = WiFi_start_AP
            break

        case WIFI_AP_INIT:
            WiFi.mode(WIFI_AP_STA);
            WiFi.softAP(SSID_AP, PASSWORD_AP, CHANNEL_AP, SSID_HIDDEN_AP, MAX_CONNECTION_AP);
            Serial.println(WiFi.softAPIP());
            WifiState = WIFI_AP_WAIT_INIT;
            break;

        case WIFI_AP_WAIT_INIT
            break;

        case WIFI_AP_SERVER_INIT
            server.begin();
            break;
          
        case WIFI_AP_WAIT_CONNECTION
            WiFiClient client = server.available();
            if (client) {
            }
            break;

        case 

        case WiFi_start_STA
            WiFi.softAPdisconnect(false);
            WiFi.mode(WIFI_STA);
            WiFi.setAutoReconnect(true);
            break;

        case Estado_IntentarConexionWiFi:
            for (int i = 0; i < MAX_CREDENCIALES; ++i)
            {
                WiFi.begin(ssids[i], passwords[i]);
                if (WiFi.waitForConnectResult() == WL_CONNECTED)
                {
                    estadoActual = Estado_MantenerConexionEstablecida;
                    return;
                }
            }
            break;

        case Estado_MantenerConexionEstablecida:
            if (WiFi.status() != WL_CONNECTED)
            {
                estadoActual = Estado_Intentar_Reconexion;
            }
            break;

        case Estado_Intentar_Reconexion:
            if (WiFi.reconnect())
            {
                estadoActual = Estado_MantenerConexionEstablecida;
            }
            else
            {
                estadoActual = Estado_Modo_AccessPoint;
            }
            break;

        case Estado_Modo_AccessPoint:
            iniciarAccessPoint();
            //estadoActual = Estado_EsperandoConexionAP;
            break;
    }
}

bool WiFiManager::GetCredentials()
{
    bool err = false;

    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        ssids[i] = NULL;
        passwords[i] = NULL;
    }

    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        if(!GetData(WIFI_NAME_SPACE, WIFI_KEY_SSID+std::to_string(i) , ssids[i], sizeof(ssids[i])))
        {
            break;
        }
        if(!GetData(WIFI_NAME_SPACE, WIFI_KEY_PASW+std::to_string(i) , passwords[i], sizeof(passwords[i])))
        {
            break;
        }
        err = true;
    }
    return err;
}

void WiFiManager::SaveCredentials(const char* ssid, const char* password)
{
    bool err = false;
    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        if(ssids[i] != NULL)
        {
            if(!PutData(WIFI_NAME_SPACE, WIFI_KEY_SSID+std::to_string(i) , ssids[i], sizeof(ssids[i])))
            {
                break;
            }
        }
        if(passwords[i] != NULL)
        {
            if(!PutData(WIFI_NAME_SPACE, WIFI_KEY_PASW+std::to_string(i) , passwords[i], sizeof(passwords[i])))
            {
                break;
            }
        }
        err = true;
    }
    return err;
}


