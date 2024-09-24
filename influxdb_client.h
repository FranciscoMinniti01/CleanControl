#ifndef INFLUXDB_CLIENT_H
#define INFLUXDB_CLIENT_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "config.h"

//----------------------------------------------------------------------------------------------------



// MAIN DEFINES ----------------------------------------------------------------------------------------------------

void connect();
void sendData(float rssi);

/*class influxDBManager_c
{
    public:

    private:
        InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
}*/

#endif//INFLUXDB_CLIENT_H
