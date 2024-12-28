// INCLUDES ----------------------------------------------------------------------------------------------------

#include "server_manager.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

extern WebServer server;

static user_param_t user_param;
storage_t storage_param[NUM_SPECIAL_PARAM];

// FUNCTIONS ROOT ----------------------------------------------------------------------------------------------------

void FormRoot()
{
  Serial.printf("DEBUG: FormRoot()\n");
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
              form {
                  background-color: #fff;
                  padding: 20px;
                  border-radius: 8px;
                  box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                  max-width: 400px;
                  width: 100%;
                  margin: 20px;
              }
              label {
                  display: block;
                  margin-bottom: 5px;
                  font-weight: bold;
              }
              input[type="text"], input[type="password"] {
                  width: 100%;
                  padding: 10px;
                  margin-bottom: 15px;
                  border: 1px solid #ccc;
                  border-radius: 5px;
                  font-size: 16px;
                  color: #555;
              }
              input[type="text"]:focus,
              input[type="password"]:focus {
                  border-color: #4CAF50;
                  outline: none;
              }
              .toggle-password {
                  display: block;
                  margin-top: -30px;
                  margin-bottom: 10px;
                  text-align: right;
                  cursor: pointer;
                  color: #4CAF50;
                  font-size: 14px;
              }
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
          <script>
              // Alternar visibilidad de la contraseña
              function togglePassword(inputId, toggleId) {
                  const input = document.getElementById(inputId); // Campo de contraseña
                  const toggle = document.getElementById(toggleId); // Botón
                  if (input.type === "password") {
                      input.type = "text"; // Mostrar contraseña
                      toggle.textContent = "Ocultar contraseña"; // Cambiar texto del botón
                  } else {
                      input.type = "password"; // Ocultar contraseña
                      toggle.textContent = "Mostrar contraseña"; // Cambiar texto del botón
                  }
              }
          </script>
      </head>
      <body>
          <form action="/submit" method="POST">
              <h2>WiFi Credentials</h2>
  )rawliteral";

  credentials_t* credentials = get_credentials();

  for (uint8_t i = 1; i <= MAX_CREDENCIALES; i++)
  {
    html += "            <label>SSID " + String(i) + ":</label>";
    html += "            <input type=\"text\" name=\"ssid" + String(i) + "\" value=\"" + String(credentials[i - 1].ssid) + "\">";

    html += "            <label>Password " + String(i) + ":</label>";
    html += "            <input type=\"password\" id=\"password" + String(i) + "\" name=\"password" + String(i) + "\" value=\"" + String(credentials[i - 1].password) + "\">";
    html += "            <span class=\"toggle-password\" id=\"togglePassword" + String(i) + "\" onclick=\"togglePassword('password" + String(i) + "', 'togglePassword" + String(i) + "')\">Mostrar contraseña</span>";
  }

  html += "            <h2>Configuration</h2>";

  // USER PARAMETERS --------------------------------------------------
  html += "            <label>Machine ID:</label>";
  html += "            <input type=\"text\" name=\"machineid\" value=\"" + user_param.machine_id + "\">";
  // --------------------------------------------------
  html += "            <label>Client ID:</label>";
  html += "            <input type=\"text\" name=\"clientid\" value=\"" + user_param.client_id + "\">";
  // --------------------------------------------------

  html += R"rawliteral(            
              <input type="submit" value="Enviar">
              <input type="button" value="Cancelar" style="background-color: #f44336;" onclick="location.reload();">
          </form>
      </body>
      </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void FormSubmitRoot()
{
  Serial.printf("DEBUG: FormSubmitRoot()\n");

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

  // FRAN PROBAR EL METODO .hasArg nota: Arg siempre devuelve string y hasArg bool
  for(uint8_t i = 0; i<MAX_CREDENCIALES; i++)
  {
    if(server.arg("ssid"+String(i+1)))
    {
      if(server.arg("password"+String(i+1))) Serial.printf("INFO: Credencial %d ingresada\n",i+1);
      else Serial.printf("INFO: Password %d vacia\n",i+1);

      set_credentials(server.arg("ssid"+String(i+1)),server.arg("password"+String(i+1)));
    }
    else Serial.printf("INFO: Ssid %d vacia\n",i+1);       
  }   
  
  // USER PARAMETERS --------------------------------------------------
  if(server.arg("machineid"))
  {
    user_param.machine_id = server.arg("machineid");
    Serial.printf("INFO: Machine ID ingresada: %s\n",user_param.machine_id);
    if(! seve_data(&storage_param[INDEX_MACHINE_ID])) Serial.println("EROOR: Special param not save");
  }
  else Serial.printf("INFO: Machine ID vacia\n");
  // --------------------------------------------------
  if(server.arg("clientid"))  
  {
    user_param.client_id  = server.arg("clientid");
    Serial.printf("INFO: Client ID ingresada: %s\n",user_param.client_id);
    if(! seve_data(&storage_param[INDEX_CLIENT_ID])) Serial.println("EROOR: Special param not save");
  }
  else Serial.printf("INFO: Client ID vacia\n");
  // --------------------------------------------------

  server.send(200, "text/html", html);
}

// FUNCTIONS ----------------------------------------------------------------------------------------------------

void server_init()
{
  Serial.printf("DEBUG: server_init()\n");

  // ROOT --------------------------------------------------
  set_hdmi_root("/",HTTP_GET,FormRoot);
  // --------------------------------------------------
  set_hdmi_root("/submit",HTTP_POST,FormSubmitRoot);
  // --------------------------------------------------

  // USER PARAMETERS --------------------------------------------------
  set_data_storage( &storage_param[INDEX_MACHINE_ID],
                    (void*)user_param.machine_id.c_str(),
                    sizeof(user_param.machine_id),
                    KEY_SPECIAL_PARAM + String(INDEX_MACHINE_ID) );
  if(!get_data( &storage_param[INDEX_MACHINE_ID] )) user_param.machine_id = "";
  // --------------------------------------------------
  set_data_storage( &storage_param[INDEX_CLIENT_ID],
                    (void*)user_param.client_id.c_str(), 
                    sizeof(user_param.client_id),
                    KEY_SPECIAL_PARAM + String(INDEX_CLIENT_ID) );
  if(!get_data( &storage_param[INDEX_CLIENT_ID] )) user_param.client_id = "";
  // --------------------------------------------------
}

user_param_t* get_special_param()
{ 
  return &user_param;
}

// ----------------------------------------------------------------------------------------------------
