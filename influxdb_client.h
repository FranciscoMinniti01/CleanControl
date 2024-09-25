#ifndef INFLUXDB_CLIENT_H
#define INFLUXDB_CLIENT_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "config.h"
#include <string>

// GENERAL CONFIG ----------------------------------------------------------------------------------------------------

#define DEBUG_INFLUX

// MAIN DEFINES ----------------------------------------------------------------------------------------------------

enum StateWifiManager
{
    INFLUX_TIMESYNC,
    INFLUX_VAL_CONN,
    INFLUX_ADD_TAG,
    INFLUX_ADD_FIELD,
    INFLUX_WRITE_POINT
};

class influxDB_c
{
    public:
        influxDB_c(const char* url, const char* org, const char* bucket, const char* token, const char* cert);
        
    private:
        StateWifiManager influxDBState;
        InfluxDBClient client;

        void influxDBStateMachine();
};

class point_c
{
    public:
        point_c(char* measurement);

    private:
        Point point;
};

#endif//INFLUXDB_CLIENT_H



