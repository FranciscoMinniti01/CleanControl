#ifndef UTILS_H
#define UTILS_H

#include <Preferences.h>

//----------------------------------------------------------------------------------------------------

#define DEBUG_UTILS

//----------------------------------------------------------------------------------------------------

bool nsv_PrepareSpace(const char* name);
bool nsv_CloseSpace();
bool nsv_PutData(const char* key, void* value, const size_t len);
bool nsv_GetData(const char* key, void* value, const size_t len);

#endif // UTILS_H
