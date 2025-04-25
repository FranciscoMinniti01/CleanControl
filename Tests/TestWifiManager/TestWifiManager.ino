#include <Arduino.h>
#include "../../src/Services/WifiManager.h"
#include "../../src/Services/WifiManager.cpp"
#include "../../src/Tools/Storage.cpp"

// Variables de control de test
bool credential_added = false;
unsigned long last_print = 0;

void handleRoot() {
  server.send(200, "text/plain", "Bienvenido al Access Point de CleanControl!\nConfiguracion de red disponible.");
}

void setup() {
  Serial.begin(115200);
  
  // Test 1: Inicialización WiFi Manager
  Serial.println("Iniciando WiFi Manager...");
  WiFi_manager();

  // Configurar una ruta para ver la web desde el AP
  Serial.println("Configurando ruta HTTP Root...");
  set_hdmi_root("/", HTTP_GET, handleRoot);

  // Test 2: Recuperar credenciales almacenadas (ANTES de intentar agregar nuevas)
  Serial.println("Test 2: Recuperar credenciales almacenadas");
  credentials_t* creds = get_credentials();
  for (int i = 0; i < MAX_CREDENCIALES; i++) {
    Serial.printf("Credencial %d: SSID=%s, PASS=%s\n", i, creds[i].ssid, creds[i].password);
  }

  // Test 3: Agregar nueva credencial
  Serial.println("Test 3: Agregar nueva credencial");
  credential_added = set_credentials("TestSSID", "TestPassword");
  Serial.printf("Resultado agregar credencial: %s\n", credential_added ? "OK" : "FALLO");
}

void loop() {
  WiFi_manager(); // Ejecuta las máquinas de estado continuamente

  unsigned long now = millis();
  if (now - last_print > 5000) { // Cada 5 segundos
    last_print = now;

    Serial.println("---- Estado WiFi ----");
    Serial.printf("WiFi conectado: %s\n", getWifiStatus() ? "SI" : "NO");
    Serial.printf("Error WiFi: %s\n", getWiFiError() ? "SI" : "NO");
    Serial.printf("SSID actual: %s\n", getSSID().c_str());
    if (getWifiStatus()) {
      Serial.printf("RSSI: %.1f dBm\n", getRSSI());
    }
  }
}
