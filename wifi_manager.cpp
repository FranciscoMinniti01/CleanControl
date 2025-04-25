// INCLUDES ----------------------------------------------------------------------------------------------------

#include "wifi_manager.h"

// VARIABLES SERVER ----------------------------------------------------------------------------------------------------

WebServer server(80);

// VARIABLES CREDENTIALS ----------------------------------------------------------------------------------------------------

credentials_t  credentials[MAX_CREDENCIALES];
storage_t credentials_storage[MAX_CREDENCIALES][2];

// VARIABLES WIFI ----------------------------------------------------------------------------------------------------

#define DEBUG_WIFI 1
WiFiMulti wifiMulti; 

IPAddress local_ip_AP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
IPAddress gateway_AP(GATEWAY_IP_1, GATEWAY_IP_2, GATEWAY_IP_3, GATEWAY_IP_4);
IPAddress subnet_AP(SUBNET_IP_1, SUBNET_IP_2, SUBNET_IP_3, SUBNET_IP_4);

static State_Wifi_t  WifiState  = WIFI_INIT;
static State_AP_t    APState    = WIFI_AP_INIT;
static State_STA_t   STAState   = WIFI_STA_INIT;

static bool has_wifi_error      = false;            // Inidca si hay un error en las maquinas de estado, si esta en true todas las maquinas se van a deinit
static bool is_wifi_connected   = false;            // Indica si el dispositivo esta conectado como cliente a una red wifi 
static bool is_wifi_init        = false;            // Indica que se inicializo la libreria wifi
static bool is_wifi_AP_init     = false;            // Indica que se inicializo el modo AP
static bool is_wifi_STA_init    = false;            // Indica que se inicializo el modo STA

// FUNCTIONS SERVER ----------------------------------------------------------------------------------------------------

void set_hdmi_root(String root_ , HTTPMethod request_ , handle_fun fun_)
{
  server.on(root_, request_, fun_);
}

// FUNCTIONS CREDENTIALS ----------------------------------------------------------------------------------------------------

bool register_credentials()
{
  uint8_t i = 0;
  bool flag = false;
  wifiMulti.APlistClean();
  Serial.printf("DEBUG: register_credentials()\n");
  for(i=0 ; i<3; i++)
  {
    if (credentials[i].ssid[0] != 0)
    {
      Serial.printf("INFO: register credential %d\n",i);
      if (credentials[i].password[0] == 0) Serial.printf("    with empty password\n");
      if(!wifiMulti.addAP(credentials[i].ssid,credentials[i].password))
      {
        Serial.printf("ERROR: add credential %d failed\n",i);
      }
      else flag = true;
    }
  }
  return flag;
}

void get_storage_credentials()
{
  bool flag = false;
  Serial.printf("DEBUG: get_storage_credentials()\n");
  for(uint8_t i = 0; i<MAX_CREDENCIALES; i++)
  {
    set_data_storage( &(credentials_storage[i][INDEX_SSID]), 
                      (void*)credentials[i].ssid,
                      MAX_LEN_CREDENCIALES,
                      KEY_CREDENTIAL_SSID + String(i) );

    set_data_storage( &(credentials_storage[i][INDEX_PASSWORD]), 
                      (void*)credentials[i].password,
                      MAX_LEN_CREDENCIALES,
                      KEY_CREDENTIAL_PASSWORD + String(i) );

    get_data(&credentials_storage[i][INDEX_SSID]);
    get_data(&credentials_storage[i][INDEX_PASSWORD]);
    
    if(credentials[i].ssid[0] != 0)
    {
      flag = true;
      Serial.printf("INFO: Se obtuvieron la credencial %d:\n     ssid=%s\n     password=%s\n",i,credentials[i].ssid ,credentials[i].password);
    }
  }
  if(flag) if(!register_credentials()) Serial.printf("ERROR: can not register credentials\n"); 
}

bool set_credentials(String ssid, String password)
{
  Serial.printf("DEBUG: set_credentials()\n");
  static uint8_t index = -1;

  if(ssid.isEmpty())
  {  
    Serial.printf("ERROR: can not set empty ssid\n"); 
    return false;
  }
  if(password.isEmpty()) Serial.printf("Empty password\n"); 

  index++;
  index = index%MAX_CREDENCIALES;
  Serial.printf("INFO: Indice de la nueva credencial = %d\n",index);

  strncpy(credentials[index].ssid, ssid.c_str(), MAX_LEN_CREDENCIALES);
  credentials[index].ssid[MAX_LEN_CREDENCIALES-1] = '\0';

  strncpy(credentials[index].password, password.c_str(), MAX_LEN_CREDENCIALES);
  credentials[index].password[MAX_LEN_CREDENCIALES-1] = '\0';

  if(!seve_data(&credentials_storage[index][INDEX_SSID]))
  {
    Serial.printf("ERROR: can not seve ssid\n"); 
    return false;
  }
  if(!seve_data(&credentials_storage[index][INDEX_PASSWORD]))
  {
    Serial.printf("ERROR: can not seve password\n"); 
    return false;
  }

  if(!register_credentials())
  {
    Serial.printf("ERROR: can not register credentials\n"); 
    return false;
  }

  Serial.printf("INFO: Configuracion de credenciales\n     ssid=%s \n     password=%s\n",credentials[index].ssid ,credentials[index].password);
  return true;
}

// FUNCTIONS WIFI ----------------------------------------------------------------------------------------------------

void WiFiEvent(WiFiEvent_t event)
{  
  switch (event)
  {
 // GENERAL ----------------------------------------------------------------------------------------------------
    case ARDUINO_EVENT_WIFI_READY:
      Serial.println("EVENT: Wifi suport ready");    
      break;

    default: 
      Serial.printf("EVENT: %d\n",event);    
      break;
      
 // STATION MODE ----------------------------------------------------------------------------------------------------
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
      //Serial.println("EVENT: STA Completed scan");    
      break;
    
    case ARDUINO_EVENT_WIFI_STA_START:
      Serial.println("EVENT: STA started");    
      break;
    
    case ARDUINO_EVENT_WIFI_STA_STOP:
      Serial.println("EVENT: STA stopped");    
      break;

    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.println("EVENT: STA Connected");
      break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("EVENT: STA Disconnected");    
      break;

    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
      Serial.println("EVENT: STA authentication mode changed");    
      break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("EVENT: STA Obtained IP: ");
      Serial.println(WiFi.localIP());
      break;

    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
      Serial.println("EVENT: STA Lost IP");    
      break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
      Serial.println("EVENT: STA IPv6 is preferred");    
      break;

 // WIFI PROTECTED SETUP (WPS) ENROLLE MODE ----------------------------------------------------------------------------------------------------
      
    case ARDUINO_EVENT_WPS_ER_SUCCESS:
      Serial.println("EVENT: WPS succeeded");    
      break;

    case ARDUINO_EVENT_WPS_ER_FAILED:
      Serial.println("EVENT: WPS failed");    
      break;

    case ARDUINO_EVENT_WPS_ER_TIMEOUT:
      Serial.println("EVENT: WPS timeout");    
      break;

    case ARDUINO_EVENT_WPS_ER_PIN:
      Serial.println("EVENT: WPS pin code");    
      break;

 // ACCES POINT MODE ----------------------------------------------------------------------------------------------------
      
    case ARDUINO_EVENT_WIFI_AP_START:
      Serial.println("EVENT: AP started");    
      break;

    case ARDUINO_EVENT_WIFI_AP_STOP:
      Serial.println("EVENT: AP stopped");    
      break;

    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
      Serial.println("EVENT: AP Client connected");    
      break;

    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
      Serial.println("EVENT: AP Client disconnected");    
      break;

    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
      Serial.println("EVENT: AP Assigned IP to client");    
      break;

    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
      Serial.println("EVENT: AP Received request");    
      break;

    case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
      Serial.println("EVENT: AP IPv6 is preferred");    
      break;
  }
}

void WiFi_manager()
{
  WiFi_stateMachine();
  AP_stateMachine();
  STA_stateMachine();

  if(WiFi.status() == WL_CONNECTED) is_wifi_connected = true;
  else is_wifi_connected = false;
}

void WiFi_stateMachine()
{
  switch (WifiState)
  {
    case WIFI_INIT:
      get_storage_credentials();                                                    // Busco credenciales guardadas
      WiFi.onEvent(WiFiEvent);                                                      // Configuro la callback de eventos
      if(!WiFi.mode(WIFI_AP_STA))                                                   // Pongo WiFi en modo Access Point y Estacion o cliente
      {
        Serial.println("ERROR: WiFi mode set failed");    
      }
      else
      {
        Serial.println("DEBUG: WiFi init");    
        is_wifi_init    = true;
        has_wifi_error  = false;
        WifiState       = WIFI_READY;
      }
      break; 
    
    case WIFI_READY:
      if(has_wifi_error && !is_wifi_AP_init && !is_wifi_STA_init) WifiState = WIFI_DEINIT;
      break;

    case WIFI_DEINIT:
      Serial.println("INFO: Wifi deinit");
      WiFi.mode(WIFI_MODE_NULL);
      is_wifi_init  = false;
      WifiState     = WIFI_INIT;
      break;
  }
}

void AP_stateMachine()
{
  switch (APState)
  {
    case WIFI_AP_INIT:
      if(is_wifi_init && !has_wifi_error)
      {
        if(WiFi.softAPConfig(local_ip_AP, gateway_AP, subnet_AP))
        {
          if(WiFi.softAP(SSID_AP, PASSWORD_AP, CHANNEL_AP, SSID_HIDDEN_AP, MAX_CONNECTION_AP, AP_CONF_FTM, AP_AUTH_MODE, AP_CIPHER_TYPE ))
          {
            Serial.println(WiFi.softAPIP());
            server.onNotFound( [](){ server.send(404, "text/plain", "Ruta no encontrada"); } );
            server.begin();
            Serial.println("DEBUG: AP init saccessfull");
            is_wifi_AP_init = true;
            APState         = WIFI_AP_READY;
          }
          else
          {
            Serial.println("ERROR: AP init failed");
            has_wifi_error  = true;
            APState         = WIFI_AP_DEINIT;
          }
        }
        else
        {
          Serial.println("ERROR: soft ap config failed");
          has_wifi_error  = true;
          APState         = WIFI_AP_DEINIT;
        }
      }
      break; 

    case WIFI_AP_READY:
      server.handleClient();
      if(has_wifi_error) APState = WIFI_AP_DEINIT;
      break;

    case WIFI_AP_DEINIT:
      Serial.println("INFO: AP deinit");
      server.close();
      WiFi.softAPdisconnect(false);
      is_wifi_AP_init = false;
      APState         = WIFI_AP_INIT;
      break; 
  }
}

void STA_stateMachine()
{
  bool STA_flag = false;
  switch (STAState)
  {
    case WIFI_STA_INIT:
      if(is_wifi_init && !has_wifi_error)
      {
        wifiMulti.setStrictMode(true);          // Se conecta o mantiene un coneccion únicamente con las credenciales de la lista WiFiMulti proporcionada.
        wifiMulti.setAllowOpenAP(false);        // No permite la coneccion con cualquier red wifi abierta.
        is_wifi_STA_init  = true;
        STAState          = WIFI_STA_READY;
        Serial.println("DEBUG: STA init");  
      }
      break;
    
    case WIFI_STA_READY:
      wifiMulti.run();
      if(has_wifi_error) STAState = WIFI_STA_DEINIT; 
      break;
    
    case WIFI_STA_DEINIT:
      Serial.println("DEBUG: STA deinit");  
      if(!WiFi.disconnect()) Serial.println("ERROR: STA disconnect failed");
      is_wifi_STA_init  = false;
      STAState          = WIFI_STA_INIT;
      break;  
  }
}

// GET FUNCTIONS ----------------------------------------------------------------------------------------------------

credentials_t* get_credentials() { return credentials; }

float getRSSI() {  return WiFi.RSSI(); }

bool getWifiStatus() { return is_wifi_connected; }

bool getWiFiError() { return has_wifi_error; }

String getSSID() { return WiFi.SSID(); }

// ----------------------------------------------------------------------------------------------------

