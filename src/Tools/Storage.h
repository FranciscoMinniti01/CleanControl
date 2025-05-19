#ifndef STORAGE_H
#define STORAGE_H

// INCLUDE ----------------------------------------------------------------------------------------------------

#include <Preferences.h>

// DEFINES ----------------------------------------------------------------------------------------------------

#define NAME_SPACE "storage"

#define SET_STORAGE(s,d,k) \
  set_storage(&(s), (void*)&(d), sizeof(d), k);

#define GET_STORAGE(s) \
  get_storage(&(s));

#define SEVE_STORAGE(s) \
  save_storage(&(s));


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