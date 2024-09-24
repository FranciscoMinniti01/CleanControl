#ifndef CONFIG_H
#define CONFIG_H

// GENERAL CONFIG ----------------------------------------------------------------------------------------------------

#define DEBUG

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
#define TZ_INFO               "UTC-3"                         //Time zone info

#define INFLUXDB_BUCKET       "CleanControl"    // Este dato proximamente deberia ser cargado con el acces point
#define ID_DEVICE             "ESP32S3"           // Este dato proximamente deberia ser cargado con el acces point

#endif//CONFIG_H