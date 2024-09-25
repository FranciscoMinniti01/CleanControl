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

class point_c
{
    public:
        point_c(const char* measurement);
        void FieldClear();
        Point& getPoint();
        //template <class T>
        //void TagPoint(const char* tag , T value);
        void TagPoint(const char* tag , const char* value);
        template <class T>
        void FieldPoint(const char* fields , T value);

    private:
        Point point;
};

#endif//INFLUXDB_CLIENT_H



