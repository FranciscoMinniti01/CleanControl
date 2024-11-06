// INCLUDE ----------------------------------------------------------------------------------------------------

#include "storage.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

Preferences preferences;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

bool get_data(storage_t* s)
{
  bool err = true;

  if( !preferences.begin(NAME_SPACE) ) { 
    Serial.println("ERROR: Preferences begin failed");
    return false;
  }

  if( !preferences.getBytes( (s->key).c_str(), s->data, s->len))
  {
    char* char_ptr = (char*)s->data;
    for(uint8_t i = 0; i<s->len ; i++) char_ptr[i] = 0;
    err = false;
  }

  preferences.end();
  return err;
}

bool seve_data(storage_t* s)
{
  bool err = true;

  if( !preferences.begin(NAME_SPACE) ) { 
    Serial.println("ERROR: Preferences begin failed");
    return false;
  }

  if( !preferences.putBytes( (s->key).c_str(), s->data, s->len))
  {
    Serial.println("ERROR: Preferences putBytes failed");
    err = false;
  }

  preferences.end();
  return err;
}

// ----------------------------------------------------------------------------------------------------
