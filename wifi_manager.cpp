#include "wifi_manager.h"

WiFiServer server(80); //Esto nose si dejarlo aca o moverlo

IPAddress local_ip_AP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
IPAddress gateway_AP(GATEWAY_IP_1, GATEWAY_IP_2, GATEWAY_IP_3, GATEWAY_IP_4);
IPAddress subnet_AP(SUBNET_IP_1, SUBNET_IP_2, SUBNET_IP_3, SUBNET_IP_4);

void WiFiManager_c::WiFiManager()
{
    WifiState = WIFI_CREDENTIALS_CHECK;

    WiFi.onEvent( [this](WiFiEvent_t event, WiFiEventInfo_t info){
                      this->WiFiEventCB(event, info);
                      #if !defined(DEBUG) && !defined(DEBUG_WIFI)
                      Serial.printf("[WiFi-event] event: %d\n", event);
                      #endif
                  });

    for (int i = 0; i < MAX_CREDENCIALES; i++) {
        ssids[i][0] = '\0';
        passwords[i][0] = '\0';
    }
}

void WiFiManager_c::WiFiEventCB(WiFiEvent_t event, WiFiEventInfo_t info)
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
            Serial.println("EVENT: AP STARTED");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            Serial.println("EVENT: AP STOPPED");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.println("EVENT: AP Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.println("EVENT: AP Client disconnected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            Serial.println("EVENT: AP Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            Serial.println("EVENT: AP Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            Serial.println("EVENT: AP IPv6 is preferred");
            break;

        default: break;
    }
}

void WiFiManager_c::WiFiStateMachine()
{
    switch (WifiState)
    {
        case WIFI_CREDENTIALS_CHECK:
            Serial.println("\n\n\n\n\n WIFI_CREDENTIALS_CHECK");

            if(!nsv_PrepareSpace(WIFI_NAME_SPACE))
            {
                Serial.println("FALLO HABRIR EL ESPACIO");
            }

            if(GetCredentials()){
                WifiState = WIFI_STA_INIT;
                //nsv_CloseSpace();
            }else WifiState = WIFI_AP_INIT;
            break;

        case WIFI_AP_INIT:
            Serial.println("WIFI_AP_INIT");

            if(!WiFi.mode(WIFI_AP)) Serial.println("WiFi mode set failed");
            if(!WiFi.softAPConfig(local_ip_AP, gateway_AP, subnet_AP)) Serial.println("soft ap config failed");
            //WiFi.softAP(SSID_AP, PASSWORD_AP, CHANNEL_AP, SSID_HIDDEN_AP, MAX_CONNECTION_AP)
            if(!WiFi.softAP("CCAP", NULL)) Serial.println("AP init failed");
            Serial.println(WiFi.softAPIP());
            WifiState = WIFI_AP_SERVER_INIT;
            break;

        case WIFI_AP_WAIT_INIT:
            break;

        case WIFI_AP_SERVER_INIT:
            Serial.println("WIFI_AP_SERVER_INIT");
            server.begin();
            WifiState = WIFI_AP_WAIT_CONNECTION;
            break;
          
        case WIFI_AP_WAIT_CONNECTION:
            ServerManager();
            break;

        case WIFI_STA_INIT:
            Serial.println("FUNCIONO");
            for (int i = 0; i < 3; i++) {
                String ssidString = String(ssids[i]);
                String passwordString = String(passwords[i]);
                Serial.println("SSID " + String(i) + ": " + ssidString);
                Serial.println("Password " + String(i) + ": " + passwordString);
            }
            nsv_CloseSpace();
            WifiState = WIFI_STA_INIT_;
            /*WiFi.softAPdisconnect(false);
            WiFi.mode(WIFI_STA);
            WiFi.setAutoReconnect(true);*/
            break;

        case WIFI_STA_INIT_:
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

    // Leer datos del cliente
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        header += c;

        // Fin de la solicitud HTTP
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
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
      
      Serial.print("DEBUG: se ingreso una credencial: ");
      Serial.println(numeroVariable+1);
      numeroVariable++;
    }

    // Cerrar la conexión con el cliente
    client.stop();
    Serial.println("DEBUG: Cliente stop");
  }

  if(numeroVariable > 2)
  {
      Serial.println("DEBUG: Se llego a las 3 credenciales");
      WifiState = WIFI_CREDENTIALS_CHECK;
      SaveCredentials();
  }

}

bool WiFiManager_c::GetCredentials(){
    bool err = false;

    /*if(!nsv_PrepareSpace(WIFI_NAME_SPACE))
    {
        return err;
    }*/
    Serial.println("DEBUG: GetCredentials");

    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        ssids[i][0]     = '\0';
        passwords[i][0] = '\0';
    }

    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        if(!nsv_GetData( (std::string(WIFI_KEY_SSID) + std::to_string(i)).c_str() , ssids[i], sizeof(ssids[i])))
        {
            Serial.print("DEBUG: No se obtubo ssid:");
            Serial.println(i);
            break;
        }
        if(!nsv_GetData( (std::string(WIFI_KEY_PASW) + std::to_string(i)).c_str() , passwords[i], sizeof(passwords[i])))
        {
            Serial.print("DEBUG: No se obtubo password:");
            Serial.println(i);
            break;
        }
        Serial.println("DEBUG: se obtuvo almenos una credencial");
        err = true;
    }

    //nsv_CloseSpace();
    return err;
}

bool WiFiManager_c::SaveCredentials()
{
    bool err = false;

    /*if(!nsv_PrepareSpace(WIFI_NAME_SPACE))
    {
        return err;
    }*/

    Serial.println("DEBUG: SaveCredentials");

    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        if(ssids[i][0] != '\0' && passwords[i][0] != '\0')
        {
            if(!nsv_PutData( (std::string(WIFI_KEY_SSID) + std::to_string(i)).c_str() , static_cast<void*>(ssids[i]) , sizeof(ssids[i])))
            {
                Serial.print("DEBUG: No se guaardo ssid:");
                Serial.println(i);
                break;
            }
            if(!nsv_PutData( (std::string(WIFI_KEY_PASW) + std::to_string(i)).c_str() , static_cast<void*>(passwords[i]) , sizeof(passwords[i])))
            {
                Serial.print("DEBUG: No se guaardo password:");
                Serial.println(i);
                break;
            }
            Serial.println("DEBUG: se guardo almenos una credencial");
            err = true;
        } else Serial.println("DEBUG: no se guardo ninguna credencial");
    }

    //nsv_CloseSpace();
    return err;
}


