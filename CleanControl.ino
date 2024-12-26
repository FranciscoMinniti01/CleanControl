// APP MAIN -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// INCLUDES ----------------------------------------------------------------------------------------------------

#include "CleanControl.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

static bool is_influx_connected = false;
static user_param_t* user_param = NULL;

// DATA WIFI -----------------------------------
data_t data[NUMBER_OF_DATA];
/*uint16_t    p_d_wifi;
my_timer_t  t_d_wifi;*/
// ----------------------------------------

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n---------- APP INIT ----------");

  timer_init();
  server_init();
  //gpio_data_init();
  user_param = get_special_param(); 

  influx_init(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert, TZ_INFO);

  // DATA ----------------------------------------
  data[D_WIFI].id = set_point(M_D_WIFI);
  set_timer(&(data[D_WIFI].timer), TIME_D_WIFI, NULL);
  /*p_d_wifi = set_point(M_D_WIFI);
  set_timer(&t_d_wifi, TIME_D_WIFI, NULL);*/
  // ----------------------------------------
}

void loop()
{
  WiFi_manager();
  //gpio_data_control();

  if(getWifiStatus())
  {
    if(influx_is_connected())
    {
      // DATA ----------------------------------------
      if( get_flag_timer( &(data[D_WIFI].timer) ) )
      {
        /*Serial.print("DEBUGGGGG: 0\n");
        static String last_ssid = "";
        if(last_ssid != getSSID())
        {
          last_ssid = getSSID();
          add_Tag( data[D_WIFI].point, T_D_WIFI_SSID, last_ssid );
        }*/

        /*Serial.print("DEBUGGGGG: 1\n");
        clear_Fields(p_d_wifi);
        add_Field(p_d_wifi,F_WIFI_RSSI,4);
        Serial.print("DEBUGGGGG: 2\n");
        if ( !influx_white_point(p_d_wifi) ) Serial.print("ERROR: to sent wifi data\n");
        else Serial.print("Sent wifi data\n");*/

        Serial.print("DEBUGGGGG: 1\n");
        clear_Fields(data[D_WIFI].id);
        add_Field(data[D_WIFI].id, F_D_WIFI_RSSI, 4);
        Serial.print("DEBUGGGGG: 2\n");
        if ( !influx_white_point(data[D_WIFI].id) ) Serial.print("ERROR: to sent wifi data\n");
        else Serial.print("Sent wifi data\n");
      }
      // ----------------------------------------

      if(user_param->is_updated)
      {
        /*for(int i=0 ; i<NUMBER_OF_DATA ; i++)
        {
          // USER PARAMETERS ----------------------------------------
          if(!(user_param->machine_id.isEmpty())) add_Tag( data[i].point, TG_ID_DEVICE, user_param->machine_id );
          // ----------------------------------------
          if(!(user_param->client_id.isEmpty())) add_Tag( data[i].point, TG_ID_CLIENTE, user_param->client_id );
          // ----------------------------------------
        }*/
        user_param->is_updated = false;
      }
    }
    else
    {
      influx_connection();
    }
  }
}

// APP MAIN -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 



// TEST INFLUX MAIN 2 -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// INCLUDES ----------------------------------------------------------------------------------------------------
/*
#include "CleanControl.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

static bool is_influx_connected = false;
static user_param_t* user_param = NULL;

InfluxDBClient*       client;
std::vector<Point*>   ptr_points;
static uint16_t       points_index;

// DATA WIFI -----------------------------------
uint16_t    p_d_wifi;
my_timer_t  t_d_wifi;
// ----------------------------------------

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n---------- APP INIT ----------");

  timer_init();
  server_init();
  user_param = get_special_param(); 

  client = new InfluxDBClient(INFLUXDB_URL,INFLUXDB_ORG,INFLUXDB_BUCKET,INFLUXDB_TOKEN,InfluxDbCloud2CACert);

  // DATA WIFI -----------------------------------
  Point* point_temp = new Point(M_D_WIFI);
  ptr_points.push_back(point_temp);
  set_timer( &t_d_wifi, TIME_D_WIFI, NULL);
  // ----------------------------------------
}

void loop()
{
  WiFi_manager();

  if(getWifiStatus())
  {
    if(client->validateConnection())
    {
      if(get_flag_timer(&t_d_wifi))
      {
        Serial.print("DEBUGGGGG: 0\n");
        ptr_points[0]->clearFields();
        ptr_points[0]->addField(F_WIFI_RSSI, 2 );
        Serial.print("DEBUGGGGG: 1\n");
        if (!client->writePoint( *(ptr_points[0]) ) )
        {
          Serial.print("ERROR: InfluxDB write failed:\n     ");
          Serial.println(client->getLastErrorMessage());
        }else Serial.print("INFO: InfluxDB write SACCESSFUL:\n     ");
      }

      if(user_param->is_updated)
      {
        Serial.print("DEBUGGGGG: User param update\n");
        for(int i=0 ; i<NUMBER_OF_DATA ; i++)
        {
          // USER PARAMETERS ------------------------
          //if(!(user_param->machine_id.isEmpty())) point_test.addTag(TG_ID_DEVICE,(user_param->machine_id).c_str());
          // ----------------------------------------
          //if(!(user_param->client_id.isEmpty())) point_test.addTag(TG_ID_CLIENTE,(user_param->client_id).c_str());
          // ----------------------------------------
        }
        //point_test.addTag("TAGTEST","Test");
        user_param->is_updated = false;
      }

    }
    else
    {
      timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
      if (client->validateConnection())
      {
        Serial.print("INFO: Connected to InfluxDB:\n     ");
        Serial.println(client->getServerUrl());
      }
      else
      {
        Serial.print("ERROR: InfluxDB connection failed:\n     ");
        Serial.println(client->getLastErrorMessage());
      }
    }
  }
}
*/
// TEST INFLUX MAIN 2 -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------






// TEST INFLUX MAIN -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*

// INCLUDES ----------------------------------------------------------------------------------------------------

#include "CleanControl.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

static bool is_influx_connected   = false;
static user_param_t* user_param   = NULL;

//InfluxDBClient  client(INFLUXDB_URL,INFLUXDB_ORG,INFLUXDB_BUCKET,INFLUXDB_TOKEN,InfluxDbCloud2CACert);
//Point           p_d_wifi(M_D_WIFI);

InfluxDBClient*  client;

// DATA WIFI -----------------------------------
Point*      p_d_wifi;
my_timer_t  t_d_wifi;
// ----------------------------------------

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n---------- APP INIT ----------");

  timer_init();
  server_init();
  user_param = get_special_param(); 

  client = new InfluxDBClient(INFLUXDB_URL,INFLUXDB_ORG,INFLUXDB_BUCKET,INFLUXDB_TOKEN,InfluxDbCloud2CACert);

  // DATA WIFI -----------------------------------
  p_d_wifi = new Point(M_D_WIFI);
  set_timer( &t_d_wifi, TIME_D_WIFI, NULL);
  // ----------------------------------------
}

void loop()
{
  WiFi_manager();

  if(getWifiStatus())
  {
    if(client->validateConnection())
    {
      if(get_flag_timer(&t_d_wifi))
      {
        Serial.print("DEBUGGGGG: 0\n");
        p_d_wifi->clearFields();
        p_d_wifi->addField(F_WIFI_RSSI, 3 );
        Serial.print("DEBUGGGGG: 1\n");
        if (!client->writePoint( *p_d_wifi ) )
        {
          Serial.print("ERROR: InfluxDB write failed:\n     ");
          Serial.println(client->getLastErrorMessage());
        }else Serial.print("INFO: InfluxDB write SACCESSFUL:\n     ");
      }

      if(user_param->is_updated)
      {
        Serial.print("DEBUGGGGG: User param update\n");
        for(int i=0 ; i<NUMBER_OF_DATA ; i++)
        {
          // USER PARAMETERS ------------------------
          //if(!(user_param->machine_id.isEmpty())) point_test.addTag(TG_ID_DEVICE,(user_param->machine_id).c_str());
          // ----------------------------------------
          //if(!(user_param->client_id.isEmpty())) point_test.addTag(TG_ID_CLIENTE,(user_param->client_id).c_str());
          // ----------------------------------------
        }
        //point_test.addTag("TAGTEST","Test");
        user_param->is_updated = false;
      }

    }
    else
    {
      timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
      if (client->validateConnection())
      {
        Serial.print("INFO: Connected to InfluxDB:\n     ");
        Serial.println(client->getServerUrl());
      }
      else
      {
        Serial.print("ERROR: InfluxDB connection failed:\n     ");
        Serial.println(client->getLastErrorMessage());
      }
    }
  }
}
*/
// TEST INFLUX MAIN -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




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