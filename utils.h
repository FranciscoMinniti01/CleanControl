#ifndef UTILS_H
#define UTILS_H

#include <Preferences.h>

class Utils {
public:
    static bool guardarDato(const char* key, const void* value, const size_t length);
    static bool cargarDato(const char* key, void* value, const size_t length);
};

#endif // UTILS_H
