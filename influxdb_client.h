#ifndef INFLUXDB_CLIENT_H
#define INFLUXDB_CLIENT_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "config.h"

// GENERAL CONFIG ----------------------------------------------------------------------------------------------------

#define DEBUG_INFLUX

// MAIN DEFINES ----------------------------------------------------------------------------------------------------

class influxDB_c
{
    public:
        influxDB_c(const char* url, const char* org, const char* bucket, const char* token, const char* cert);
        void TimeSync();
        bool ClientConnection();
        bool ValidateConnection();
        bool WhitePoint(Point& point);
        
    private:
        InfluxDBClient client;
};

template <typename T>
class point_c
{
    public:
        point_c(char* measurement);
        void TagPoint(std::initializer_list<std::pair<const char*, T>> tags);
        void FieldPoint(std::initializer_list<std::pair<const char*, T>> fields);

    private:
        Point point;
};

#endif//INFLUXDB_CLIENT_H



