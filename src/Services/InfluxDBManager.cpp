// INCLUDES ----------------------------------------------------------------------------------------------------

#include "../../src/Services/InfluxDBManager.h"


// VARIABLES ----------------------------------------------------------------------------------------------------

InfluxDBClient*     Client = NULL;
Point*              PtrPoint = NULL;
static const char*  TimeZone;


// FUNCTIONS POINT ----------------------------------------------------------------------------------------------------

void SetPoint( String mensurement )
{ 
  if(PtrPoint != NULL) DeletePoint();
  PtrPoint = new Point(mensurement.c_str());
  PtrPoint->clearFields(); 
  PtrPoint->clearTags(); 
}

void AddTag( String tag, String value_tag ) { if(PtrPoint != NULL) PtrPoint->addTag( tag.c_str(), value_tag.c_str() ); }

void AddField( String field, String value )    { if(PtrPoint != NULL) PtrPoint->addField( field.c_str() , value.c_str() ); }
void AddField( String field, bool value )      { if(PtrPoint != NULL) PtrPoint->addField( field.c_str() , value ); }
void AddField( String field, float value )     { if(PtrPoint != NULL) PtrPoint->addField( field.c_str() , value ); }
void AddField( String field, uint32_t value )  { if(PtrPoint != NULL) PtrPoint->addField( field.c_str() , value ); }
void AddField( String field, uint16_t value )  { if(PtrPoint != NULL) PtrPoint->addField( field.c_str() , value ); }
void AddField( String field, uint8_t value )   { if(PtrPoint != NULL) PtrPoint->addField( field.c_str() , value ); }

bool WhitePoint()
{
  if(Client == NULL)  return false;
  if(PtrPoint == NULL)   return false;
  if ( !Client->writePoint( *PtrPoint ) )
  {
    Serial.print("ERROR: InfluxDB write failed:\n     ");
    Serial.println(Client->getLastErrorMessage());
    DeletePoint();
    return false;
  }
  DeletePoint();
  return true;
}

void DeletePoint()
{ 
  delete PtrPoint;
  PtrPoint = NULL;
}


// FUNCTIONS INFLUXDB ----------------------------------------------------------------------------------------------------

void InfluxInit(const char* url, const char* org, const char* bucket, const char* token, const char* cert, const char* tz)
{
  Client   = new InfluxDBClient(url, org, bucket, token, cert);
  TimeZone = tz;
}

bool InfluxConnection()
{
  if(Client == NULL) return false;
  timeSync(TimeZone, "pool.ntp.org", "time.nis.gov");
  if (Client->validateConnection())
  {
    Serial.print("INFO: Connected to InfluxDB:\n     ");
    Serial.println(Client->getServerUrl());
    return true;
  }
  else
  {
    Serial.print("ERROR: InfluxDB connection failed:\n     ");
    Serial.println(Client->getLastErrorMessage());
    return false;
  }
}

bool InfluxIsConnected()
{
  if(Client == NULL) return false;
  if(Client->validateConnection()) return true;
  else return false;
}


// ----------------------------------------------------------------------------------------------------