// INCLUDE ----------------------------------------------------------------------------------------------------

#include "Storage.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

Preferences preferences;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void set_storage(storage_t* s, void* d, size_t l, String k)
{
  if(s == NULL) return;
  if(d == NULL) return;
  s->data   = d;
  s->length = l;
  s->key    = k;
}

bool get_storage(storage_t* s)
{
  bool flag = true;

  if(s == NULL) return false;

  if( !preferences.begin(NAME_SPACE) )
  { 
    Serial.println("ERROR: Get preferences begin failed");
    return false;
  }

  if( !preferences.getBytes( s->key.c_str(), s->data, s->length ) )
  {
    char* char_ptr = (char*)s->data;
    for(uint8_t i = 0; i<s->length ; i++) char_ptr[i] = 0;
    flag = false;
  }

  preferences.end();

  return flag;
}

bool save_storage(storage_t* s)
{
  bool flag = true;

  if(s == NULL) return false;

  if( !preferences.begin(NAME_SPACE) )
  { 
    Serial.println("ERROR: Save preferences begin failed");
    return false;
  }

  if( !preferences.putBytes( s->key.c_str(), s->data, s->length ) )
  {
    Serial.println("ERROR: Preferences putBytes failed");
    flag = false;
  }

  preferences.end();
  
  return flag;
}

// ----------------------------------------------------------------------------------------------------
