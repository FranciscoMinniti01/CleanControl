// INCLUDE ----------------------------------------------------------------------------------------------------

#include "storage.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

storage_t storage[MAX_NUM_STORAGE];
Preferences preferences;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

int16_t set_data(void* data, size_t len)
{
  if(data != NULL && len != 0)
  {
    for(int8_t i = 0 ; i<MAX_NUM_STORAGE ; i++)
    {
      if(storage[i].index < 0)
      {
        storage[i].len = len;
        storage[i].data = data;
        storage[i].index = i;
        return i;
      }
    }
  }
  return -1;
}

bool get_data(int16_t index)
{
  if( !preferences.begin(NAME_SPACE) ) Serial.println("ERROR: Preferences begin failed");

  if( !preferences.getBytes( (std::string(KEY_NAME) + std::to_string(index)).c_str(), storage[index].data, storage[index].len))
  {
    return false;
    storage[index].data = NULL;
  }
  preferences.end();
  return true;
}

bool seve_data(int16_t index)
{
  if( !preferences.begin(NAME_SPACE) ) Serial.println("ERROR: Preferences begin failed");

  if( !preferences.putBytes( (std::string(KEY_NAME) + std::to_string(index)).c_str(), storage[index].data, storage[index].len))
  {
    return false;
  }
  preferences.end();
  return true;
}

// ----------------------------------------------------------------------------------------------------
