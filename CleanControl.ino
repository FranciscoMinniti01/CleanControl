#include "wifi_manager.h"      // Incluimos el módulo de Wi-Fi
#include "web_server.h"        // Incluimos el servidor web (portal cautivo)
#include "influxdb_client.h"   // Incluimos el cliente para enviar datos a InfluxDB

// Definimos constantes y variables globales
const char* INFLUXDB_URL = "http://your-influxdb-server.com";
const char* INFLUXDB_ORG = "your-org";
const char* INFLUXDB_BUCKET = "your-bucket";
const char* INFLUXDB_TOKEN = "your-token";

InfluxDBClient influxClient(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

// Variable que almacenará el estado actual de la máquina de estados
enum EstadoWiFi {
  Estado_Inicializacion,
  Estado_IntentarConexionWiFi,
  Estado_MantenerConexionEstablecida,
  Estado_EvaluarErrores,
  Estado_Modo_AccessPoint,
  Estado_GuardarCredenciales
};

EstadoWiFi estadoActual = Estado_Inicializacion;
bool conexionEstablecida = false;
unsigned long tiempoFuncionamientoMotor = 0;  // Dato que queremos enviar a InfluxDB
unsigned long ultimoEnvio = 0;
const unsigned long intervaloEnvio = 60000;  // Enviar datos cada 60 segundos

void setup() {
  Serial.begin(115200);
  
  // Configuramos el cliente de InfluxDB
  influxClient.setDeviceID("ESP32_001");  // Asignamos un ID al dispositivo
  influxClient.setLocation("Factory A");  // Asignamos una ubicación

  // Inicializamos el módulo de Wi-Fi
  iniciarWiFi();
}

void loop() {
  switch (estadoActual) {
    case Estado_Inicializacion:
      Serial.println("Estado: Inicialización");
      iniciarWiFi();  // Intentar iniciar Wi-Fi
      if (!cargarCredencialesGuardadas()) {
        estadoActual = Estado_Modo_AccessPoint;  // No hay credenciales, ir a modo AP
      } else {
        estadoActual = Estado_IntentarConexionWiFi;  // Credenciales encontradas, intentar conexión
      }
      break;

    case Estado_IntentarConexionWiFi:
      Serial.println("Estado: Intentar Conexión Wi-Fi");
      if (intentarConexionWiFi()) {
        conexionEstablecida = true;
        estadoActual = Estado_MantenerConexionEstablecida;
      } else {
        conexionEstablecida = false;
        estadoActual = Estado_EvaluarErrores;
      }
      break;

    case Estado_MantenerConexionEstablecida:
      Serial.println("Estado: Mantener Conexión Establecida");
      if (WiFi.isConnected()) {
        // Verificamos si es hora de enviar los datos
        if (millis() - ultimoEnvio > intervaloEnvio) {
          enviarDatosInfluxDB();
          ultimoEnvio = millis();
        }
      } else {
        estadoActual = Estado_EvaluarErrores;  // Si se pierde la conexión, evaluamos el error
      }
      break;

    case Estado_EvaluarErrores:
      Serial.println("Estado: Evaluar Errores");
      if (!WiFi.isConnected()) {
        Serial.println("Conexión Wi-Fi perdida. Intentando reconectar...");
        estadoActual = Estado_IntentarConexionWiFi;
      }
      break;

    case Estado_Modo_AccessPoint:
      Serial.println("Estado: Modo Access Point");
      if (iniciarAccessPoint()) {
        Serial.println("Access Point iniciado. Esperando credenciales...");
        iniciarPortalCautivo(onCredencialesRecibidas);
      }
      break;

    case Estado_GuardarCredenciales:
      Serial.println("Estado: Guardar Credenciales");
      guardarCredenciales();  // Guardar credenciales en memoria no volátil
      estadoActual = Estado_IntentarConexionWiFi;  // Intentamos conectarnos con las nuevas credenciales
      break;
  }
}

// Callback que se llama cuando se reciben credenciales del portal cautivo
void onCredencialesRecibidas(const char* ssid, const char* password) {
  // Guardamos las credenciales
  strcpy(credencialesSSID[0], ssid);
  strcpy(credencialesPassword[0], password);
  estadoActual = Estado_GuardarCredenciales;  // Cambiamos el estado para guardar credenciales
}

// Función para enviar datos a InfluxDB
void enviarDatosInfluxDB() {
  String data = String(tiempoFuncionamientoMotor);  // Ejemplo de dato a enviar
  if (influxClient.enviarDatos(data.c_str(), data.length())) {
    Serial.println("Datos enviados a InfluxDB.");
  } else {
    Serial.println("Error al enviar datos a InfluxDB.");
  }
}


