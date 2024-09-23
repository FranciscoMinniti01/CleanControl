#include "utils.h"

bool nsv_PrepareSpace(Preferences* preferences_ ,const char* name)
{
    if(!(*preferences_).begin(name) )
    {
        #if !defined(DEBUG) && !defined(DEBUG_UTILS)
        Serial.println("DEBUG: open space failed");
        #endif
        return false;
    }
    Serial.println("DEBUG: begin");
    return true;
}

bool nsv_CloseSpace(Preferences* preferences_ )
{
    Serial.println("DEBUG: end");
    (*preferences_).end();
    Serial.println("DEBUG: end");
}

bool nsv_PutData(Preferences* preferences_, const char* key, void* value, const size_t len)
{
    Serial.print("PutData free entries:");
    Serial.println((*preferences_).freeEntries());
    
    /*if( !preferences.begin(name,false) ){
        Serial.println("DEBUG: begin failed");
        return false;
    }*/
    if( (*preferences_).putBytes(key, value, len) == 0){
        Serial.println("DEBUG: putBytes failed");
        return false;
    }
    //preferences.end();
    return true;
}

bool nsv_GetData(Preferences* preferences_, const char* key, void* value, const size_t len)
{
    //if( !preferences.begin(name,false) )             return false;
    if( (*preferences_).getBytes(key, value, len) == 0)  return false;
    //preferences.end();
    return true;
}
