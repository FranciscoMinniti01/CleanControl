// INCLUDES ----------------------------------------------------------------------------------------------------

#include "influxdb_client.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

InfluxDBClient*       client = NULL;
std::vector<Point*>   ptr_points;
static uint16_t       points_index = 0;
static const char*    time_zone;

// FUNCTIONS POINT ----------------------------------------------------------------------------------------------------

uint16_t set_point(String mensurement)
{
  Point* aux_point = new Point(mensurement.c_str());
  ptr_points.push_back(aux_point);
  return points_index ++;
}

void add_Tag(uint16_t id, String tag, String value_tag ) { ptr_points[id]->addTag(tag.c_str(), value_tag.c_str()); }

void clear_Fields(uint16_t id) { ptr_points[id]->clearFields(); }

void clear_Tags(uint16_t id) { ptr_points[id]->clearTags(); }

void add_Field(uint16_t id, String field, String value) { ptr_points[id]->addField( field.c_str() , value.c_str() ); }
void add_Field(uint16_t id, String field, int value   ) { ptr_points[id]->addField( field.c_str() , value ); }
void add_Field(uint16_t id, String field, char value  ) { ptr_points[id]->addField( field.c_str() , value ); }
void add_Field(uint16_t id, String field, bool value  ) { ptr_points[id]->addField( field.c_str() , value ); }
void add_Field(uint16_t id, String field, float value ) { ptr_points[id]->addField( field.c_str() , value ); }

bool influx_white_point(uint16_t id)
{
  if(client == NULL) return false;
  if ( !client->writePoint( *(ptr_points[0]) ) )
  {
    Serial.print("ERROR: InfluxDB write failed:\n     ");
    Serial.println(client->getLastErrorMessage());
    return false;
  }
  return true;
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