// influxdb_client.h
#ifndef INFLUXDB_CLIENT_H
#define INFLUXDB_CLIENT_H

#include <WiFiClient.h>   // Para manejar conexiones HTTP
#include <HTTPClient.h>   // Para manejar peticiones HTTP

class InfluxDBClient {
public:
    InfluxDBClient(const char* serverUrl, const char* org, const char* bucket, const char* token);

    bool enviarDatos(const char* data, size_t length); // Método para enviar datos a InfluxDB
    void setDeviceID(const char* id);                  // Configurar el ID del dispositivo
    void setLocation(const char* location);            // Configurar la ubicación del dispositivo

private:
    const char* serverUrl;   // URL del servidor InfluxDB
    const char* org;         // Organización en InfluxDB
    const char* bucket;      // Bucket en InfluxDB
    const char* token;       // Token de autenticación para InfluxDB

    const char* deviceID;    // ID del dispositivo
    const char* location;    // Ubicación del dispositivo

    WiFiClient wifiClient;   // Cliente Wi-Fi para la conexión HTTP
    HTTPClient httpClient;   // Cliente HTTP para enviar las peticiones

    String prepararPayload(const char* data); // Método para preparar el payload en formato line protocol
};

#endif // INFLUXDB_CLIENT_H
