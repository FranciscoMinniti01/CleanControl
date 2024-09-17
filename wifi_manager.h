#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "utils.h"
#include "web_server.h"
#include "config.h"

#define MAX_CREDENCIALES 3

enum StateWifiManager
{
  Estado_Inicializacion,
  Estado_IntentarConexionWiFi,
  Estado_MantenerConexionEstablecida,
  Estado_Intentar_Reconexion,
  Estado_Modo_AccessPoint,
  Estado_GuardarCredenciales
};

class WiFiManager
{
  public:
    WiFiManager();
    void manejarWiFi();

  private:
    StateWifiManager estadoActual;
    char ssids[MAX_CREDENCIALES][MAX_LEN_CREDENCIALES];       // Array para almacenar los nombres de redes
    char passwords[MAX_CREDENCIALES][MAX_LEN_CREDENCIALES];   // Array para almacenar las contraseñas

    void iniciarWiFi();
    void intentarConexion();
    void mantenerConexion();
    void intentarReconexion();
    void iniciarAccessPoint();
    void guardarCredenciales(const char* ssid, const char* password);
    void cargarCredencialesGuardadas();
};

#endif // WIFI_MANAGER_H

