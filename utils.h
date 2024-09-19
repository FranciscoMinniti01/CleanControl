#ifndef UTILS_H
#define UTILS_H

#include <Preferences.h>

bool init_utils();
bool CreatSpace(const char* name, const size_t length);
bool guardarDato(const char* key, const void* value, const size_t length);
bool cargarDato(const char* key, const void* value, const size_t length);

#endif // UTILS_H
