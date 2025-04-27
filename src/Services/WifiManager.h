#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

// DESCRIPTION ----------------------------------------------------------------------------------------
/*
  Modulo:
    El modulo permite mantener una coneccion STA wifi con las credenciales que se le cargen a la vez que permite activar un servidor y mostrar las rootas configuradas

  Notas:
    - La clave del AP no puede ser menor a 8 caracteres ni mayor (creo) de 60 caracteres
    - La SSID del AP no puede ser vacia ni mayor de 60 caracteres
    - La SSID en addAP de wifi multi no puede ser mayor a 30 caracteres
    - La Clave en addAP de wifi multi no puede ser mayor a 60 caracteres

  Pendientes: FRAN
    - Queda pendiente ver quien o donde se crea la intancia de Wifi
    - Para areglar esto podria implementar dos timers que llamen a una callback para ejecutar periodicamente la tareas correspondientes.
      - Queda pendinete el uso de server.stop() en WIFI_AP_DEINIT, este metodo elimina las rutas configuradas y en el codigo actual estas no se volverian a restaurar
      - Queda pendiente el manejo del error al guardar las credenciales en memoria no volatil
    - Queda pendiente evalur la secuencia de error
    - Queda pendiente dar un error si la credenciales superan el largo configurado. Tener en cuenta el caracter final.
      Esto se va a poder hacer desde server manager conciderando el return de la funcion set_credentials en wifi manager
*/


// INCLUDES -------------------------------------------------------------------------------------------

#include <string>
#include "Print.h"
#include "HardwareSerial.h"

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>

#include "../../src/Tools/Storage.h"
#include "../../src/Tools/TimerManager.h"


// ACCESS POINT ---------------------------------------------------------------------------------------

#define SSID_AP               "CleanControlAP"
#define PASSWORD_AP           "12345678"
#define CHANNEL_AP            1
#define SSID_HIDDEN_AP        0
#define MAX_CONNECTION_AP     1
#define AP_CONF_FTM           false
#define AP_AUTH_MODE          WIFI_AUTH_WPA2_PSK //WIFI_AUTH_WPA3_PSK o WIFI_AUTH_WPA2_WPA3_PSK
#define AP_CIPHER_TYPE        WIFI_CIPHER_TYPE_CCMP

#define LOCAL_IP_1            192
#define LOCAL_IP_2            168
#define LOCAL_IP_3            4
#define LOCAL_IP_4            1

#define GATEWAY_IP_1          192
#define GATEWAY_IP_2          168
#define GATEWAY_IP_3          4
#define GATEWAY_IP_4          1

#define SUBNET_IP_1           255
#define SUBNET_IP_2           255
#define SUBNET_IP_3           255
#define SUBNET_IP_4           0


// SERVER ---------------------------------------------------------------------------------------------

typedef void(*handle_fun)();

typedef struct {
    String      root;
    HTTPMethod  request;
    handle_fun  fun;
} hdmi_root_t;


// CREDENTIALS ----------------------------------------------------------------------------------------

#define MAX_CREDENCIALES      3   // Maximo numero de credenciales wifi que se pueden almacenar en el dispositivo
#define MAX_LEN_CREDENCIALES  30  // Largo maximo de las credenciales wifi que se pueden almacenar en el dispositivo

#define KEY_CREDENTIAL_PASSWORD "CP"
#define KEY_CREDENTIAL_SSID     "CS"

#define INDEX_SSID              0
#define INDEX_PASSWORD          1

typedef struct {
  char   ssid[MAX_LEN_CREDENCIALES+1];
  char   password[MAX_LEN_CREDENCIALES+1];
} credentials_t;


// STRUCTS TYPEDEFS DEFINES ---------------------------------------------------------------------------

enum State_Wifi_t
{
  WIFI_INIT,
  WIFI_READY,
  WIFI_DEINIT
};

enum State_AP_t
{
  WIFI_AP_INIT,
  WIFI_AP_READY,
  WIFI_AP_DEINIT
};

enum State_STA_t
{
  WIFI_STA_INIT,
  WIFI_STA_READY,
  WIFI_STA_DEINIT
};


// PUBLIC FUNCTIONS ----------------------------------------------------------------------------------- 

void SetHdmiRoot(String root_ , HTTPMethod request_ , handle_fun fun_);

bool SetCredentials(String ssid, String password);

void WifiManager();

credentials_t* GetCredentials();

float GetRssi();

bool GetWifiStatus();

bool IsWifiError();

String GetSsid();


// ----------------------------------------------------------------------------------------------------

#endif//WIFI_MANAGER_H