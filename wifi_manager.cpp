// INCLUDES ----------------------------------------------------------------------------------------------------

#include "wifi_manager.h"

// VARIABLES SERVER ----------------------------------------------------------------------------------------------------

WebServer server(80);
hdmi_root_t hdmi_root[MAX_HDMI_ROOT];

// VARIABLES CREDENTIALS ----------------------------------------------------------------------------------------------------

credentials_t  credentials[MAX_CREDENCIALES];
storage_t credentials_storage[MAX_CREDENCIALES][2];

// VARIABLES WIFI ----------------------------------------------------------------------------------------------------

WiFiMulti wifiMulti; 

IPAddress local_ip_AP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
IPAddress gateway_AP(GATEWAY_IP_1, GATEWAY_IP_2, GATEWAY_IP_3, GATEWAY_IP_4);
IPAddress subnet_AP(SUBNET_IP_1, SUBNET_IP_2, SUBNET_IP_3, SUBNET_IP_4);

static State_Wifi_t  WifiState  = WIFI_INIT;
static State_AP_t    APState    = WIFI_AP_INIT;
static State_STA_t   STAState   = WIFI_STA_INIT;

static bool wifi_error          = false;            // Inidca si hay un error en las maquinas de estado, si esta en true todas las maquinas se van a deinit
static bool wifi_is_connected   = false;            // Indica si el dispositivo esta conectado como cliente a una red wifi 

static bool wifi_init           = false;            // Indica que se inicializo la libreria wifi

static bool wifi_AP_init        = false;            // Indica que se inicializo el modo AP
static bool wifi_server_init    = false;            // Indica que se inicializo el servidor

static bool wifi_STA_init       = false;            // Indica que se inicializo el modo STA
static bool wifi_STA_new_credentials = false;       // Indica que hay nuevas credenciales, se borraran todas y se armara nuevamente la lista

// FUNCTIONS SERVER ----------------------------------------------------------------------------------------------------

bool set_hdmi_root(String root_ , HTTPMethod request_ , handle_fun fun_)
{
  for(uint8_t i = 0; i<MAX_HDMI_ROOT; i++)
  {
    if(hdmi_root[i].fun == NULL)
    {
      hdmi_root[i].root     = root_;
      hdmi_root[i].request  = request_;
      hdmi_root[i].fun      = fun_;
      return true;
    }
  }
  return false;
}

// FUNCTIONS CREDENTIALS ----------------------------------------------------------------------------------------------------

void get_storage_credentials()
{
  for(uint8_t i = 0; i<MAX_CREDENCIALES; i++)
  {
    credentials_storage[i][0].data   = (void*)credentials[i].ssid;
    credentials_storage[i][0].len    = MAX_LEN_CREDENCIALES;
    credentials_storage[i][0].key    = KEY_CREDENTIAL_SSID + String(i);

    get_data(&credentials_storage[i][0]);

    credentials_storage[i][1].data   = (void*)credentials[i].password;
    credentials_storage[i][1].len    = MAX_LEN_CREDENCIALES;
    credentials_storage[i][1].key    = KEY_CREDENTIAL_PASSWORD + String(i);

    get_data(&credentials_storage[i][1]);
    
    if(credentials[i].ssid[0] != 0)
    {
      #ifdef DEBUG_WIFI
      Serial.println("INFO: Se obtuvieron credenciales");
      #endif
    }
  }
}

bool set_credentials(String ssid, String password)
{
  bool flag     = false;
  uint8_t index = 0;
  static uint8_t last_index = 0;

  #ifdef DEBUG_WIFI
  Serial.printf("INF: Set ssid=%s\n",ssid);
  Serial.printf("INF: Set password=%s\n",password);
  #endif

  for(index = 0; index<MAX_CREDENCIALES; index++)
  {
    if(credentials[index].ssid[0] == 0)
    {
      flag = true;
      break;
    }
  }

  if(!flag) index = (++last_index)%MAX_CREDENCIALES;

  Serial.printf("INF: Set index = %d\n",index);

  strncpy(credentials[index].ssid, ssid.c_str(), MAX_LEN_CREDENCIALES);
  credentials[index].ssid[MAX_LEN_CREDENCIALES-1] = '\0';

  strncpy(credentials[index].password, password.c_str(), MAX_LEN_CREDENCIALES);
  credentials[index].password[MAX_LEN_CREDENCIALES-1] = '\0';

  if(!seve_data(&credentials_storage[index][0])) return false;
  if(!seve_data(&credentials_storage[index][1])) return false;

  wifi_STA_new_credentials = true;

  return true;
}

// FUNCTIONS WIFI ----------------------------------------------------------------------------------------------------

void WiFiEvent(WiFiEvent_t event)
{  
  switch (event)
  {
 // GENERAL ----------------------------------------------------------------------------------------------------
    case ARDUINO_EVENT_WIFI_READY:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: Wifi suport ready");    
      #endif
      break;

    default: 
      #ifdef DEBUG_WIFI
      Serial.printf("EVENT: %d\n",event);    
      #endif  
      break;
      
 // STATION MODE ----------------------------------------------------------------------------------------------------
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
      #ifdef DEBUG_WIFI
      //Serial.println("EVENT: STA Completed scan");    
      #endif
      break;
    
    case ARDUINO_EVENT_WIFI_STA_START:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: STA started");    
      #endif
      break;
    
    case ARDUINO_EVENT_WIFI_STA_STOP:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: STA stopped");    
      #endif
      break;

    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: STA Connected");    
      #endif
      break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: STA Disconnected");    
      #endif
      break;

    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: STA authentication mode changed");    
      #endif
      break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      #ifdef DEBUG_WIFI
      Serial.print("EVENT: STA Obtained IP: ");
      Serial.println(WiFi.localIP());
      #endif
      break;

    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: STA Lost IP");    
      #endif
      break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: STA IPv6 is preferred");    
      #endif
      break;

 // WIFI PROTECTED SETUP (WPS) ENROLLE MODE ----------------------------------------------------------------------------------------------------
      
    case ARDUINO_EVENT_WPS_ER_SUCCESS:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: WPS succeeded");    
      #endif
      break;

    case ARDUINO_EVENT_WPS_ER_FAILED:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: WPS failed");    
      #endif
      break;

    case ARDUINO_EVENT_WPS_ER_TIMEOUT:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: WPS timeout");    
      #endif
      break;

    case ARDUINO_EVENT_WPS_ER_PIN:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: WPS pin code");    
      #endif
      break;

 // ACCES POINT MODE ----------------------------------------------------------------------------------------------------
      
    case ARDUINO_EVENT_WIFI_AP_START:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: AP started");    
      #endif
      break;

    case ARDUINO_EVENT_WIFI_AP_STOP:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: AP stopped");    
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
      Serial.println("EVENT: AP Assigned IP to client");    
      #endif
      break;

    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: AP Received request");    
      #endif
      break;

    case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
      #ifdef DEBUG_WIFI
      Serial.println("EVENT: AP IPv6 is preferred");    
      #endif
      break;
  }
}

void WiFi_manager()
{
  WiFi_stateMachine();
  AP_stateMachine();
  STA_stateMachine();

  if(WiFi.status() == WL_CONNECTED) wifi_is_connected = true;
  else wifi_is_connected = false;
}

void WiFi_stateMachine()
{
  switch (WifiState)
  {
    case WIFI_INIT:
      get_storage_credentials();                                                    // Busco credenciales guardadas
      WiFi.onEvent(WiFiEvent);                                                      // Configuro la callback de eventos
      if(!WiFi.mode(WIFI_AP_STA)) Serial.println("ERROR: WiFi mode set failed");    // Pongo WiFi en modo Access Point y Estacion o cliente
      wifi_init = true;
      wifi_error = false;
      WifiState = WIFI_READY;
      break; 
    
    case WIFI_READY:
      if(wifi_error && !wifi_AP_init && !wifi_STA_init) WifiState = WIFI_DEINIT;
      break;

    case WIFI_DEINIT:
      Serial.println("INFO: WIFI DEINIT");
      WiFi.mode(WIFI_MODE_NULL);
      wifi_init = false;
      WifiState = WIFI_INIT;
      break;
  }
}

void AP_stateMachine()
{
  bool AP_flag = false;
  switch (APState)
  {
    case WIFI_AP_INIT:
      if(wifi_init)
      {
        if(WiFi.softAPConfig(local_ip_AP, gateway_AP, subnet_AP))
        {
          if(WiFi.softAP(SSID_AP, PASSWORD_AP, CHANNEL_AP, SSID_HIDDEN_AP, MAX_CONNECTION_AP, false))
          {
            AP_flag = true;
            Serial.println(WiFi.softAPIP());
          }
          else Serial.println("ERROR: AP init failed");
        }
        else Serial.println("ERROR: soft ap config failed");
        
        if(!AP_flag)
        {
          APState     = WIFI_AP_DEINIT;
          wifi_error  = !AP_flag;
        }
        else
        {
          wifi_AP_init  = true;
          APState       = WIFI_AP_SERVER_INIT;
        }
      }
      break; 
        
    case WIFI_AP_SERVER_INIT:
      for(uint8_t i = 0 ; i<MAX_HDMI_ROOT ; i++)
      {
        if(hdmi_root[i].fun != NULL )
        {
          server.on(hdmi_root[i].root, hdmi_root[i].request, hdmi_root[i].fun);
          AP_flag = true;
        }
      }
      if(AP_flag)
      {
        server.begin();
        wifi_server_init = true;
      } 
      APState = WIFI_AP_READY;
      break;

    case WIFI_AP_READY:
      if(wifi_server_init) server.handleClient();
      if(wifi_error) APState = WIFI_AP_SERVER_DEINIT;
      break;

    case WIFI_AP_SERVER_DEINIT:
      server.close();
      server.stop();
      wifi_server_init  = false;
      APState           = WIFI_AP_DEINIT;
      break; 

    case WIFI_AP_DEINIT:
      WiFi.softAPdisconnect(false);
      wifi_AP_init  = false;
      APState       = WIFI_AP_INIT;
      break; 
  }
}

void STA_stateMachine()
{
  bool STA_flag = false;
  switch (STAState)
  {
    case WIFI_STA_INIT:
      if(wifi_init)
      {
        wifiMulti.setStrictMode(true);          // Se conecta o mantiene un coneccion únicamente con las credenciales de la lista WiFiMulti proporcionada.
        wifiMulti.setAllowOpenAP(false);        // No permite la coneccion con cualquier red wifi abierta.
        STAState = WIFI_STA_CLEAR_CREDENTIALS;
      }
      break;

    case WIFI_STA_CLEAR_CREDENTIALS:
      wifiMulti.APlistClean();
      STAState = WIFI_STA_ADD_CREDENTIALS;
      break;

    case WIFI_STA_ADD_CREDENTIALS:
      for (int i = 0; i < MAX_CREDENCIALES; i++)
      {
        if (credentials[i].ssid[0] != 0)
        {
          Serial.printf("INF: Add credential %d\n",i);
          if (credentials[i].password[0] == 0)  Serial.printf("INFO: password %d empty\n",i);
          if(!wifiMulti.addAP(credentials[i].ssid,credentials[i].password))
          {
            Serial.printf("ERROR: add credential %d failed\n",i);
            wifi_error = true;
          }
          wifi_STA_init = true;
        }
      }
      STAState = WIFI_STA_READY;
      wifi_STA_new_credentials = false;
      break;
    
    case WIFI_STA_READY:
      if(wifi_STA_init) wifiMulti.run();
      if(wifi_STA_new_credentials) STAState = WIFI_STA_CLEAR_CREDENTIALS;
      if(wifi_error) STAState = WIFI_STA_DEINIT; 
      break;
    
    case WIFI_STA_DEINIT:
      wifiMulti.APlistClean();
      if(!WiFi.disconnect()) Serial.println("ERROR: STA disconnect failed");
      wifi_STA_init = false;
      STAState = WIFI_STA_INIT;
      break;  
  }
}

// FUNCTIONS GET ----------------------------------------------------------------------------------------------------

credentials_t* get_credentials() { return credentials; }

float getRSSI() { return WiFi.RSSI(); }

bool getWifiStatus() { return wifi_is_connected; }

bool getWiFiError() { return wifi_error; }

String getSSID() { return WiFi.SSID(); }

// ----------------------------------------------------------------------------------------------------

