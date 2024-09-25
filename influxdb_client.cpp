#include "influxdb_client.h"

// POINT --------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
point_c<T>::point_c(char* measurement)
    : point(String(measurement))
{   
}

template <typename T>
void point_c<T>::TagPoint(std::initializer_list<std::pair<const char*, T>> tags) {
    for (const auto& tag : tags)
    {
        point.addTag(String(tag.first), tag.second);
    }
}

template <typename T>
void point_c<T>::FieldPoint(std::initializer_list<std::pair<const char*, T>> fields)
{
    point.clearFields();
    for (const auto& field : fields)
    {
        point.addField(String(fields.first),fields.second);
    }
}

// INFLUXDB --------------------------------------------------------------------------------------------------------------------------------------------

influxDB_c::influxDB_c(const char* url, const char* org, const char* bucket, const char* token, const char* cert) 
    : client(url, org, bucket, token, cert)
    //: client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert)
{

}

void influxDB_c::TimeSync()
{
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
}

bool influxDB_c::ClientConnection()
{
    if (client.validateConnection()){
        Serial.print("CONNECTED TO INFLUXDB: ");
        Serial.println(client.getServerUrl());
        return true;
    } else {
        Serial.println("ERROR: InfluxDB connection failed: ");
        Serial.print("    ");
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
        Serial.println("ERROR: InfluxDB write failed: ");
        Serial.print("    ");
        Serial.println(client.getLastErrorMessage());
        return false;
    }
    return true;
}