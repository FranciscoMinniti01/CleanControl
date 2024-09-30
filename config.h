#ifndef CONFIG_H
#define CONFIG_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <string>

// GENERAL CONFIG ----------------------------------------------------------------------------------------------------

#define DEBUG                 1

#define TIME_FREC             1000000
#define TIME_60S              60000000
#define TIME_10S              10000000
#define TIME_1S               1000000

// WIFI CONFIG ----------------------------------------------------------------------------------------------------

#define MAX_CREDENCIALES      3   // Maximo numero de credenciales wifi que se pueden almacenar en el dispositivo
#define MAX_LEN_CREDENCIALES  30  // Largo maximo de las credenciales wifi que se pueden almacenar en el dispositivo

// ACCESS POINT CONFIG ----------------------------------------------------------------------------------------------------

#define SSID_AP               "CleanControlAP"
#define PASSWORD_AP           "1234"
#define MAX_CONNECTION_AP     1

// INFLUXDB CONFIG ----------------------------------------------------------------------------------------------------

#define INFLUXDB_URL          "http://200.23.159.2:10010"
#define INFLUXDB_ORG          "7f9af91f5425e5ef"            
#define INFLUXDB_TOKEN        "8mpluT_LBK3_nKJIVmOscI-WEO3QCPCg9mg-_VRZFMBq2bpSJk7ySHW6Im0H3MOmmUQUZ-e2Vf34CKMlpf0fcg=="
#define TZ_INFO               "UTC-3"             //Time zone info

#define INFLUXDB_BUCKET       "CleanControl"      // Este dato proximamente deberia ser cargado con el acces point

// DATA CONFIG ----------------------------------------------------------------------------------------------------

#define ID_DEVICE             "ESP32S3"           // Estos dato proximamente deberia ser cargado con el acces point
#define ID_CLIENTE            "Carrefour"         // Estos dato proximamente deberia ser cargado con el acces point

//TAGs
#define T_ID_DEVICE           "MAQUINA"
#define T_ID_CLIENTE          "CLIENTE"

//DataOn
#define M_DataOn              "DataOn"
#define F1_DataOn             "OnOff"

//DataWifi
#define M_DataWifi            "DataWifi"
#define T_DataWifi            "SSID"
#define F1_DataWifi           "RSSI"


#endif//CONFIG_H