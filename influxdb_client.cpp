#include "influxdb_client.h"

/*Point sensor("wifi_status");

void connect()
{
    sensor.addTag("device", ID_DEVICE);
    sensor.addTag("SSID", "Claro-hogar");
}

void sendData(float rssi)
{
    sensor.clearFields();
    sensor.addField("rssi",(int)rssi);
    if (!client.writePoint(sensor)) {
        Serial.print("InfluxDB write failed: ");
        Serial.println(client.getLastErrorMessage());
    }
}*/

point_c::point_c(char* measurement)
    : point(String(measurement))
{

}

influxDB_c::influxDB_c(const char* url, const char* org, const char* bucket, const char* token, const char* cert) 
    : client(url, org, bucket, token, cert)
    //: client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert)
{

}

void influxDB_c::influxDBStateMachine()
{
    switch (influxDBState)
    {
        case INFLUX_TIMESYNC:
            timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
            break;

        case INFLUX_VAL_CONN:
            if (client.validateConnection()){
                Serial.print("Connected to InfluxDB: ");
                Serial.println(client.getServerUrl());
            } else {
                Serial.print("ERROR: InfluxDB connection failed: ");
                Serial.print("\n\n");
                Serial.println(client.getLastErrorMessage());
            }
            break;
    }
}