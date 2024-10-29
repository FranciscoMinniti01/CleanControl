#ifndef WIFI_MANAGER_OLD_H
#define WIFI_MANAGER_OLD_H
/*
// INCLUDES ----------------------------------------------------------------------------------------------------

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>

#include "config.h"

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

// VARIABLES ----------------------------------------------------------------------------------------------------



// PARAMETROS ESPECIALES ----------------------------------------------------------------------------------------------------

#define NUM_SPECIAL_PARAM     2
#define MAX_LEN_SPECIAL_PARAM 30
#define OFSET MAX_CREDENCIALES*2

#define MACHINE_ID_INDEX      OFSET + 0
#define CLIENT_ID_INDEX       OFSET + 1

// MAIN DEFINES ----------------------------------------------------------------------------------------------------

enum StateWifiManager
{
    WIFI_INIT,

    WIFI_AP_INIT,
    WIFI_AP_SERVER_INIT,
    WIFI_AP_DEINIT,
    
    WIFI_STA_INIT,
    WIFI_STA_DEINIT,
    
    WIFI_READY
};

float getRSSI();
bool getWifiStatus();
const char* getSSID();

void WiFiEvent(WiFiEvent_t event);
void WiFiManager_c();
void WiFiStateMachine();
*/
// ----------------------------------------------------------------------------------------------------

void FormRoot();
void FormSubmitRoot();

#endif//WIFI_MANAGER_H

