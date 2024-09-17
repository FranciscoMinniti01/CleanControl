#include "wifi_manager.h"

//WebServerManager webServerManager; // Declarar el objeto del servidor web

WiFiManager::WiFiManager() {
    estadoActual = Estado_Inicializacion;
}

void WiFiManager::manejarWiFi() {
    switch (estadoActual) {
        case Estado_Inicializacion:
            iniciarWiFi();
            /*if (!cargarCredencialesGuardadas()) {
                estadoActual = Estado_Modo_AccessPoint;
            } else {
                estadoActual = Estado_IntentarConexionWiFi;
            }*/
            break;

        case Estado_IntentarConexionWiFi:
            // Intentar conectar a Wi-Fi usando las credenciales cargadas
            /*if (intentarConectar()) {
                estadoActual = Estado_MantenerConexionEstablecida;
            } else {
                estadoActual = Estado_Modo_AccessPoint; // Volver al modo AP si falla
            }
            break;*/

        case Estado_MantenerConexionEstablecida:
            mantenerConexion();
            break;

        case Estado_Intentar_Reconexion:
            intentarReconexion();
            break;

        case Estado_Modo_AccessPoint:
            iniciarAccessPoint();
            //estadoActual = Estado_EsperandoConexionAP;
            break;

        case Estado_GuardarCredenciales:
            // Procesar para guardar credenciales del portal cautivo
            guardarCredenciales("nuevo_ssid", "nueva_password");
            estadoActual = Estado_IntentarConexionWiFi;
            break;
    }
}

void WiFiManager::iniciarWiFi() {
    WiFi.mode(WIFI_STA);
}

void WiFiManager::intentarConexion() {
    // Intentar conectarse a WiFi con las credenciales
    for (int i = 0; i < MAX_CREDENCIALES; ++i) {
        WiFi.begin(ssids[i], passwords[i]);
        if (WiFi.waitForConnectResult() == WL_CONNECTED) {
            estadoActual = Estado_MantenerConexionEstablecida;
            return;
        }
    }
    estadoActual = Estado_Intentar_Reconexion;
}

void WiFiManager::mantenerConexion() {
    if (WiFi.status() != WL_CONNECTED) {
        estadoActual = Estado_Intentar_Reconexion;
    }
}

void WiFiManager::intentarReconexion() {
    if (WiFi.reconnect()) {
        estadoActual = Estado_MantenerConexionEstablecida;
    } else {
        estadoActual = Estado_Modo_AccessPoint;
    }
}

void WiFiManager::iniciarAccessPoint() {
    WiFi.softAP("ESP32_AP", "passwordAP");

        // Configurar el ESP32 en modo Access Point
    WiFi.softAP("ESP32_AccessPoint");

    // Iniciar el servidor web embebido y pasarle la callback para manejar las credenciales
    /*bool exito = webServerManager.iniciar([this](const char* ssid, const char* password) {
        Serial.println("Credenciales recibidas:");
        Serial.println(ssid);
        Serial.println(password);

        // Guardar las credenciales recibidas
        guardarCredenciales(ssid, password);
        
        // Cambiar al estado de intentar conectar
        estadoActual = Estado_IntentarConexionWiFi;
    });*/

    /*if (!exito) {
        Serial.println("Error al iniciar el portal cautivo.");
    }*/

    estadoActual = Estado_GuardarCredenciales;
}

void WiFiManager::guardarCredenciales(const char* ssid, const char* password) {
    // Guardar nuevas credenciales en NVS
    for (int i = 0; i < MAX_CREDENCIALES; ++i) {
        if (Utils::guardarDato(("ssid_" + String(i)).c_str(), ssids[i], sizeof(ssids[i])) &&
            Utils::guardarDato(("password_" + String(i)).c_str(), passwords[i], sizeof(passwords[i]))) {
            continue;
        }
    }
}

void WiFiManager::cargarCredencialesGuardadas() {
    // Cargar credenciales guardadas de NVS
    /*bool found = false;
    for (int i = 0; i < MAX_CREDENCIALES; ++i) {
        if (Utils::cargarDato(("ssid_" + String(i)).c_str(), ssids[i], sizeof(ssids[i])) &&
            Utils::cargarDato(("password_" + String(i)).c_str(), passwords[i], sizeof(passwords[i]))) {
            found = true;
        }
    }
    return found;*/
}
