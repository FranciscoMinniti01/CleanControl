#ifndef INFLUXDB_CLIENT_H
#define INFLUXDB_CLIENT_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "config.h"

// MAIN DEFINES ----------------------------------------------------------------------------------------------------

void influx_init(const char* url, const char* org, const char* bucket, const char* token, const char* cert);
bool influx_connection();
bool is_influx_connected();
bool influx_white_point(Point& point);

// ----------------------------------------------------------------------------------------------------

#endif//INFLUXDB_CLIENT_H



