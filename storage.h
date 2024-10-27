#ifndef STORAGE_H
#define STORAGE_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include <Preferences.h>
#include "config.h"

// DEFINES ----------------------------------------------------------------------------------------------------

#define NAME_SPACE    "storage"
#define KEY_NAME      "D"

// VARIABLES ----------------------------------------------------------------------------------------------------

typedef struct {
    void*    data;
    size_t   len;
    int16_t  index = -1;
} storage_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

int16_t set_data(void* data, size_t len);
bool gat_data(int16_t index);
bool seve_data(int16_t index);

// ----------------------------------------------------------------------------------------------------

#endif//STORAGE_H