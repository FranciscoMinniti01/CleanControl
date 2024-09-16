#include "utils.h"

Preferences preferences;

bool Utils::guardarDato(const char* key, const void* value, const size_t length) {
    preferences.begin("storage", false); // "storage" es el espacio de nombres para guardar los datos
    bool resultado = preferences.putBytes(key, value, length); // Guardar el dato en NVS
    preferences.end();
    return resultado;
}

bool Utils::cargarDato(const char* key, void* value, const size_t length) {
    preferences.begin("storage", true); // "storage" es el espacio de nombres para cargar los datos
    size_t bytesLeidos = preferences.getBytes(key, value, length); // Cargar el dato de NVS
    preferences.end();
    return (bytesLeidos > 0);
}
