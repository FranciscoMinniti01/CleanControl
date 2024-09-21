#include "wifi_manager.h"      // Incluimos el módulo de Wi-Fi
//#include "web_server.h"        // Incluimos el servidor web (portal cautivo)
//#include "influxdb_client.h"   // Incluimos el cliente para enviar datos a InfluxDB

// Definimos constantes y variables globales
//const char* INFLUXDB_URL = "http://your-influxdb-server.com";
//const char* INFLUXDB_ORG = "your-org";
//const char* INFLUXDB_BUCKET = "your-bucket";
//const char* INFLUXDB_TOKEN = "your-token";

//InfluxDBClient influxClient(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

bool conexionEstablecida = false;
//unsigned long tiempoFuncionamientoMotor = 0;  // Dato que queremos enviar a InfluxDB
//unsigned long ultimoEnvio = 0;
//const unsigned long intervaloEnvio = 60000;  // Enviar datos cada 60 segundos

WiFiManager_c wifimanager;

void setup() {
  Serial.begin(115200);
  
  // Configuramos el cliente de InfluxDB
  //influxClient.setDeviceID("ESP32_001");  // Asignamos un ID al dispositivo
  //influxClient.setLocation("Factory A");  // Asignamos una ubicación

  // Inicializamos el módulo de Wi-Fi
}

void loop() {
  wifimanager.WiFiStateMachine();
}

// Callback que se llama cuando se reciben credenciales del portal cautivo
/*void onCredencialesRecibidas(const char* ssid, const char* password) {
  // Guardamos las credenciales
  strcpy(credencialesSSID[0], ssid);
  strcpy(credencialesPassword[0], password);
  estadoActual = Estado_GuardarCredenciales;  // Cambiamos el estado para guardar credenciales
}*/

// Función para enviar datos a InfluxDB
/*void enviarDatosInfluxDB() {
  String data = String(tiempoFuncionamientoMotor);  // Ejemplo de dato a enviar
  if (influxClient.enviarDatos(data.c_str(), data.length())) {
    Serial.println("Datos enviados a InfluxDB.");
  } else {
    Serial.println("Error al enviar datos a InfluxDB.");
  }
}*/


