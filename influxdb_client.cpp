// INCLUDES ----------------------------------------------------------------------------------------------------

#include "influxdb_client.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

InfluxDBClient*     client = NULL;
Point*              ptr_point = NULL;
static const char*  time_zone;

// FUNCTIONS POINT ----------------------------------------------------------------------------------------------------

void set_Point( String mensurement )
{ 
  ptr_point = new Point(mensurement.c_str());
  if(ptr_point != NULL)
  {
    ptr_point->clearFields(); 
    ptr_point->clearTags();
  } 
}

void add_Tag( String tag, String value_tag ) { if(ptr_point != NULL) ptr_point->addTag( tag.c_str(), value_tag.c_str() ); }

void add_Field( String field, String value )    { if(ptr_point != NULL) ptr_point->addField( field.c_str() , value.c_str() ); }
void add_Field( String field, bool value )      { if(ptr_point != NULL) ptr_point->addField( field.c_str() , value ); }
void add_Field( String field, float value )     { if(ptr_point != NULL) ptr_point->addField( field.c_str() , value ); }
void add_Field( String field, uint32_t value )  { if(ptr_point != NULL) ptr_point->addField( field.c_str() , value ); }
void add_Field( String field, uint16_t value )  { if(ptr_point != NULL) ptr_point->addField( field.c_str() , value ); }
void add_Field( String field, uint8_t value )   { if(ptr_point != NULL) ptr_point->addField( field.c_str() , value ); }

bool white_Point()
{
  if(client == NULL)    return false;
  if(ptr_point == NULL) return false;
  if ( !client->writePoint( *(ptr_points[id]) ) )
  {
    Serial.print("ERROR: InfluxDB write failed:\n     ");
    Serial.println(client->getLastErrorMessage());
    return false;
  }
  return true;
}

void delete_Point()
{ 
  delete ptr_point;
  ptr_point = NULL;
}

// FUNCTIONS INFLUXDB ----------------------------------------------------------------------------------------------------

void influx_init(const char* url, const char* org, const char* bucket, const char* token, const char* cert, const char* tz)
{
  client    = new InfluxDBClient(url, org, bucket, token, cert);
  time_zone = tz;
}

bool influx_connection()
{
  if(client == NULL) return false;
  timeSync(time_zone, "pool.ntp.org", "time.nis.gov");
  if (client->validateConnection())
  {
    Serial.print("INFO: Connected to InfluxDB:\n     ");
    Serial.println(client->getServerUrl());
    return true;
  }
  else
  {
    Serial.print("ERROR: InfluxDB connection failed:\n     ");
    Serial.println(client->getLastErrorMessage());
    return false;
  }
}

bool influx_is_connected()
{
  if(client == NULL) return false;
  if(client->validateConnection()) return true;
  else return false;
}

// ----------------------------------------------------------------------------------------------------