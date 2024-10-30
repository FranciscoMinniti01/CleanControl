// INCLUDES ----------------------------------------------------------------------------------------------------

#include "wifi_manager.h"
#include "influxdb_client.h"
#include "config.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

influxDB_c*   influxClient  = nullptr;
bool flagWiFiConnection     = false;
bool InfluxConnection       = false;
sv_param_t* sv_param        = NULL;

// DATA -----------------------------------
point_c DataON(M_DataOn);
int16_t timer_DataOn;
// ----------------------------------------
point_c DataWifi(M_DataWifi);
int16_t timer_DataWifi;
// ----------------------------------------



// MAIN FUNCTIONS ----------------------------------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\n\n\n\nAPP INIT");

    timer_init();

    // DATA -----------------------------------
    timer_DataOn   = set_timer(TIME_30S);
    // ----------------------------------------
    timer_DataWifi = set_timer(TIME_30S);
    // ----------------------------------------
}

void loop()
{
    WiFi_manager();
    sv_param = getSpecialParam();

    if(getWifiStatus())
    {    
        if (influxClient == nullptr)
        {
            influxClient = new influxDB_c(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
            
            // DATA -----------------------------------
            DataON.TagPoint(T_ID_DEVICE,ID_DEVICE);
            DataON.TagPoint(T_ID_CLIENTE,ID_CLIENTE);
            // ----------------------------------------
            DataWifi.TagPoint(T_ID_DEVICE,ID_DEVICE);
            DataWifi.TagPoint(T_ID_CLIENTE,ID_CLIENTE);
            // ----------------------------------------
        }

        if(!InfluxConnection) {
            influxClient->ClientConnection();
            InfluxConnection = true;
        }

        if(!flagWiFiConnection) {
            DataWifi.TagPoint(T_DataWifi,getSSID().c_str());
            flagWiFiConnection = true;
        }

        if(influxClient->ValidateConnection())
        {
            // DATA -----------------------------------
            if(get_flag_timer(timer_DataWifi))
            {
                DataWifi.FieldClear();
                DataWifi.FieldPoint(F1_DataWifi, getRSSI());
                if(influxClient->WhitePoint(DataWifi.getPoint())) {
                    Serial.println("DataWifi send");
                }
            }
            // ----------------------------------------
            if(get_flag_timer(timer_DataOn))
            {
                DataON.FieldClear();
                DataON.FieldPoint(F1_DataOn, 1);
                if(influxClient->WhitePoint(DataON.getPoint())) {
                    Serial.println("DataON send");
                }
            }
            // ----------------------------------------
        }
        else InfluxConnection = false;
    }
    else flagWiFiConnection = false;
}



