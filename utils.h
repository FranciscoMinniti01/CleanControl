#ifndef UTILS_H
#define UTILS_H

#include <Preferences.h>

bool ClearSpace(const char* name);
void init_utils(const char* name);
bool PutData(const char* name, const char* key, void* value, const size_t len);
bool GetData(const char* name, const char* key, void* value, const size_t len);

#endif // UTILS_H
