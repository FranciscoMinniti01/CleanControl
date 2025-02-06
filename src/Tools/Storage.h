#ifndef STORAGE_H
#define STORAGE_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include <Preferences.h>

// DEFINES ----------------------------------------------------------------------------------------------------

#define NAME_SPACE "storage"

// VARIABLES ----------------------------------------------------------------------------------------------------

typedef struct
{
  void*     data;
  size_t    length;
  String    key;
} storage_t;

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void set_storage(storage_t* s, void* d, size_t l, String k);
bool get_storage(storage_t* s);
bool save_storage(storage_t* s);

// ----------------------------------------------------------------------------------------------------

#endif//STORAGE_H