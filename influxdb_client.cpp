// INCLUDES ----------------------------------------------------------------------------------------------------

#include "influxdb_client.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

InfluxDBClient client;

// POINT ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
point.clearFields();
point.addTag(String(tag),value);
point.addField(String(fields),value);
*/

// INFLUXDB ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void influx_init(const char* url, const char* org, const char* bucket, const char* token, const char* cert)
{
  client = InfluxDBClient(url, org, bucket, token, cert);
}

bool influx_connection(const char* time_zone)
{
  timeSync(time_zone, "pool.ntp.org", "time.nis.gov");
  if (client.validateConnection())
  {
    Serial.print("CONNECTED TO INFLUXDB: ");
    Serial.println(client.getServerUrl());
    return true;
  }
  else
  {
    Serial.println("ERROR: InfluxDB connection failed:    ");
    Serial.println(client.getLastErrorMessage());
    return false;
  }
}

bool influx_is_connected()
{
  if(client.validateConnection()) return true;
  else return false;
}

bool influx_white_point(Point* point)
{
  if (!client.writePoint(*point))
  {
    Serial.println("ERROR: InfluxDB write failed:   ");
    Serial.println(client.getLastErrorMessage());
    return false;
  }
  return true;
}

// ----------------------------------------------------------------------------------------------------