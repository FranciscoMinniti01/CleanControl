// INCLUDES ----------------------------------------------------------------------------------------------------

#include "wifi_manager.h"

// VARIABLES STORAGE PARAMETERS ----------------------------------------------------------------------------------------------------

int16_t credential_storage[MAX_CREDENCIALES][2];

// SPECIAL PARAMETERS
int16_t special_param_storage[NUM_SPECIAL_PARAM];

// VARIABLES SERVER ----------------------------------------------------------------------------------------------------

WebServer server(80);

#define MAX_HDMI_ROOT 2
hdmi_root_t     hdmi_root[MAX_HDMI_ROOT];
sv_param_t      sv_param;

// VARIABLES WIFI ----------------------------------------------------------------------------------------------------

WiFiMulti wifiMulti; 

IPAddress local_ip_AP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
IPAddress gateway_AP(GATEWAY_IP_1, GATEWAY_IP_2, GATEWAY_IP_3, GATEWAY_IP_4);
IPAddress subnet_AP(SUBNET_IP_1, SUBNET_IP_2, SUBNET_IP_3, SUBNET_IP_4);

State_Wifi_t  WifiState = WIFI_INIT;
bool wifi_error         = false;
bool wifi_is_connected  = false;
bool wifi_AP_init       = false;
bool wifi_server_init   = false;
bool wifi_STA_init      = false;

credentials_t  credentials[MAX_CREDENCIALES];

// FUNCTIONS STORAGE PARAMETERS ----------------------------------------------------------------------------------------------------

void set_storage()
{
  for (int i = 0; i < MAX_CREDENCIALES; i++)
  {
    credential_storage[i][0] = set_data((void*)credentials[i].ssid.c_str()    , MAX_LEN_CREDENCIALES);
    credential_storage[i][1] = set_data((void*)credentials[i].password.c_str(), MAX_LEN_CREDENCIALES);
  }

  // SPECIAL PARAMETERS
  special_param_storage[MACHINE_ID_INDEX]  = set_data((void*)sv_param.machine_id.c_str(), sizeof(sv_param.machine_id));
  special_param_storage[CLIENT_ID_INDEX]   = set_data((void*)sv_param.client_id.c_str() , sizeof(sv_param.client_id));
}

void get_storage()
{
  for (int i = 0; i < MAX_CREDENCIALES; i++)
  {
    if( get_data(credential_storage[i][0]) )
    {
      get_data(credential_storage[i][1]);
    }
  }

  for(int i = OFSET ; i < OFSET+NUM_SPECIAL_PARAM ; i++)
  {
    get_data(special_param_storage[i]);
  }
}

void save_Storage()
{
  for (int i = 0; i < MAX_CREDENCIALES; i++)
  {
    if( seve_data(credential_storage[i][0]) )
    {
      seve_data(credential_storage[i][1]);
    }
  }

  for(int i = OFSET ; i < OFSET+NUM_SPECIAL_PARAM ; i++)
  {
    seve_data(special_param_storage[i]);
  }
}

// FUNCTIONS SERVER ----------------------------------------------------------------------------------------------------

void FormRoot()
{
    String html = R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Formulario de Configuración</title>
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <style>
                /* Estilos generales */
                body {
                    font-family: Arial, sans-serif;
                    font-size: 18px;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 100vh;
                    margin: 0;
                    background-color: #f4f4f9;
                }
                h2 {
                    text-align: center;
                    color: #333;
                }
                /* Estilos para el formulario */
                form {
                    background-color: #fff;
                    padding: 20px;
                    border-radius: 8px;
                    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                    max-width: 400px;
                    width: 100%;
                    margin: 20px;
                }
                /* Estilos para las etiquetas y entradas del formulario */
                label {
                    display: block;
                    margin-bottom: 10px;
                    font-weight: bold;
                }
                input[type="text"], input[type="number"], input[type="password"] {
                    width: 100%;
                    padding: 10px;
                    margin-bottom: 15px;
                    border: 1px solid #ccc;
                    border-radius: 5px;
                    font-size: 16px;
                }
                /* Botón de envío */
                input[type="submit"] {
                    background-color: #4CAF50;
                    color: white;
                    padding: 10px 20px;
                    border: none;
                    border-radius: 5px;
                    cursor: pointer;
                    font-size: 18px;
                    width: 100%;
                }
                input[type="submit"]:hover {
                    background-color: #45a049;
                }
            </style>
        </head>
        <body>
            <form action="/submit" method="POST">
                <h2>-</h2>
                <h2>-</h2>
                <h2>-</h2>
                <h2>-</h2>
                <h2>WiFi Credentials</h2>
    )rawliteral";

    for(uint8_t i = 1; i<=MAX_CREDENCIALES; i++)
    {
        html += "            <p>SSID " + String(i) + " actual: " + credentials[i-1].ssid + "</p>";
        html += "            <label>SSID " + String(i) + ":</label>";
        html += "            <input type=\"text\" name=\"ssid" + String(i) + "\">";

        html += "            <p>Password " + String(i) + " actual: " + credentials[i-1].password + "</p>";            
        html += "            <label>Password " + String(i) + ":</label>";
        html += "            <input type=\"password\" name=\"password" + String(i) + "\">";
    }

    html += "            <h2>Configuration</h2>";

    //Parametrizacion especial
    html += "            <p>Current machine ID: " + sv_param.machine_id + "</p>";            
    html += "            <label>Machine ID:</label>";
    html += "            <input type=\"text\" name=\"machineid\">";

    html += "            <p>Current client ID: " + sv_param.client_id + "</p>";            
    html += "            <label>Client ID:</label>";
    html += "            <input type=\"text\" name=\"clientid\">";

    html += R"rawliteral(            
                <input type="submit" value="Enviar">
            </form>
        </body>
        </html>
    )rawliteral";

    server.send(200, "text/html", html);
}

void FormSubmitRoot()
{
  String html = R"rawliteral(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Confirmación</title>
            <style>
                /* Estilos generales */
                body {
                    font-family: Arial, sans-serif;
                    font-size: 18px;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 100vh;
                    margin: 0;
                    background-color: #f4f4f9;
                }
                h3 {
                    text-align: center;
                    color: #333;
                }
                .message-box {
                    background-color: #fff;
                    padding: 20px;
                    border-radius: 8px;
                    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                    max-width: 400px;
                    width: 100%;
                    margin: 20px;
                    text-align: center;
                }
            </style>
        </head>
        <body>
            <div class="message-box">
                <h3>Datos recibidos correctamente</h3>
                <p>Gracias por enviar sus datos.</p>
            </div>
        </body>
        </html>
  )rawliteral";

  for(uint8_t i = 0; i<MAX_CREDENCIALES; i++)
  {
    if(server.arg("ssid"+String(i+1)))      credentials[i].ssid = server.arg("ssid"+String(i+1));
    if(server.arg("password"+String(i+1)))  credentials[i].password = server.arg("password"+String(i+1));
  }   
  
  //Parametrizacion especial
  if(server.arg("machineid")) sv_param.machine_id = server.arg("machineid");
  if(server.arg("clientid"))  sv_param.client_id  = server.arg("clientid");

  save_Storage();

  //server.send(200, "text/html", "<h3>Datos recibidos correctamente</h3>");
  server.send(200, "text/html", html);
}

void server_init()
{
  hdmi_root[0].root     = "/";
  hdmi_root[0].request  = HTTP_GET;
  hdmi_root[0].fun      = FormRoot;

  hdmi_root[1].root     = "/submit";
  hdmi_root[1].request  = HTTP_POST;
  hdmi_root[1].fun      = FormSubmitRoot;
}

// FUNCTIONS WIFI ----------------------------------------------------------------------------------------------------

void WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
// GENERAL ----------------------------------------------------------------------------------------------------
        case ARDUINO_EVENT_WIFI_READY:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: WIFI_READY");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: Completed scan for access points");    
            #endif
            break;
// STATION MODE ----------------------------------------------------------------------------------------------------
        case ARDUINO_EVENT_WIFI_STA_START:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: WiFi client started");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: WiFi clients stopped");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: Connected to access point");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: Disconnected from WiFi access point");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: Authentication mode of access point has changed");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            #ifdef DEBUG_WIFI
            Serial.print("EVENT: Obtained IP address: ");
            Serial.println(WiFi.localIP());
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: Lost IP address and IP address is reset to 0");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: STA IPv6 is preferred");    
            #endif
            break;

// ERRORES CREO ----------------------------------------------------------------------------------------------------
        
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: WiFi Protected Setup (WPS): succeeded in enrollee mode");    
            #endif
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: WiFi Protected Setup (WPS): failed in enrollee mode");    
            #endif
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: WiFi Protected Setup (WPS): timeout in enrollee mode");    
            #endif
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: WiFi Protected Setup (WPS): pin code in enrollee mode");    
            #endif
            break;

// ACCES POINT MODE ----------------------------------------------------------------------------------------------------
        
        case ARDUINO_EVENT_WIFI_AP_START:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: AP STARTED");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: AP STOPPED");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: AP Client connected");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: AP Client disconnected");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: AP Assigned IP address to client");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            #ifdef DEBUG_WIFI
            Serial.println("EVENT: AP Received probe request");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            #ifdef DEBUG_WIFI
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
  bool flag = false;

  switch (WifiState)
  {
    case WIFI_INIT:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: WIFI_READY");    
      #endif
      
      set_storage();
      get_storage();
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
      flag = false;

      for(uint8_t i = 0 ; i<MAX_HDMI_ROOT ; i++)
      {
        if(hdmi_root[i].fun != NULL )
        {
          server.on(hdmi_root[i].root, hdmi_root[i].request, hdmi_root[i].fun);
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
      flag = false;

      server_init();
      for (int i = 0; i < MAX_CREDENCIALES; i++)
      {
        if(!(credentials[i].ssid).isEmpty())
        {
          if(!wifiMulti.addAP(credentials[i].ssid.c_str(),credentials[i].password.c_str())) Serial.printf("ERROR: add credential %d failed\n",i);
          flag = true;
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
      WiFi.softAPdisconnect(false);

      wifi_AP_init = false;
      WifiState = WIFI_DEINIT;
      break; 

    case WIFI_DEINIT:
      WiFi.mode(WIFI_MODE_NULL);

      WifiState = WIFI_INIT;
      break;
  }
}

// FUNCTIONS GET ----------------------------------------------------------------------------------------------------

float getRSSI() { return WiFi.RSSI(); }

bool getWifiStatus() { return wifi_is_connected; }

bool getWiFiError() { return wifi_error; }

String getSSID() {
    return WiFi.SSID();
}

sv_param_t* getSpecialParam(){ return &sv_param; }

