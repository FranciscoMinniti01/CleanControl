#include "influxdb_client.h"

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
Point sensor("wifi_status");

void connect(){
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

    if (client.validateConnection()) {
      Serial.print("Connected to InfluxDB: ");
      Serial.println(client.getServerUrl());
    } else {
      Serial.print("InfluxDB connection failed: ");
      Serial.println(client.getLastErrorMessage());
    }

    sensor.addTag("device", ID_DEVICE);
    sensor.addTag("SSID", "Claro-hogar");
}

void sendData(float rssi){
    sensor.clearFields();
    sensor.addField("rssi",(int)rssi);
    if (!client.writePoint(sensor)) {
        Serial.print("InfluxDB write failed: ");
        Serial.println(client.getLastErrorMessage());
    }
}
