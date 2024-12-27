// APP MAIN -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// INCLUDES ----------------------------------------------------------------------------------------------------

#include "CleanControl.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

static bool is_influx_connected = false;
static user_param_t* user_param = NULL;
data_t data[NUMBER_OF_DATA];

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n---------- APP INIT ----------");

  timer_init();
  server_init();
  gpio_data_init();
  user_param = get_special_param(); 

  influx_init(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert, TZ_INFO);

  // D_WIFI ----------------------------------------
  SET_DATA(data[D_WIFI], M_D_WIFI, TIME_D_WIFI, "", "")
  SET_DATA(data[D_ONOFF], M_D_ONOFF, TIME_D_ONOFF, "", "")
  // ----------------------------------------
}

void loop()
{
  WiFi_manager();
  gpio_data_control();

  if(getWifiStatus())
  {
    if(influx_is_connected())
    {
      // D_WIFI ----------------------------------------
      if( get_flag_timer( &(data[D_WIFI].timer) ) )
      {
        static String last_ssid = "";
        String ssid = getSSID();
        if(last_ssid != ssid)
        {
          last_ssid = ssid;
          add_Tag( data[D_WIFI].id, T_D_WIFI_SSID, ssid );
        }
        clear_Fields(data[D_WIFI].id);
        add_Field(data[D_WIFI].id, F_D_WIFI_RSSI, getRSSI());
        //if ( !influx_white_point(data[D_WIFI].id) ) Serial.print("ERROR: Wifi data can't sent\n");
        //else Serial.print("INFO: Wifi data sent\n");
      }
      // D_ONOFF ----------------------------------------
      if( get_flag_timer( &(data[D_ONOFF].timer) ) )
      {
        clear_Fields(data[D_ONOFF].id);
        digital_pin_t* traction_motor = get_digital_pin(MOTOR_TRACCION);
        add_Field(data[D_ONOFF].id,   F_D_ONOFF_STATE,  traction_motor->state);
        add_Field(data[D_ONOFF].id,   F_D_ONOFF_TON,    traction_motor->time_state[1]);
        add_Field(data[D_ONOFF].id,   F_D_ONOFF_TOFF,   traction_motor->time_state[0]);
        add_Field(data[D_ONOFF].id,   F_D_ONOFF_TTON,   traction_motor->total_time_state[1]);
        add_Field(data[D_ONOFF].id,   F_D_ONOFF_TTOFF,  traction_motor->total_time_state[0]);
        if ( !influx_white_point(data[D_ONOFF].id) ) Serial.print("ERROR: onoff data can't sent\n");
        else Serial.print("INFO: onoff data sent\n");
      }
      // ----------------------------------------

      if(user_param->is_updated)
      {
        for(int i=0 ; i<NUMBER_OF_DATA ; i++)
        {
          // GLOBAL TAGS ----------------------------------------
          if(!(user_param->machine_id.isEmpty())) add_Tag( data[i].id, TG_ID_DEVICE, user_param->machine_id );
          if(!(user_param->client_id.isEmpty()))  add_Tag( data[i].id, TG_ID_CLIENTE, user_param->client_id );
          // ----------------------------------------
        }
        Serial.print("INFO: Globals Tags adds\n");
        user_param->is_updated = false;
      }
    }
    else
    {
      influx_connection();
    }
  }
}

// ----------------------------------------------------------------------------------------------------

// APP MAIN -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 



// TEST WIFI MAIN -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
#include "wifi_manager.h"
#include "server_manager.h"
bool is_wifi_connected      = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n---------- TEST WIFI MAIN ----------");

  server_init();
}

void loop()
{
  WiFi_manager();
  if(getWifiStatus())
  {
    Serial.println("wifi conectado");
  }
}
*/
// TEST WIFI MAIN -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




// TEST STORAGE MAIN -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
#include <string>
#include "storage.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n---------- APP TEST STORAGE ----------");

  String test = "Funciona";
  storage_t storage_test;
  
  set_data_storage( &storage_test, (void*)test.c_str(), 8, "KeyTest" ); //sizeof((void*)test.c_str()), "KeyTest" );
  
  if(seve_data(&storage_test))Serial.println("Se guardo informacion con exito");
  else Serial.println("Save fallo, no se guardo la informacion");
  Serial.printf("test = %S\n",test);

  if(get_data(&storage_test)) Serial.println("Se obtubo informacion almacenada");
  else Serial.println("Get fallo, no se obtubo informacion almacenada");
  Serial.printf("test = %S\n",test);
}

void loop(){}

*/
// TEST STORAGE MAIN -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------