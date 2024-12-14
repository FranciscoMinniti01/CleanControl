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
//#include "gpio_data.h"

// INFLUXDB CONFIG ----------------------------------------------------------------------------------------------------

#define INFLUXDB_URL          "http://149.78.55.22:10010"
#define INFLUXDB_ORG          "84c63ebe8b1b2dd6"      
#define INFLUXDB_BUCKET       "Test"      
#define INFLUXDB_TOKEN        "CLXk6Wa3zif4lsBb4dV4X67oxRCfGxZ9wykmpoRlFYKFkw61ijCkqHH7JZ5XYz8T7vtp7qpb2CdBD6V2AGXXBQ=="
#define TZ_INFO               "UTC-3" //Time zone info

// DATA CONFIG ----------------------------------------------------------------------------------------------------

//GLOBAL TAGS
#define TG_ID_DEVICE         "MAQUINA"
#define TG_ID_CLIENTE        "CLIENTE"

typedef struct {
  Point*       point;
  my_timer_t  timer;
}gpio_data_t;

// DATA -----------------------------------
#define NUMBER_OF_DATA        1

enum index_poins
{
  D_WIFI,
  D_ONOFF
};
// ----------------------------------------

// DATA -----------------------------------
#define M_WIFI          "WIFI"
#define T_WIFI_SSID     "SSID"
#define F_WIFI_RSSI     "RSSI"
#define TIME_D_WIFI     TIME_60S
// ----------------------------------------
#define MENSUREMENT_ONOFF     "ONOFF"
#define FIELT_ONOFF_STATE     "State"
#define FIELT_ONOFF_TON       "TON"
#define FIELT_ONOFF_TOFF      "TOFF"
#define FIELT_ONOFF_TTON      "TTON"
#define FIELT_ONOFF_TTOFF     "TTOFF"
// ----------------------------------------

// ----------------------------------------------------------------------------------------------------

#endif//CLEAN_CONTROL_H