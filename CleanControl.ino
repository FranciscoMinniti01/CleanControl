// APP MAIN ----------------------------------------------------------------------------------------------------

// INCLUDES ----------------------------------------------------------------------------------------------------

#include "CleanControl.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

static bool is_influx_initialized = false;
static user_param_t* user_param = NULL;

gpio_data_t data[NUMBER_OF_DATA];

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n---------- APP INIT ----------");

  timer_init();
  server_init();
  //gpio_data_init();
  user_param = get_special_param(); 

  // DATA -----------------------------------
  data[D_WIFI].point  = new Point(M_WIFI);
  set_timer(&(data[D_WIFI].timer), TIME_D_WIFI);
  // ----------------------------------------
}

void loop()
{
  WiFi_manager();
  //gpio_data_control();

  if(getWifiStatus())
  {
    if(!is_influx_initialized)
    {
      // FRAN aca despues tengo que pasar estas configuraciones harcodiadas a una estructura de variables y recuperarlas de memoria (Pero cuando defina por donde van a ingresar)
      influx_init(INFLUXDB_URL,INFLUXDB_ORG,INFLUXDB_BUCKET,INFLUXDB_TOKEN,InfluxDbCloud2CACert);
      is_influx_initialized = true;
    }
    else if(influx_is_connected())
    {
      // DATA -----------------------------------
      if(get_flag_timer(&(data[D_WIFI].timer)))
      {
        static String last_ssid = "";
        String ssid = getSSID();
        float rssi = (float)getRSSI();
        Serial.print("DEBUGGGGG: 1\n");
        Serial.printf("DEBUGGGGG: SSID: %s\n",ssid);
        Serial.printf("DEBUGGGGG: SSID: %s\n",getSSID().c_str());
        Serial.printf("DEBUGGGGG: Last SSID: %s\n",last_ssid);
        Serial.printf("DEBUGGGGG: RSSI: %d\n",(float)getRSSI());
        Serial.printf("DEBUGGGGG: RSSI: %d\n",rssi);
        Serial.printf("DEBUGGGGG: Point: %p\n",data[D_WIFI].point);
        Serial.print("DEBUGGGGG: 2\n");
        if(last_ssid != getSSID())
        {
          Serial.print("DEBUGGGGG: Chain SSID\n");
          last_ssid = getSSID();
          data[D_WIFI].point->addTag(T_WIFI_SSID,ssid.c_str());
        }
        Serial.print("DEBUGGGGG: 3\n");
        data[D_WIFI].point->clearFields();
        data[D_WIFI].point->addField(F_WIFI_RSSI,rssi);
        Serial.print("DEBUGGGGG: 4\n");
        Serial.printf("DEBUGGGGG: Point: %p\n",data[D_WIFI].point);
        if(influx_white_point(data[D_WIFI].point)) Serial.println("INFO: Mensurement WIFI send");
        Serial.print("DEBUGGGGG: 6\n");      
      }
      // ----------------------------------------

      if(user_param->is_updated)
      {
        for(int i=0 ; i<NUMBER_OF_DATA ; i++)
        {
          // USER PARAMETERS ------------------------
          if(!(user_param->machine_id.isEmpty())) data[i].point->addTag(TG_ID_DEVICE,(user_param->machine_id).c_str());
          // ----------------------------------------
          if(!(user_param->client_id.isEmpty())) data[i].point->addTag(TG_ID_CLIENTE,(user_param->client_id).c_str());
          // ----------------------------------------
        }
        user_param->is_updated = false;
      }

    }
    else
    {
      influx_connection(TZ_INFO);
    }
  }
}

// APP MAIN ----------------------------------------------------------------------------------------------------



// TEST WIFI MAIN ----------------------------------------------------------------------------------------------------
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
// TEST WIFI MAIN ----------------------------------------------------------------------------------------------------



// TEST STORAGE MAIN ----------------------------------------------------------------------------------------------------
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
// TEST STORAGE MAIN ----------------------------------------------------------------------------------------------------