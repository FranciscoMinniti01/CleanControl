// influxdb_client.cpp
#include "influxdb_client.h"

// Constructor: Inicializa las variables necesarias
InfluxDBClient::InfluxDBClient(const char* serverUrl, const char* org, const char* bucket, const char* token) 
    : serverUrl(serverUrl), org(org), bucket(bucket), token(token), deviceID(""), location("") {}

// Configurar el ID del dispositivo
void InfluxDBClient::setDeviceID(const char* id) {
    this->deviceID = id;
}

// Configurar la ubicación del dispositivo
void InfluxDBClient::setLocation(const char* location) {
    this->location = location;
}

// Método para enviar datos a InfluxDB
bool InfluxDBClient::enviarDatos(const char* data, size_t length) {
    if (!WiFi.isConnected()) {
        Serial.println("Error: No hay conexión Wi-Fi.");
        return false;
    }

    String payload = prepararPayload(data); // Preparar el payload

    httpClient.begin(wifiClient, String(serverUrl) + "/api/v2/write?org=" + org + "&bucket=" + bucket + "&precision=s");
    httpClient.addHeader("Authorization", "Token " + String(token));
    httpClient.addHeader("Content-Type", "text/plain");

    int httpResponseCode = httpClient.POST(payload); // Enviar la petición POST

    if (httpResponseCode > 0) {
        Serial.println("Datos enviados a InfluxDB exitosamente.");
        httpClient.end();
        return true;
    } else {
        Serial.printf("Error al enviar datos a InfluxDB: %d\n", httpResponseCode);
        httpClient.end();
        return false;
    }
}

// Método para preparar el payload en formato line protocol
String InfluxDBClient::prepararPayload(const char* data) {
    String payload = "device_data";
    
    if (deviceID[0] != '\0') {
        payload += ",device_id=" + String(deviceID);
    }
    if (location[0] != '\0') {
        payload += ",location=" + String(location);
    }
    payload += " value=" + String(data) + "i";
    
    return payload;
}
