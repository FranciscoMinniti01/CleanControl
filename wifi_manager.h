#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "utils.h"

#define MAX_CREDENCIALES 3

class WiFiManager
{
  public:
    WiFiManager();
    void manejarWiFi();

  private:
    EstadoWiFi estadoActual;
    char ssids[MAX_CREDENCIALES][32];  // Array para almacenar los nombres de redes
    char passwords[MAX_CREDENCIALES][64];  // Array para almacenar las contraseñas

    void iniciarWiFi();
    void intentarConexion();
    void mantenerConexion();
    void intentarReconexion();
    void iniciarAccessPoint();
    void guardarCredenciales(const char* ssid, const char* password);
    void cargarCredencialesGuardadas();
    void cargarCredencialesGuardadas();
};

#endif // WIFI_MANAGER_H

