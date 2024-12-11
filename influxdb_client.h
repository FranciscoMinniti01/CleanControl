#ifndef INFLUXDB_CLIENT_H
#define INFLUXDB_CLIENT_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include "Print.h"
#include "HardwareSerial.h"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// MAIN DEFINES ----------------------------------------------------------------------------------------------------

void influx_init(const char* url, const char* org, const char* bucket, const char* token, const char* cert);
bool influx_connection(const char* time_zone);
bool influx_is_connected();
bool influx_white_point(Point* point);

// ----------------------------------------------------------------------------------------------------

#endif//INFLUXDB_CLIENT_H



