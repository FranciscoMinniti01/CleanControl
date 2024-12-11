#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

/* 
  Notas:
    - La clave del AP no puede ser menor a 8 caracteres ni mayor (creo) de 60 caracteres
    - La SSID del AP no puede ser mayor de 63 caracteres
    - La SSID en addAP de wifi multi no puede ser mayor a 30 caracteres
    - La Clave en addAP de wifi multi no puede ser mayor a 60 caracteres

    - Problemas: 
      - En set_credentials recibe la ssid y la password como string, podria cambiarlo a puntero a char pero no estoy seguro de las ventajas o desventajas
      - Si po alguna razon no se llama a set_hdmi_root antes del estado WIFI_AP_SERVER_INIT, no se inicializa el servidor web
*/

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>

#include "config.h"

// ACCESS POINT ----------------------------------------------------------------------------------------------------

// ROOT ----------------------------------------
#define MAX_HDMI_ROOT 2
// ----------------------------------------

#define CHANNEL_AP            1
#define SSID_HIDDEN_AP        0

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

// SERVER ----------------------------------------------------------------------------------------------------

typedef void(*handle_fun)();

typedef struct {
    String      root;
    HTTPMethod  request;
    handle_fun  fun;
} hdmi_root_t;

// WIFI ----------------------------------------------------------------------------------------------------

enum State_Wifi_t
{
  WIFI_INIT,
  WIFI_READY,
  WIFI_DEINIT
};

enum State_AP_t
{
  WIFI_AP_INIT,
  WIFI_AP_SERVER_INIT,
  WIFI_AP_READY,
  WIFI_AP_SERVER_DEINIT,
  WIFI_AP_DEINIT
};

enum State_STA_t
{
  WIFI_STA_INIT,
  WIFI_STA_CLEAR_CREDENTIALS,
  WIFI_STA_ADD_CREDENTIALS,
  WIFI_STA_READY,
  WIFI_STA_DEINIT
};

#define KEY_CREDENTIAL_PASSWORD "CP"
#define KEY_CREDENTIAL_SSID     "CS"

typedef struct {
  char   ssid[MAX_LEN_CREDENCIALES];
  char   password[MAX_LEN_CREDENCIALES];
} credentials_t;

// PRIVATE FUNCTIONS ---------------------------------------------------------------------------------------------------- 

void get_storage_credentials();

void WiFi_manager();

void WiFi_stateMachine();

void AP_stateMachine();

void STA_stateMachine();

// PUBLIC FUNCTIONS ---------------------------------------------------------------------------------------------------- 

void set_hdmi_root(String root_ , HTTPMethod request_ , handle_fun fun_);

bool set_credentials(String ssid, String password);

void WiFi_manager();

credentials_t* get_credentials();

float getRSSI();

bool getWifiStatus();

bool getWiFiError();

String getSSID();

// ----------------------------------------------------------------------------------------------------

#endif//WIFI_MANAGER_H




