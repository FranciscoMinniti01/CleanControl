#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

// INCLUDES ----------------------------------------------------------------------------------------------------

#include <WebServer.h>

#include "wifi_manager.h"
#include "config.h"

// SPECIAL PARAMETERS ----------------------------------------------------------------------------------------------------

#define NUM_SPECIAL_PARAM     2

#define KEY_SPECIAL_PARAM     "SP"

#define INDEX_MACHINE_ID      0
#define INDEX_CLIENT_ID       1

typedef struct {
    String machine_id;
    String client_id;
} sv_param_t;

// FUNCTIONS ---------------------------------------------------------------------------------------------------- 

void server_init();
sv_param_t* get_special_param();

// ----------------------------------------------------------------------------------------------------

#endif//SERVER_MANAGER_H