// INCLUDES ----------------------------------------------------------------------------------------------------

#include "wifi_manager.h"
#include "influxdb_client.h"
#include "server_manager.h"
#include "gpio_data.h"
#include "config.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

bool is_wifi_connected      = false;
bool is_influx_connected    = false;
bool is_influx_initialized  = false;
user_param_t* sv_param        = NULL;

gpio_data_t data[NUMBER_OF_DATA];

// MAIN FUNCTIONS ----------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n---------- APP INIT ----------");

  // DATA -----------------------------------
  data[DATA_WIFI].point  = new Point(MENSUREMENT_ONOFF);
  set_timer(&(data[DATA_WIFI].timer), TIME_30S);
  // ----------------------------------------

  timer_init();
  server_init();
  gpio_data_init();
  sv_param = get_special_param(); 
}

void loop()
{
  WiFi_manager();
  gpio_data_control();
  if(getWifiStatus())
  {
    if(is_influx_initialized && influx_is_connected())
    {
      // DATA -----------------------------------
      if(get_flag_timer(&(data[DATA_WIFI].timer)))
      {
        data[DATA_WIFI].point->addTag(TAG_WIFI_SSID,getSSID().c_str());
        data[DATA_WIFI].point->clearFields();
        data[DATA_WIFI].point->addField(FIELT_WIFI_RSSI,getRSSI());
        if(influx_white_point(data[DATA_WIFI].point)) Serial.println("INFO: Mensurement WIFI send");
      }
      // ----------------------------------------
    }
    else if(!is_influx_initialized)
    { 
      if(!sv_param->machine_id.isEmpty() && !sv_param->client_id.isEmpty())
      {
        influx_init(INFLUXDB_URL,INFLUXDB_ORG,INFLUXDB_BUCKET,INFLUXDB_TOKEN,InfluxDbCloud2CACert);
        sv_param->is_updated = true;
        is_influx_initialized = true;
      }
    }
    else
    {
      influx_connection(TZ_INFO);
    }

    if(sv_param->is_updated)
    {
      for(int i=0 ; i<NUMBER_OF_DATA ; i++)
      {
        data[i].point->addTag(TAG_ID_DEVICE,(sv_param->machine_id).c_str());
        data[i].point->addTag(TAG_ID_CLIENTE,(sv_param->client_id).c_str());
      }
    }

  }
}



