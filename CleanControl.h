#ifndef CLEAN_CONTROL_H
#define CLEAN_CONTROL_H

// --------------------------------------------------
// ----------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------


// INCLUDES -------------------------------------------------------------------------------------------

#include <string>
#include "Print.h"
#include "HardwareSerial.h"

#include "src/Tools/TimerManager.h"
#include "src/Tools/Storage.h"
#include "src/Services/InfluxDBManager.h"
#include "src/Services/WifiManager.h"
#include "src/Services/GpiosManager.h"
#include "src/App/ServerManager.h"
//../../

// GENERAL CONFIG ------------------------------------------------------------------------------------

#define INFLUXDB_URL          "http://149.78.55.22:10010"
#define INFLUXDB_ORG          "7a51ffbe1c65ce0f"      
#define INFLUXDB_BUCKET       "cleancontrol"      
#define INFLUXDB_TOKEN        "58lWlntgNVpzfnK91ZUajQlOkm9z7fmYMMM4YwT2A7JXbaOQiUkD49f0Tb4FarHtNHY6T4q6G9xzs_ZpNi--9A=="
#define TZ_INFO               "UTC-3" //Time zone info

#define MACHINE_TIPE_S20
#define MACHINE_TIPE_S32


// DATA CONFIG ----------------------------------------------------------------------------------------

// GLOBAL TAGS --------------------------------------
#define T_ID_DEVICE          "MAQUINA"
#define T_ID_CLIENTE         "CLIENTE"


// D_WIFI -------------------------------------------
#define M_D_WIFI                "WIFI"
#define T_D_WIFI_SSID           "SSID"
#define F_D_WIFI_RSSI           "RSSI"
#define C_D_WIFI                TIME_30S


// D_BATTERY ----------------------------------------
#define M_D_BATTERY             "BATTERY"
#define F_D_BATTERY             "State"
#define C_D_BATTERY             TIME_30S
#define P_D_BATTERY             10


// D_MOTOR ------------------------------------------
#define M_D_TRACCION            "TRACCION"
#define M_D_ASPIRACION          "ASPIRACION"
#define M_D_CEPILLO             "CEPILLO"
#define M_D_CEPILLO_IZ          "CEPILLO_IZ"
#define M_D_CEPILLO_DE          "CEPILLO_DE"

#define F_D_MOTOR_STATE         "State"
#define F_D_MOTOR_TON           "TON"
#define F_D_MOTOR_TOFF          "TOFF"
#define F_D_MOTOR_TTON          "TTON"
#define F_D_MOTOR_TTOFF         "TTOFF"

#define C_D_MOTOR               TIME_30S

#define P_D_TRACCION            15
#define P_D_ASPIRACION          16
#define P_D_CEPILLO             17
#define P_D_CEPILLO_IZ          0
#define P_D_CEPILLO_DE          0


// --------------------------------------------------


// ----------------------------------------------------------------------------------------------------

#endif//CLEAN_CONTROL_H