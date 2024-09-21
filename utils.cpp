#include "utils.h"

Preferences preferences;

bool ClearSpace(const char* name)
{
    preferences.begin(name,false);
    return preferences.clear();
}

bool PutData(const char* name, const char* key, void* value, const size_t len)
{
    if( !preferences.begin(name,false) )             return false;
    if( preferences.putBytes(key, value, len) == 0)  return false;
    preferences.end();
    return true;
}

bool GetData(const char* name, const char* key, void* value, const size_t len)
{
    if( !preferences.begin(name,false) )             return false;
    if( preferences.getBytes(key, value, len) == 0)  return false;
    preferences.end();
    return true;
}
