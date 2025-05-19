
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
  ServerInit();
  GpioInit();

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
  WifiManager();
  GpioManager();

  if(getWifiStatus())
  {
    if(is_influx_connected == false) influx_connection();

    if(influx_is_connected())
    {
      is_influx_connected = true;
      // D_WIFI --------------------------------------------------
      if( get_flag_timer( &timer_d_wifi ) )
      {
        user_param = GetConfigParam();
        set_Point(M_D_WIFI);
        add_Tag(TG_ID_DEVICE,     user_param->machine_id );
        add_Tag(TG_ID_CLIENTE,    user_param->client_id );
        add_Tag(T_D_WIFI_SSID,    getSSID());
        add_Field(F_D_WIFI_RSSI,  getRSSI());
        if ( !white_Point() ) Serial.print("ERROR: Wifi data can't sent\n");
        else Serial.print("INFO: Wifi data sent\n");
      }
      // D_ONOFF --------------------------------------------------
      if( get_flag_timer( &timer_d_onoff ) )
      {
        user_param = GetConfigParam();
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