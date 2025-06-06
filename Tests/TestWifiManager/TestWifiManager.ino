#include <Arduino.h>
#include "../../src/Services/WifiManager.h"
#include "../../src/Services/WifiManager.cpp"
#include "../../src/Tools/Storage.cpp"
#include "../../src/Tools/TimerManager.cpp"

bool credential_added = false;
unsigned long last_status_check = 0;

void handleRoot()
{
  server.send(200, "text/plain", "Bienvenido al Access Point de CleanControl!\nConfiguracion de red disponible.");
}

void setup()
{
  Serial.begin(115200);

  // Test 1: Inicialización WiFi Manager
  Serial.println("\n\n\n\n\n\n\n\n\n\nIniciando WifiManager...");
  WifiManager();

  // Configurar la ruta HTTP Root para el Access Point
  Serial.println("Configurando ruta HTTP Root...");
  SetHdmiRoot("/", HTTP_GET, handleRoot);

  // Test 2: Recuperar credenciales almacenadas
  Serial.println("Test 2: Recuperar credenciales almacenadas");
  credentials_t* creds = GetCredentials();
  for (int i = 0; i < MAX_CREDENCIALES; i++) {
    Serial.printf("Credencial %d: SSID=%s, PASS=%s\n", i, creds[i].ssid, creds[i].password);
  }

  // Test 3: Agregar una nueva credencial
  Serial.println("Test 3: Agregar nueva credencial");
  credential_added = SetCredentials("Claro-hogar", "98769876");
  Serial.printf("Resultado agregar credencial: %s\n", credential_added ? "OK" : "FALLO");
}

void loop()
{
  WifiManager(); // Ejecuta las máquinas de estado continuamente

  unsigned long now = millis();
  if (now - last_status_check > 5000) { // Cada 5 segundos
    last_status_check = now;

    Serial.println("\n---- Estado de conexión WiFi ----");
    Serial.printf("Conectado a WiFi: %s\n", GetWifiStatus() ? "SI" : "NO");
    Serial.printf("Error en WiFi: %s\n", IsWifiError() ? "SI" : "NO");
    Serial.printf("SSID actual: %s\n", GetSsid().c_str());
    if (GetWifiStatus()) {
      Serial.printf("RSSI: %.1f dBm\n", GetRssi());
    }
  }
}
