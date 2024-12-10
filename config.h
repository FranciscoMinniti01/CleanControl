#ifndef CONFIG_H
#define CONFIG_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <string>
#include "Print.h"
#include "HardwareSerial.h"

#include "my_timer.h"
#include "storage.h"

// GENERAL CONFIG ----------------------------------------------------------------------------------------------------

#define DEBUG_WIFI            1
#define DEBUG_SERVER          1
#define DEBUG_INFLUX          1
#define DEBUG_DATA            1
#define DEBUG_APP             1

// WIFI CONFIG ----------------------------------------------------------------------------------------------------

#define MAX_CREDENCIALES      3   // Maximo numero de credenciales wifi que se pueden almacenar en el dispositivo
#define MAX_LEN_CREDENCIALES  30  // Largo maximo de las credenciales wifi que se pueden almacenar en el dispositivo

// ACCESS POINT CONFIG ----------------------------------------------------------------------------------------------------

#define SSID_AP               "CleanControlAP"
#define PASSWORD_AP           "12345678"
#define MAX_CONNECTION_AP     1

// INFLUXDB CONFIG ----------------------------------------------------------------------------------------------------

#define INFLUXDB_URL          "http://149.78.55.22:10010"
#define INFLUXDB_ORG          "7f9af91f5425e5ef"      
#define INFLUXDB_BUCKET       "CleanControl"      
#define INFLUXDB_TOKEN        "8mpluT_LBK3_nKJIVmOscI-WEO3QCPCg9mg-_VRZFMBq2bpSJk7ySHW6Im0H3MOmmUQUZ-e2Vf34CKMlpf0fcg=="
#define TZ_INFO               "UTC-3" //Time zone info

// DATA CONFIG ----------------------------------------------------------------------------------------------------

typedef struct {
  Point     point;
  my_time_t timer;
}data_t;

enum index_poins
{
  DATA_WIFI,
  DATA_ONOFF
};

#define NUMBER_OF_DATA        2

//GLOBAL TAGS
#define TAG_ID_DEVICE         "MAQUINA"
#define TAG_ID_CLIENTE        "CLIENTE"

//DataOn
#define MENSUREMENT_ONOFF     "ONOFF"
#define FIELT_ONOFF_STATE     "State"
#define FIELT_ONOFF_TON       "TON"
#define FIELT_ONOFF_TOFF      "TOFF"
#define FIELT_ONOFF_TTON      "TTON"
#define FIELT_ONOFF_TTOFF     "TTOFF"

//DataWifi
#define MENSUREMENT_WIFI      "WIFI"
#define TAG_WIFI_SSID         "SSID"
#define FIELT_WIFI_RSSI       "RSSI"

#endif//CONFIG_H