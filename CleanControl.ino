// INCLUDES ----------------------------------------------------------------------------------------------------

#include "wifi_manager.h"
#include "influxdb_client.h"
#include "server_manager.h"
#include "gpio_data.h"
#include "config.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

influxDB_c*   influxClient  = nullptr;
bool is_wifi_connected      = false;
bool is_influx_connected    = false;
sv_param_t* sv_param        = NULL;

// DATA ----------------------------------------
point_c DataON(M_DataOn);
my_time_t timer_DataOn;
// ----------------------------------------
point_c DataWifi(M_DataWifi);
my_time_t timer_DataWifi;
// ----------------------------------------

// MAIN FUNCTIONS ----------------------------------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\n\n\n\n---------- APP INIT ----------");

    timer_init();
    server_init();
    gpio_data_init();

    // DATA -----------------------------------
    timer_DataOn   = set_timer(TIME_30S);
    // ----------------------------------------
    timer_DataWifi = set_timer(TIME_30S);
    // ----------------------------------------
}

void loop()
{
  WiFi_manager();
  gpio_data_control();

  if(getWifiStatus())
  {    
    if (influxClient == nullptr)
    {
      influxClient = new influxDB_c(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
      
      sv_param = get_special_param();

      // DATA -----------------------------------
      DataON.TagPoint(T_ID_DEVICE,(sv_param->machine_id).c_str());
      DataON.TagPoint(T_ID_CLIENTE,(sv_param->client_id).c_str());
      // ----------------------------------------
      DataWifi.TagPoint(T_ID_DEVICE,(sv_param->machine_id).c_str());
      DataWifi.TagPoint(T_ID_CLIENTE,(sv_param->client_id).c_str());
      // ----------------------------------------
    }

    if(!is_influx_connected) {
      influxClient->ClientConnection();
      is_influx_connected = true;
    }

    if(!is_wifi_connected) {
      DataWifi.TagPoint(T_DataWifi,getSSID().c_str());
      is_wifi_connected = true;
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
      digital_pin_t* digital_pin = get_digital_pin(0);

      if(get_flag_timer(timer_DataOn))
      {
        DataON.FieldClear();
        DataON.FieldPoint(F1_DataOn, digital_pin->state );
        DataON.FieldPoint(F2_DataOn, digital_pin->time_state[1] );
        DataON.FieldPoint(F3_DataOn, digital_pin->total_time_state[1]);
        if(influxClient->WhitePoint(DataON.getPoint())) {
            Serial.println("DataON send");
        }
      }
      // ----------------------------------------
    }
    else is_influx_connected = false;
  }
  else is_wifi_connected = false;
}



