#ifndef INFLUXDB_CLIENT_H
#define INFLUXDB_CLIENT_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include "Print.h"
#include "HardwareSerial.h"
#include <vector>

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "my_timer.h"

// FUNCTIONS POINT ----------------------------------------------------------------------------------------------------

uint16_t set_point(String mensurement);

void add_Tag(uint16_t id, String tag, String value_tag );

void clear_Fields(uint16_t id);

void clear_Tags(uint16_t id);

void add_Field(uint16_t id, String field, String value);
void add_Field(uint16_t id, String field, int value   );
void add_Field(uint16_t id, String field, char value  );
void add_Field(uint16_t id, String field, bool value  );
void add_Field(uint16_t id, String field, float value );

bool influx_white_point(uint16_t id);

// FUNCTIONS INFLUXDB ----------------------------------------------------------------------------------------------------

void influx_init(const char* url, const char* org, const char* bucket, const char* token, const char* cert, const char* tz);
bool influx_connection();
bool influx_is_connected();

// ----------------------------------------------------------------------------------------------------

#endif//INFLUXDB_CLIENT_H



