// APP MAIN ------------------------------------------------------------------------------------------------------------------------------------------------------
/*
// INCLUDES ----------------------------------------------------------------------------------------------------

#include "CleanControl.h"


// VARIABLES ----------------------------------------------------------------------------------------------------

static bool is_influx_connected = false;
static user_param_t* user_param = NULL;

// DATA --------------------------------------------------
my_timer_t timer_d_wifi;    // D_WIFI
my_timer_t timer_d_onoff;   // D_ONOFF
// --------------------------------------------------


// FUNCTIONS ----------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n---------- APP INIT ----------");

  timer_init();
  server_init();
  gpio_data_init();
  motion_init();
  user_param = get_special_param(); 

  influx_init(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert, TZ_INFO);

  // DATA --------------------------------------------------
  set_timer(&timer_d_wifi, CLOCK_D_WIFI, NULL);     // D_WIFI
  delay(10);
  set_timer(&timer_d_onoff, CLOCK_D_ONOFF, NULL);   // D_ONOFF
  delay(10);
  // --------------------------------------------------
}

void loop()
{
  WiFi_manager();
  gpio_data_control();
  motion_control();

  if(getWifiStatus())
  {
    if(is_influx_connected == false) influx_connection();

    if(influx_is_connected())
    {
      is_influx_connected = true;
      // D_WIFI --------------------------------------------------
      if( get_flag_timer( &timer_d_wifi ) )
      {
        set_Point(M_D_WIFI);
        add_Tag(TG_ID_DEVICE,  user_param->machine_id );
        add_Tag(TG_ID_CLIENTE, user_param->client_id );
        add_Tag(T_D_WIFI_SSID,getSSID());
        add_Field(F_D_WIFI_RSSI, getRSSI());
        if ( !white_Point() ) Serial.print("ERROR: Wifi data can't sent\n");
        else Serial.print("INFO: Wifi data sent\n");
      }
      // D_ONOFF --------------------------------------------------
      if( get_flag_timer( &timer_d_onoff ) )
      {
        digital_pin_t* traction_motor = get_digital_pin(MOTOR_TRACCION);
        set_Point(M_D_ONOFF);
        add_Tag(TG_ID_DEVICE,  user_param->machine_id );
        add_Tag(TG_ID_CLIENTE, user_param->client_id );
        add_Field(F_D_ONOFF_STATE,  traction_motor->state);
        add_Field(F_D_ONOFF_TON,    traction_motor->time_state[1]);
        add_Field(F_D_ONOFF_TOFF,   traction_motor->time_state[0]);
        add_Field(F_D_ONOFF_TTON,   traction_motor->total_time_state[1]);
        add_Field(F_D_ONOFF_TTOFF,  traction_motor->total_time_state[0]);
        if ( !white_Point() ) Serial.print("ERROR: onoff data can't sent\n");
        else Serial.print("INFO: onoff data sent\n");
      }
      // --------------------------------------------------
    }
    else is_influx_connected = false;

  }
}


// ----------------------------------------------------------------------------------------------------
*/
// APP MAIN ------------------------------------------------------------------------------------------------------------------------------------------------------
 







// MPU6050 TEST MAIN 1 ------------------------------------------------------------------------------------------------------------------------------------------------------

#include <string>
#include "Print.h"
#include "HardwareSerial.h"

#include "my_timer.h"
#include "MPU6050_module.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n\n\n\n\n\n---------- MPU6050 TEST MAIN ----------");

  timer_init();
  motion_init();
}

void loop()
{
  while(true)
  {
    motion_control();
  }
}

// MPU6050 TEST MAIN 1 ------------------------------------------------------------------------------------------------------------------------------------------------------


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