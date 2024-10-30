#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>

#include "config.h"

#define DEBUG_WIFI 1

// ACCESS POINT CONFIG ----------------------------------------------------------------------------------------------------

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

// VARIABLES STORAGE PARAMETERS ----------------------------------------------------------------------------------------------------

#define OFSET MAX_CREDENCIALES*2

// SPECIAL PARAMETERS
#define NUM_SPECIAL_PARAM     2
#define MACHINE_ID_INDEX      OFSET + 0
#define CLIENT_ID_INDEX       OFSET + 1

typedef struct {
    String machine_id;
    String client_id;
} sv_param_t;

// VARIABLES SERVER ----------------------------------------------------------------------------------------------------

typedef void(*handle_fun)();

typedef struct {
    String      root;
    HTTPMethod  request;
    handle_fun  fun;
} hdmi_root_t;

// VARIABLES WIFI ----------------------------------------------------------------------------------------------------

enum State_Wifi_t
{
    WIFI_INIT,
    WIFI_DEINIT,
    WIFI_READY,

    WIFI_AP_INIT,
    WIFI_AP_SERVER_INIT,
    WIFI_AP_SERVER_DEINIT,
    WIFI_AP_DEINIT,
    WIFI_AP_READY,

    WIFI_STA_INIT,
    WIFI_STA_DEINIT,
    WIFI_STA_READY
};

typedef struct {
  String   ssid;
  String   password;
} credentials_t;

// FUNCTIONS ---------------------------------------------------------------------------------------------------- 

void WiFiStateMachine();
void WiFi_manager();

float getRSSI();
bool getWifiStatus();
bool getWiFiError();
String getSSID();
sv_param_t* getSpecialParam();

#endif//WIFI_MANAGER_H




