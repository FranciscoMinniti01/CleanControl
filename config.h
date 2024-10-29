#ifndef CONFIG_H
#define CONFIG_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <string>
#include "Print.h"
#include "HardwareSerial.h"

#include "my_timer.h"
#include "storage.h"

// GENERAL CONFIG ----------------------------------------------------------------------------------------------------

#define DEBUG                 1
#define MAX_NUM_STORAGE       5

// TIMER CONFIG ----------------------------------------------------------------------------------------------------

#define MAX_NUM_TIMERS        5           // Este depende principalmente del numero de variables a publicar en influx mas dos timer que utilizo para la obtencion y el almacenado de datos - > MAX_DATA_INFLUX + 2
#define TIME_FREC             1000000
#define BASE_TIME_10mS        10000

#define TIME_10mS             1
#define TIME_60S              6000
#define TIME_30S              3000
#define TIME_10S              1000
#define TIME_1S               100

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
#define TZ_INFO               "UTC-3" //Time zone info
#define INFLUXDB_BUCKET       "CleanControl"

// DATA CONFIG ----------------------------------------------------------------------------------------------------

#define ID_DEVICE             "ESP32S3"           // Para cargado con el acces point
#define ID_CLIENTE            "Carrefour"         // Para cargado con el acces point

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

// INPUT DATA CONFIG ----------------------------------------------------------------------------------------------------

#define NUMBER_OF_DIGITAL_PIN 3
#define DIGITAL_PIN_CERO      15
#define DIGITAL_PIN_UNO       16
#define DIGITAL_PIN_DOS       17

#define NUMBER_OF_ANALOG_PIN  1
#define ANALOG_PIN_UNO        10

#define COUNTER_COMPARATOR    10

#endif//CONFIG_H