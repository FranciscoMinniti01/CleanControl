#include "influxdb_client.h"

// POINT ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

point_c::point_c(const char* measurement): point(String(measurement)){ }

void point_c::FieldClear(){point.clearFields();}

Point& point_c::getPoint(){ return point;}

void point_c::TagPoint(const char* tag , const char* value)
{
    point.addTag(String(tag),value);
    Serial.print("AddTag: ");
    Serial.println(value);
}

template <class T>
void point_c::FieldPoint(const char* fields , T value)
{
    point.addField(String(fields),value);
}
template void point_c::FieldPoint(const char* fields, int value);
template void point_c::FieldPoint(const char* fields, double value);
template void point_c::FieldPoint(const char* fields, float value);
template void point_c::FieldPoint(const char* fields, char* value);
template void point_c::FieldPoint(const char* fields, bool value);

// INFLUXDB ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

influxDB_c::influxDB_c(const char* url, const char* org, const char* bucket, const char* token, const char* cert) : client(url, org, bucket, token, cert) {}

bool influxDB_c::ClientConnection()
{
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
    if (client.validateConnection()) {
        Serial.print("CONNECTED TO INFLUXDB: ");
        Serial.println(client.getServerUrl());
        return true;
    } else {
        Serial.println("ERROR: InfluxDB connection failed:    ");
        Serial.println(client.getLastErrorMessage());
        return false;
    }
}

bool influxDB_c::ValidateConnection()
{
    if(client.validateConnection()) return true;
    else return false;
}

bool influxDB_c::WhitePoint(Point& point)
{
    if (!client.writePoint(point))
    {
        Serial.println("ERROR: InfluxDB write failed:   ");
        Serial.println(client.getLastErrorMessage());
        return false;
    }
    return true;
}

