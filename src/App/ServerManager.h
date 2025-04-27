#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

// INCLUDES -------------------------------------------------------------------------------------------

#include <string>
#include "Print.h"
#include "HardwareSerial.h"

#include <WebServer.h>

#include "../../src/Services/WifiManager.h"


// DEFINES --------------------------------------------------------------------------------------------

#define KEY_SPECIAL_PARAM     "SP"

// USER PARAMETERS --------------------------------------------------
#define NUM_SPECIAL_PARAM     2
#define INDEX_MACHINE_ID      0
#define INDEX_CLIENT_ID       1

typedef struct {
  String machine_id = "";
  String client_id  = "";
}user_param_t;
// --------------------------------------------------


// FUNCTIONS ------------------------------------------------------------------------------------------ 

void ServerManagerInit();
user_param_t* GetConfigParam();


// ----------------------------------------------------------------------------------------------------

#endif//SERVER_MANAGER_H