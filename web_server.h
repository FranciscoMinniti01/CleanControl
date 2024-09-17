#ifndef WEB_SERVER_H
#define WEB_SERVER_H
/*
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

// Definimos un tipo para la función callback que será llamada cuando se ingresen las credenciales.
typedef void (*CredencialesCallback)(const char* ssid, const char* password);

class WebServerManager {
public:
    WebServerManager();
    //bool iniciar(CredencialesCallback callback);
    void manejarServidor();

private:
    WebServer servidor; // Servidor web en el ESP32
    DNSServer dnsServer; // Servidor DNS para redirección

    CredencialesCallback callbackCredenciales; // Callback para cuando se ingresen las credenciales
    void handleRoot(); // Maneja la solicitud de la página principal
    void handleCredentials(); // Maneja la solicitud de envío de credenciales
};*/

#endif // WEB_SERVER_H
