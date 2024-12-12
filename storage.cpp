// INCLUDE ----------------------------------------------------------------------------------------------------

#include "storage.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

Preferences preferences;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void set_data_storage(storage_t* s, void* d, size_t l, String k)
{
  s->data = d;
  s->len  = l;
  s->key  = k;
}

bool get_data(storage_t* s)
{
  Serial.printf("DEBUG: get_data()\n");
  bool flag = true;
  if( !preferences.begin(NAME_SPACE) )
  { 
    Serial.println("ERROR: Get preferences begin failed");
    return false;
  }

  if( !preferences.getBytes( (s->key).c_str(), s->data, s->len))
  {
    char* char_ptr = (char*)s->data;
    for(uint8_t i = 0; i<s->len ; i++) char_ptr[i] = 0;
    flag = false;
  }

  preferences.end();
  return flag;
}

bool seve_data(storage_t* s)
{
  Serial.printf("DEBUG: seve_data()\n");
  bool flag = true;
  if( !preferences.begin(NAME_SPACE) ) { 
    Serial.println("ERROR: Save preferences begin failed");
    return false;
  }

  if( !preferences.putBytes( (s->key).c_str(), s->data, s->len))
  {
    Serial.println("ERROR: Preferences putBytes failed");
    flag = false;
  }

  preferences.end();
  return flag;
}

// ----------------------------------------------------------------------------------------------------
