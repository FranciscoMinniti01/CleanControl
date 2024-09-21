#include "wifi_manager.h"

WiFiServer server(80); //Esto nose si dejarlo aca o moverlo

IPAddress local_ip_AP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
IPAddress gateway_AP(GATEWAY_IP_1, GATEWAY_IP_2, GATEWAY_IP_3, GATEWAY_IP_4);
IPAddress subnet_AP(SUBNET_IP_1, SUBNET_IP_2, SUBNET_IP_3, SUBNET_IP_4);

void WiFiManager_c::WiFiManager(){
    WifiState = WIFI_CREDENTIALS_CHECK;
    WiFi.onEvent( [this](WiFiEvent_t event, WiFiEventInfo_t info){
                      this->WiFiEventCB(event, info); // Llamamos al método no estático usando 'this' y pasando los parámetros
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

void WiFiManager_c::WiFiStateMachine()
{
    switch (WifiState)
    {
        case WIFI_CREDENTIALS_CHECK:
            Serial.println("WIFI_CREDENTIALS_CHECK");
            if(GetCredentials()) WifiState = WIFI_STA_INIT;
            else WifiState = WIFI_AP_INIT;
            break;

        case WIFI_AP_INIT:
            Serial.println("WIFI_AP_INIT");
            WiFi.mode(WIFI_AP_STA);
            WiFi.softAPConfig(local_ip_AP, gateway_AP, subnet_AP);
            WiFi.softAP(SSID_AP, PASSWORD_AP, CHANNEL_AP, SSID_HIDDEN_AP, MAX_CONNECTION_AP);
            Serial.println(WiFi.softAPIP());
            WifiState = WIFI_AP_WAIT_INIT;
            break;

        case WIFI_AP_WAIT_INIT:
            break;

        case WIFI_AP_SERVER_INIT:
            Serial.println("WIFI_AP_SERVER_INIT");
            server.begin();
            break;
          
        case WIFI_AP_WAIT_CONNECTION:
            ServerManager();
            break;

        case WIFI_STA_INIT:
            WiFi.softAPdisconnect(false);
            WiFi.mode(WIFI_STA);
            WiFi.setAutoReconnect(true);
            break;
    }
}

void WiFiManager_c::ServerManager(){
  static int numeroVariable = 1;
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Nuevo cliente conectado.");
    String currentLine = "";
    String header = "";  // Almacenará la solicitud HTTP

    // Leer datos del cliente
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;

        // Fin de la solicitud HTTP
        if (c == '\n') {
          if (currentLine.length() == 0) {
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
            client.println("<p>Valor actual del número: " + String(numeroVariable) + "</p>");
            client.println("</body></html>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    // Procesar los datos recibidos (extraer nombre y contraseña)
    if (header.indexOf("GET /?nombre=") >= 0) {
      int nombreStart = header.indexOf("nombre=") + 7;
      int nombreEnd = header.indexOf("&", nombreStart);
      int contrasenaStart = header.indexOf("contrasena=") + 11;

      header.substring(nombreStart, nombreEnd).toCharArray(ssids[numeroVariable], MAX_LEN_CREDENCIALES);
      header.substring(contrasenaStart, header.indexOf(" ", contrasenaStart)).toCharArray(passwords[numeroVariable], MAX_LEN_CREDENCIALES);

      Serial.println("Nombre recibido: " + String(ssids[numeroVariable]));
      Serial.println("Contraseña recibida: " + String(passwords[numeroVariable]));
    }

    // Cerrar la conexión con el cliente
    client.stop();
    Serial.println("Cliente desconectado.");
  }
}

bool WiFiManager_c::GetCredentials(){
    bool err = false;

    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        ssids[i][0]     = '\0';
        passwords[i][0] = '\0';
    }

    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        if(!GetData(WIFI_NAME_SPACE, (std::string(WIFI_KEY_SSID) + std::to_string(i)).c_str() , ssids[i], sizeof(ssids[i])))
        {
            break;
        }
        if(!GetData(WIFI_NAME_SPACE, (std::string(WIFI_KEY_PASW) + std::to_string(i)).c_str() , passwords[i], sizeof(passwords[i])))
        {
            break;
        }
        err = true;
    }
    return err;
}

bool WiFiManager_c::SaveCredentials(const char* ssid, const char* password){
    bool err = false;
    for (int i = 0; i < MAX_CREDENCIALES; ++i)
    {
        if(ssids[i][0] != '\0')
        {
            if(!PutData(WIFI_NAME_SPACE, (std::string(WIFI_KEY_SSID) + std::to_string(i)).c_str() , ssids[i], sizeof(ssids[i])))
            {
                break;
            }
        }
        if(passwords[i][0] != '\0')
        {
            if(!PutData(WIFI_NAME_SPACE, (std::string(WIFI_KEY_PASW) + std::to_string(i)).c_str() , passwords[i], sizeof(passwords[i])))
            {
                break;
            }
        }
        err = true;
    }
    return err;
}


