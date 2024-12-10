// INCLUDES ----------------------------------------------------------------------------------------------------

#include "server_manager.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

extern WebServer server;

sv_param_t special_params;
storage_t storage_param[NUM_SPECIAL_PARAM];

// FUNCTIONS ROOT ----------------------------------------------------------------------------------------------------

void FormRoot()
{
    String html = R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Formulario de Configuración</title>
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <style>
                /* Estilos generales */
                body {
                    font-family: Arial, sans-serif;
                    font-size: 18px;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 100vh;
                    margin: 0;
                    background-color: #f4f4f9;
                }
                h2 {
                    text-align: center;
                    color: #333;
                }
                /* Estilos para el formulario */
                form {
                    background-color: #fff;
                    padding: 20px;
                    border-radius: 8px;
                    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                    max-width: 400px;
                    width: 100%;
                    margin: 20px;
                }
                /* Estilos para las etiquetas y entradas del formulario */
                label {
                    display: block;
                    margin-bottom: 10px;
                    font-weight: bold;
                }
                input[type="text"], input[type="number"], input[type="password"] {
                    width: 100%;
                    padding: 10px;
                    margin-bottom: 15px;
                    border: 1px solid #ccc;
                    border-radius: 5px;
                    font-size: 16px;
                }
                /* Botón de envío */
                input[type="submit"] {
                    background-color: #4CAF50;
                    color: white;
                    padding: 10px 20px;
                    border: none;
                    border-radius: 5px;
                    cursor: pointer;
                    font-size: 18px;
                    width: 100%;
                }
                input[type="submit"]:hover {
                    background-color: #45a049;
                }
            </style>
        </head>
        <body>
            <form action="/submit" method="POST">
                <h2>-</h2>
                <h2>-</h2>
                <h2>-</h2>
                <h2>-</h2>
                <h2>WiFi Credentials</h2>
    )rawliteral";

    credentials_t* credentials = get_credentials();

    for(uint8_t i = 1; i<=MAX_CREDENCIALES; i++)
    {
        html += "            <p>SSID " + String(i) + " actual: " + String(credentials[i - 1].ssid) + "</p>";
        html += "            <label>SSID " + String(i) + ":</label>";
        html += "            <input type=\"text\" name=\"ssid" + String(i) + "\">";

        html += "            <p>Password " + String(i) + " actual: " + String(credentials[i - 1].password) + "</p>";            
        html += "            <label>Password " + String(i) + ":</label>";
        html += "            <input type=\"password\" name=\"password" + String(i) + "\">";
    }

    html += "            <h2>Configuration</h2>";

    // SPECIAL PARAMETERS ----------------------------------------
    html += "            <p>Current machine ID: " + special_params.machine_id + "</p>";            
    html += "            <label>Machine ID:</label>";
    html += "            <input type=\"text\" name=\"machineid\">";
    // ----------------------------------------
    html += "            <p>Current client ID: " + special_params.client_id + "</p>";            
    html += "            <label>Client ID:</label>";
    html += "            <input type=\"text\" name=\"clientid\">";
    // ----------------------------------------

    html += R"rawliteral(            
                <input type="submit" value="Enviar">
            </form>
        </body>
        </html>
    )rawliteral";

    server.send(200, "text/html", html);
}

void FormSubmitRoot()
{
  String html = R"rawliteral(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Confirmación</title>
            <style>
                /* Estilos generales */
                body {
                    font-family: Arial, sans-serif;
                    font-size: 18px;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 100vh;
                    margin: 0;
                    background-color: #f4f4f9;
                }
                h3 {
                    text-align: center;
                    color: #333;
                }
                .message-box {
                    background-color: #fff;
                    padding: 20px;
                    border-radius: 8px;
                    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                    max-width: 400px;
                    width: 100%;
                    margin: 20px;
                    text-align: center;
                }
            </style>
        </head>
        <body>
            <div class="message-box">
                <h3>Datos recibidos correctamente</h3>
                <p>Gracias por enviar sus datos.</p>
            </div>
        </body>
        </html>
  )rawliteral";

  for(uint8_t i = 0; i<MAX_CREDENCIALES; i++)
  {
    if(server.arg("ssid"+String(i+1)))
    {
      if(!server.arg("password"+String(i+1)))
      {
        #ifdef DEBUG_SERVER
        Serial.printf("INFO: Password %d vacia\n",i+1);
        #endif
      }
      if(set_credentials(server.arg("ssid"+String(i+1)),server.arg("password"+String(i+1))))
      {
        #ifdef DEBUG_SERVER
        Serial.printf("INFO: Credencial %d guardada con exito\n", i + 1);
        #endif
      }
      else Serial.printf("EROOR: Credencial %d no se guardo\n",i+1);
    }      
  }   
  
  // SPECIAL PARAMETERS ----------------------------------------
  if(server.arg("machineid"))
  {
    special_params.machine_id = server.arg("machineid");
    if(! seve_data(&storage_param[INDEX_MACHINE_ID])) Serial.println("EROOR: Special param not save");
    is_updated = true;
  }
  // ----------------------------------------
  if(server.arg("clientid"))  
  {
    special_params.client_id  = server.arg("clientid");
    if(! seve_data(&storage_param[INDEX_CLIENT_ID])) Serial.println("EROOR: Special param not save");
    is_updated = true;
  }
  // ----------------------------------------

  //server.send(200, "text/html", "<h3>Datos recibidos correctamente</h3>");
  server.send(200, "text/html", html);
}

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void server_init()
{
  // ROOT ----------------------------------------
  set_hdmi_root("/",HTTP_GET,FormRoot);
  // ----------------------------------------
  set_hdmi_root("/submit",HTTP_POST,FormSubmitRoot);
  // ----------------------------------------

  // SPECIAL PARAMETERS ----------------------------------------
  storage_param[INDEX_MACHINE_ID].data = (void*)special_params.machine_id.c_str();
  storage_param[INDEX_MACHINE_ID].len = sizeof(special_params.machine_id);
  storage_param[INDEX_MACHINE_ID].key = KEY_SPECIAL_PARAM + String(INDEX_MACHINE_ID);
  get_data(&storage_param[INDEX_MACHINE_ID]);
  // ----------------------------------------
  storage_param[INDEX_CLIENT_ID].data = (void*)special_params.client_id.c_str();
  storage_param[INDEX_CLIENT_ID].len = sizeof(special_params.client_id);
  storage_param[INDEX_CLIENT_ID].key = KEY_SPECIAL_PARAM + String(INDEX_CLIENT_ID);
  get_data(&storage_param[INDEX_CLIENT_ID]);
  // ----------------------------------------
}

sv_param_t* get_special_param()
{ 
  return &special_params;
}


