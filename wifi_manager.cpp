#include "wifi_manager.h"

WiFiServer server(80); //Esto nose si dejarlo aca o moverlo

IPAddress local_ip_AP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
IPAddress gateway_AP(GATEWAY_IP_1, GATEWAY_IP_2, GATEWAY_IP_3, GATEWAY_IP_4);
IPAddress subnet_AP(SUBNET_IP_1, SUBNET_IP_2, SUBNET_IP_3, SUBNET_IP_4);

Preferences preferences;

WiFiManager_c::WiFiManager_c()
{
    WifiState = WIFI_CREDENTIALS_CHECK;

    WiFi.onEvent( [this](WiFiEvent_t event, WiFiEventInfo_t info){
                      this->WiFiEventCB(event, info);
                  });

    for (int i = 0; i < MAX_CREDENCIALES; i++) {
        ssids[i][0] = '\0';
        passwords[i][0] = '\0';
    }
}

void WiFiManager_c::WiFiEventCB(WiFiEvent_t event, WiFiEventInfo_t info)
{
    switch (event)
    {
// GENERAL ----------------------------------------------------------------------------------------------------
        case ARDUINO_EVENT_WIFI_READY:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: WIFI_READY");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: Completed scan for access points");    
            #endif
            break;
// STATION MODE ----------------------------------------------------------------------------------------------------
        case ARDUINO_EVENT_WIFI_STA_START:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: WiFi client started");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: WiFi clients stopped");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: Connected to access point");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: Disconnected from WiFi access point");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: Authentication mode of access point has changed");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.print("EVENT: Obtained IP address: ");
            Serial.println(WiFi.localIP());
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: Lost IP address and IP address is reset to 0");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: STA IPv6 is preferred");    
            #endif
            break;

// ERRORES CREO ----------------------------------------------------------------------------------------------------
        
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: WiFi Protected Setup (WPS): succeeded in enrollee mode");    
            #endif
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: WiFi Protected Setup (WPS): failed in enrollee mode");    
            #endif
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: WiFi Protected Setup (WPS): timeout in enrollee mode");    
            #endif
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: WiFi Protected Setup (WPS): pin code in enrollee mode");    
            #endif
            break;

// ACCES POINT MODE ----------------------------------------------------------------------------------------------------
        
        case ARDUINO_EVENT_WIFI_AP_START:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: AP STARTED");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: AP STOPPED");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: AP Client connected");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: AP Client disconnected");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: AP Assigned IP address to client");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: AP Received probe request");    
            #endif
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("EVENT: AP IPv6 is preferred");    
            #endif
            break;
        default: break;
    }
}

void WiFiManager_c::WiFiStateMachine()
{
    switch (WifiState)
    {
        case WIFI_CREDENTIALS_CHECK:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("STATE: WIFI_CREDENTIALS_CHECK");    
            #endif

            if(GetCredentials()){
                WifiState = WIFI_STA_INIT;
            }else {
                WifiState = WIFI_AP_INIT;
            }

            break;

        case WIFI_AP_INIT:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("STATE: WIFI_AP_INIT");    
            #endif

            if(!WiFi.disconnect()) Serial.println("ERROR: STA disconnect failed");
            if(!WiFi.mode(WIFI_AP)) Serial.println("ERROR: WiFi mode set failed");
            if(!WiFi.softAPConfig(local_ip_AP, gateway_AP, subnet_AP)) Serial.println("ERROR: soft ap config failed");
            //if(!WiFi.softAP(SSID_AP, PASSWORD_AP, CHANNEL_AP, SSID_HIDDEN_AP, MAX_CONNECTION_AP)) Serial.println("ERROR: AP init failed");
            if(!WiFi.softAP(SSID_AP, NULL)) Serial.println("ERROR: AP init failed");
            Serial.print("IP address: ");
            Serial.println(WiFi.softAPIP());

            WifiState = WIFI_AP_SERVER_INIT;
            break;

        case WIFI_AP_SERVER_INIT:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("STATE: WIFI_AP_SERVER_INIT");    
            #endif

            server.begin();

            WifiState = WIFI_AP_WAIT_CONNECTION;
            break;
          
        case WIFI_AP_WAIT_CONNECTION:
            ServerManager();
            break;

        case WIFI_STA_INIT:
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("STATE: WIFI_STA_INIT");    
            #endif

            if(!WiFi.softAPdisconnect(false)) Serial.println("ERROR: AP disconnect failed");
            WiFi.mode(WIFI_STA);
            WiFi.setAutoReconnect(true);

            for (int i = 0; i < MAX_CREDENCIALES; i++) {
                if(ssids[i][0] != '\0' && passwords[i][0] != '\0'){
                    if(!wifiMulti.addAP(ssids[i],passwords[i])) Serial.printf("ERROR: add credential %d failed\n",i);
                }
            }

            WifiState = WIFI_STA_CONNECTING;
            break;

        case WIFI_STA_CONNECTING:
            if(wifiMulti.run() == WL_CONNECTED){
                Serial.println("WIFI CONNECTED");
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP());
                WifiState = WIFI_STA_READY;
                isconnected = true;
            }
            /*if(WiFi.isConnected()){}*/
            break;

        case WIFI_STA_READY:
            if(wifiMulti.run() != WL_CONNECTED){
                Serial.println("WIFI DISCONNECTED");
                Serial.println("WIFI RECONNECTING .....");
                WifiState = WIFI_STA_CONNECTING;
                isconnected = false;
            }
            /*if(!WiFi.isConnected()){}*/
            break;
    }
}

void WiFiManager_c::ServerManager()
{
  static int numeroVariable = 0;
  WiFiClient client = server.available();

  if (client)
  {
    String currentLine = "";
    String header = "";  // Almacenará la solicitud HTTP

    if(client.connected())
    {
      #if !defined(DEBUG) && !defined(DEBUG_WIFI)
      Serial.println("DEBUG: Cliente connected");   
      #endif
    }
    // Leer datos del cliente
    while (client.connected())
    {
      if (client.available())
      {
        #if !defined(DEBUG) && !defined(DEBUG_WIFI)
        Serial.println("DEBUG: Client available");    
        #endif

        char c = client.read();
        header += c;

        // Detecta el fin de la solicitud HTTP
        if (c == '\n')
        {
          #if !defined(DEBUG) && !defined(DEBUG_WIFI)
          Serial.println("DEBUG: fin de la solicitud HTTP");    
          #endif

          if (currentLine.length() == 0)
          {
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.println("DEBUG: currentLine length = 0");    
            #endif

            // Enviar la página web al cliente
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<body>");
            client.println("<h1>Ingresa tus datos</h1>");
            client.println("<form method='GET' action='/'>");
            client.println("Nombre: <input type='text' name='nombre'><br>");
            client.println("Contraseña: <input type='password' name='contrasena'><br>");
            client.println("<input type='submit' value='Aceptar'>");
            client.println("</form>");
            client.println("<p>Valor actual del número: " + String(numeroVariable+1) + "</p>");
            client.println("</body></html>");

            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        } 
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }

    // Procesar los datos recibidos (extraer nombre y contraseña)
    if (header.indexOf("GET /?nombre=") >= 0)
    {
      int nombreStart = header.indexOf("nombre=") + 7;
      int nombreEnd = header.indexOf("&", nombreStart);
      int contrasenaStart = header.indexOf("contrasena=") + 11;

      header.substring(nombreStart, nombreEnd).toCharArray(ssids[numeroVariable], MAX_LEN_CREDENCIALES);
      header.substring(contrasenaStart, header.indexOf(" ", contrasenaStart)).toCharArray(passwords[numeroVariable], MAX_LEN_CREDENCIALES);

      Serial.println("Nombre recibido: " + String(ssids[numeroVariable]));
      Serial.println("Contraseña recibida: " + String(passwords[numeroVariable]));
      
      Serial.print("Se ingreso la credencial: ");
      Serial.println(numeroVariable+1);
      numeroVariable++;
    }

    // Cerrar la conexión con el cliente
    client.stop();
    #if !defined(DEBUG) && !defined(DEBUG_WIFI)
    Serial.println("DEBUG: Cliente stop");   
    #endif
  }

  if(numeroVariable > 2)
  {
      #if !defined(DEBUG) && !defined(DEBUG_WIFI)
      Serial.println("DEBUG: Se llego a las 3 credenciales");
      #endif
      WifiState = WIFI_CREDENTIALS_CHECK;
      SaveCredentials();
  }

}

bool WiFiManager_c::GetCredentials()
{
    bool err = false;
    if( !preferences.begin(WIFI_NAME_SPACE) ) Serial.println("ERROR: Preferences begin failed");

    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        ssids[i][0]     = '\0';
        passwords[i][0] = '\0';
    }

    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        if( !preferences.getBytes( (std::string(WIFI_KEY_SSID) + std::to_string(i)).c_str() , ssids[i], sizeof(ssids[i])) )
        {
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.print("DEBUG: No se obtuvo ssid:");
            Serial.println(i);    
            #endif
            break;
        }
        if( !preferences.getBytes( (std::string(WIFI_KEY_PASW) + std::to_string(i)).c_str() , passwords[i], sizeof(passwords[i])))
        {
            #if !defined(DEBUG) && !defined(DEBUG_WIFI)
            Serial.print("DEBUG: No se obtuvo password:");
            Serial.println(i);  
            #endif
            break;
        }
        Serial.println("Se obtuvo credenciales");
        err = true;
    }

    preferences.end();
    return err;
}

bool WiFiManager_c::SaveCredentials()
{
    bool err = false;
    if( !preferences.begin(WIFI_NAME_SPACE) ) Serial.println("ERROR: Preferences begin failed");

    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        if(ssids[i][0] != '\0' && passwords[i][0] != '\0')
        {
            if(!preferences.putBytes( (std::string(WIFI_KEY_SSID) + std::to_string(i)).c_str() , static_cast<void*>(ssids[i]) , sizeof(ssids[i])))
            {
                #if !defined(DEBUG) && !defined(DEBUG_WIFI)
                Serial.print("DEBUG: No se guardo ssid:");
                Serial.println(i);
                #endif
                break;
            }
            if(!preferences.putBytes( (std::string(WIFI_KEY_PASW) + std::to_string(i)).c_str() , static_cast<void*>(passwords[i]) , sizeof(passwords[i])))
            {
                #if !defined(DEBUG) && !defined(DEBUG_WIFI)
                Serial.print("DEBUG: No se guardo password:");
                Serial.println(i);
                #endif
                break;
            }
            Serial.println("Se guardo credenciales");
            err = true;
        }
    }

    preferences.end();
    return err;
}

float WiFiManager_c::getRSSI()
{
    return WiFi.RSSI();
}

bool WiFiManager_c::getWifiStatus()
{
    return isconnected;
}

const char* WiFiManager_c::getSSID()
{
    strncpy(ssid, WiFi.SSID().c_str(), sizeof(ssid) - 1);  // Copia segura, dejando espacio para el terminador nulo
    ssid[sizeof(ssid) - 1] = '\0';  // Asegura que el array esté terminado en null
    return ssid;
}
