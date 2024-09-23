#ifndef UTILS_H
#define UTILS_H

#include <Preferences.h>

//----------------------------------------------------------------------------------------------------

#define DEBUG_UTILS

//----------------------------------------------------------------------------------------------------

bool nsv_PrepareSpace(Preferences* preferences_ ,const char* name);
bool nsv_CloseSpace(Preferences* preferences_);
bool nsv_PutData(Preferences* preferences_ ,const char* key, void* value, const size_t len);
bool nsv_GetData(Preferences* preferences_ ,const char* key, void* value, const size_t len);

#endif // UTILS_H
