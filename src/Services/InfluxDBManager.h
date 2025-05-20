#ifndef INFLUXDB_CLIENT_H
#define INFLUXDB_CLIENT_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include "Print.h"
#include "HardwareSerial.h"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "../../src/Tools/TimerManager.h"


// FUNCTIONS POINT ----------------------------------------------------------------------------------------------------

void SetPoint( String mensurement );

void AddTag( String tag, String value_tag );

void AddField( String field, String value );
void AddField( String field, bool value );
void AddField( String field, float value );
void AddField( String field, uint32_t value );
void AddField( String field, uint16_t value );
void AddField( String field, uint8_t value );

bool WhitePoint();

void DeletePoint();


// FUNCTIONS INFLUXDB ----------------------------------------------------------------------------------------------------

void InfluxInit(const char* url, const char* org, const char* bucket, const char* token, const char* cert, const char* tz);
bool InfluxConnection();
bool InfluxIsConnected();


// ----------------------------------------------------------------------------------------------------

#endif//INFLUXDB_CLIENT_H



