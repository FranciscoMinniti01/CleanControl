#include <Arduino.h>
#include "../../src/Services/WifiManager.h"
#include "../../src/Services/WifiManager.cpp"
#include "../../src/App/ServerManager.cpp"
#include "../../src/App/ServerManager.h"
#include "../../src/Tools/Storage.cpp"
#include "../../src/Tools/TimerManager.cpp"

bool credential_added = false;
unsigned long last_status_check = 0;

void handleRootInfo()
{
  server.send(200, "text/plain", "Access Point de CleanControl: Configuración disponible en /");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("\n\n\n\n\n\n\n\n\n\n INICIO DE TEST ");

  // Inicializa el módulo WiFi
  Serial.println("Iniciando WifiManager...");
  WifiManager();

  // Configura las rutas HTTP del servidor
  Serial.println("Iniciando ServerManager...");
  ServerManagerInit();

  // Recuperar credenciales almacenadas
  Serial.println("Test 1: Recuperar credenciales almacenadas");
  credentials_t* creds = GetCredentials();
  for (int i = 0; i < MAX_CREDENCIALES; i++) {
    Serial.printf("Credencial %d: SSID=%s, PASS=%s\n", i, creds[i].ssid, creds[i].password);
  }

  // Agregar una nueva credencial
  Serial.println("Test 2: Agregar nueva credencial");
  credential_added = SetCredentials("TestSSID", "TestPassword");
  Serial.printf("Resultado agregar credencial: %s\n", credential_added ? "OK" : "FALLO");
}

void loop()
{
  WifiManager();

  unsigned long now = millis();
  if (now - last_status_check > 5000)
  {
    last_status_check = now;

    Serial.println("---- Estado de conexión WiFi ----");
    Serial.printf("Conectado a WiFi: %s\n", GetWifiStatus() ? "SI" : "NO");
    Serial.printf("Error en WiFi: %s\n", IsWifiError() ? "SI" : "NO");
    Serial.printf("SSID actual: %s\n", GetSsid().c_str());
    if (GetWifiStatus()) {
      Serial.printf("RSSI: %.1f dBm\n", GetRssi());
    }
  }
}
