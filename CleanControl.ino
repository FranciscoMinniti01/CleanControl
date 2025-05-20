
// INCLUDES -------------------------------------------------------------------------------------------

#include "CleanControl.h"


// VARIABLES ------------------------------------------------------------------------------------------

my_timer_t TimerDataWifi;
my_timer_t TimerDataBattery;
my_timer_t TimerDataMotor;


// DATA -----------------------------------------------------------------------------------------------

void InitDataTimer()
{
  set_timer(&TimerDataWifi, C_D_WIFI, NULL);
  delay(10);
  set_timer(&TimerDataBattery, C_D_BATTERY, NULL);
  delay(10);
  set_timer(&TimerDataMotor, C_D_MOTOR, NULL);
  delay(10);
}

void GlobalTags()
{
  static user_param_t*  UserParam = GetConfigParam();
  AddTag(T_ID_DEVICE,   UserParam->machine_id );
  AddTag(T_ID_CLIENTE,  UserParam->client_id );
}

void WhiteWifi()
{
  if( get_flag_timer( &TimerDataWifi ) )
  {
    SetPoint(M_D_WIFI);
    GlobalTags();
    
    AddTag(T_D_WIFI_SSID,    GetSsid());

    AddField(F_D_WIFI_RSSI,  GetRssi());
    
    if ( !WhitePoint() ) Serial.print("ERROR: Wifi data can't sent\n");
    else Serial.print("INFO: Wifi data sent\n");
  }
}

void WhiteBattery()
{
  if( get_flag_timer( &TimerDataBattery ) )
  {
    SetPoint(M_D_BATTERY);
    GlobalTags();

    AddField(F_D_BATTERY, GetAnalogAverage(P_D_BATTERY));
    
    if ( !WhitePoint() ) Serial.print("ERROR: battery data can't sent\n");
    else Serial.print("INFO: battery data sent\n");
  }
}

void WhiteSpecificMotor(String Motor, uint8_t Pin)
{
  SetPoint(Motor);
  GlobalTags();

  AddField(F_D_MOTOR_STATE,  GetDigitalValues(Pin, STATE));
  AddField(F_D_MOTOR_TON,    GetDigitalValues(Pin, TIME_ON));
  AddField(F_D_MOTOR_TOFF,   GetDigitalValues(Pin, TIME_OFF));
  AddField(F_D_MOTOR_TTON,   GetDigitalValues(Pin, TOTAL_TIME_ON));
  AddField(F_D_MOTOR_TTOFF,  GetDigitalValues(Pin, TOTAL_TIME_OFF));

  if ( !WhitePoint() ) Serial.printf("ERROR: %s data can't sent\n", Motor);
  else Serial.printf("INFO: %s data sent\n", Motor);
}

void WhiteMotor()
{
  if( get_flag_timer( &TimerDataMotor ) )
  {
    WhiteSpecificMotor(M_D_TRACCION, P_D_TRACCION);
    WhiteSpecificMotor(M_D_ASPIRACION, P_D_ASPIRACION);
    WhiteSpecificMotor(M_D_CEPILLO, P_D_CEPILLO);
  }
}


// FUNCTIONS ------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n---------- APP INIT ----------");

  timer_init();
  ServerInit();
  GpioInit();
  InfluxInit(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert, TZ_INFO);

  InitDataTimer();
}

void loop()
{
  static bool IsInfluxConnected = false;

  WifiManager();
  GpioManager();

  if(GetWifiStatus())
  {
    if(IsInfluxConnected == false) InfluxConnection();

    if(InfluxIsConnected())
    {
      IsInfluxConnected = true;
      WhiteWifi();
      WhiteBattery();
      WhiteMotor();
    }
    else IsInfluxConnected = false;
  }
}


// ----------------------------------------------------------------------------------------------------