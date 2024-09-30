// INCLUDES ----------------------------------------------------------------------------------------------------

#include "wifi_manager.h"
#include "influxdb_client.h"
#include "time.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

hw_timer_t* timer_DataON    = NULL;
bool flag_DataON            = true;
point_c DataON(M_DataOn);

hw_timer_t* timer_DataWifi  = NULL;
bool flag_DataWifi          = true;
point_c DataWifi(M_DataWifi);

WiFiManager_c wifimanager;
influxDB_c*   influxClient  = nullptr;
bool flagWiFiConnection     = false;

// TIME CALLBACK ----------------------------------------------------------------------------------------------------

void fun_DataON() {
    flag_DataON = true;
}

void fun_DataWifi() {
    flag_DataWifi = true;
}

// MAIN FUNCTIONS ----------------------------------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);

    timer_DataON = timerBegin(TIME_FREC);
    timerAttachInterrupt(timer_DataON, &fun_DataON);
    timerAlarm(timer_DataON, TIME_10S, true, 0);

    timer_DataWifi = timerBegin(TIME_FREC);
    timerAttachInterrupt(timer_DataWifi, &fun_DataWifi);
    timerAlarm(timer_DataWifi, TIME_10S, true, 0);

    Serial.println("\n\n\n\n\nAPP INIT");
}

void loop()
{
    wifimanager.WiFiStateMachine();

    if(wifimanager.getWifiStatus())
    {    
        if (influxClient == nullptr) {
            influxClient = new influxDB_c(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
            influxClient->TimeSync();
            influxClient->ClientConnection();                  //Esto no deberia ir aca solo asi
            
            DataON.TagPoint(T_ID_DEVICE,ID_DEVICE);
            DataON.TagPoint(T_ID_CLIENTE,ID_CLIENTE);
            DataWifi.TagPoint(T_ID_DEVICE,ID_DEVICE);
            DataWifi.TagPoint(T_ID_CLIENTE,ID_CLIENTE);
        }

        if(!flagWiFiConnection) {
            DataWifi.TagPoint(T_DataWifi,wifimanager.getSSID());
            flagWiFiConnection = true;
        }

        if(influxClient->ValidateConnection())
        {
            if(flag_DataON) {
                DataON.FieldClear();
                DataON.FieldPoint(F1_DataOn, 1);
                if(influxClient->WhitePoint(DataON.getPoint())) {
                    flag_DataON = false;
                    Serial.println("DataON send");
                }
            }
            if(flag_DataWifi) {
                DataWifi.FieldClear();
                DataWifi.FieldPoint(F1_DataWifi, wifimanager.getRSSI());
                if(influxClient->WhitePoint(DataWifi.getPoint())) {
                    flag_DataWifi = false;
                    Serial.println("DataWifi send");
                }
            }
        }
    } else flagWiFiConnection = false;
}



