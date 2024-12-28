#ifndef INFLUXDB_CLIENT_H
#define INFLUXDB_CLIENT_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include "Print.h"
#include "HardwareSerial.h"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "my_timer.h"


// FUNCTIONS POINT ----------------------------------------------------------------------------------------------------

void set_Point( String mensurement );

void add_Tag( String tag, String value_tag );

void add_Field( String field, String value );
void add_Field( String field, bool value );
void add_Field( String field, float value );
void add_Field( String field, uint32_t value );
void add_Field( String field, uint16_t value );
void add_Field( String field, uint8_t value );

bool white_Point();

void delete_Point();


// FUNCTIONS INFLUXDB ----------------------------------------------------------------------------------------------------

void influx_init(const char* url, const char* org, const char* bucket, const char* token, const char* cert, const char* tz);
bool influx_connection();
bool influx_is_connected();


// ----------------------------------------------------------------------------------------------------

#endif//INFLUXDB_CLIENT_H



