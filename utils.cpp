#include "utils.h"

Preferences preferences;

bool nsv_PrepareSpace(const char* name)
{
    if(!preferences.begin(name) )
    {
        #if !defined(DEBUG) && !defined(DEBUG_UTILS)
        Serial.println("DEBUG: open space failed");
        #endif
        return false;
    }
    return true;
}

bool nsv_CloseSpace()
{
    preferences.end();
}

bool nsv_PutData(const char* key, void* value, const size_t len)
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

bool nsv_GetData(const char* key, void* value, const size_t len)
{
    //if( !preferences.begin(name,false) )             return false;
    if( preferences.getBytes(key, value, len) == 0)  return false;
    //preferences.end();
    return true;
}
