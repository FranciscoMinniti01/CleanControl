#include "utils.h"

Preferences preferences;

void init_utils(const char* name)
{
    if(!preferences.begin("credentials") )
    {
        Serial.println("DEBUG: begin failed");
    }
}

bool ClearSpace(const char* name)
{
    preferences.begin(name,false);
    return preferences.clear();
}

bool PutData(const char* name, const char* key, void* value, const size_t len)
{
    Serial.print("PutData free entries:");
    Serial.println(preferences.freeEntries());
    
    /*if( !preferences.begin(name,false) ){
        Serial.println("DEBUG: begin failed");
        return false;
    }*/
    if( preferences.putBytes(key, value, len) == 0){
        Serial.println("DEBUG: putBytes failed");
        return false;
    }
    //preferences.end();
    return true;
}

bool GetData(const char* name, const char* key, void* value, const size_t len)
{
    //if( !preferences.begin(name,false) )             return false;
    if( preferences.getBytes(key, value, len) == 0)  return false;
    //preferences.end();
    return true;
}
