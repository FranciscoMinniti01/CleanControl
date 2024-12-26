#ifndef CLEAN_CONTROL_H
#define CLEAN_CONTROL_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <string>
#include "Print.h"
#include "HardwareSerial.h"

#include "my_timer.h"
#include "influxdb_client.h"
#include "wifi_manager.h"
#include "server_manager.h"
#include "gpio_data.h"

// DEFINES ----------------------------------------------------------------------------------------------------

typedef struct {
  uint16_t      id;
  my_timer_t    timer;
}data_t;

// INFLUXDB CONFIG ----------------------------------------------------------------------------------------------------

#define INFLUXDB_URL          "http://149.78.55.22:10010"
#define INFLUXDB_ORG          "7a51ffbe1c65ce0f"      
#define INFLUXDB_BUCKET       "cleancontrol"      
#define INFLUXDB_TOKEN        "58lWlntgNVpzfnK91ZUajQlOkm9z7fmYMMM4YwT2A7JXbaOQiUkD49f0Tb4FarHtNHY6T4q6G9xzs_ZpNi--9A=="
#define TZ_INFO               "UTC-3" //Time zone info

// DATA CONFIG ----------------------------------------------------------------------------------------------------

// GLOBAL TAGS ----------------------------------------
#define TG_ID_DEVICE          "MAQUINA"
#define TG_ID_CLIENTE         "CLIENTE"
// ----------------------------------------

// DATA ----------------------------------------
#define NUMBER_OF_DATA        1
enum data_index
{
  D_WIFI,
  D_ONOFF
};
// ----------------------------------------

// D_WIFI ----------------------------------------
#define M_D_WIFI              "WIFI"
#define T_D_WIFI_SSID         "SSID"
#define F_D_WIFI_RSSI         "RSSI"
#define TIME_D_WIFI           TIME_30S
// D_ONOFF ----------------------------------------
#define MENSUREMENT_ONOFF     "ONOFF"
#define FIELT_ONOFF_STATE     "State"
#define FIELT_ONOFF_TON       "TON"
#define FIELT_ONOFF_TOFF      "TOFF"
#define FIELT_ONOFF_TTON      "TTON"
#define FIELT_ONOFF_TTOFF     "TTOFF"
// ----------------------------------------

// ----------------------------------------------------------------------------------------------------

#endif//CLEAN_CONTROL_H