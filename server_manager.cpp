// INCLUDES ----------------------------------------------------------------------------------------------------

#include "server_manager.h"

// VARIABLES ----------------------------------------------------------------------------------------------------

char ssids[MAX_CREDENCIALES][MAX_LEN_CREDENCIALES];
char passwords[MAX_CREDENCIALES][MAX_LEN_CREDENCIALES];

// FUNCTIONS ----------------------------------------------------------------------------------------------------

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

    for(uint8_t i = 1; i<=MAX_CREDENCIALES; i++)
    {
        html =+ "            <p>SSID " + std::to_string(i) + " actual: " + ssids[i-1] + "</p>";
        html =+ "            <label>SSID " + std::to_string(i) + ":</label>";
        html =+ "            <input type=\"text\" name=\"ssid" + std::to_string(i) + "\">";

        html =+ "            <p>Password " + std::to_string(i) + " actual: " + passwords[i-1] + "</p>";            
        html =+ "            <label>Password " + std::to_string(i) + ":</label>";
        html =+ "            <input type=\"password\" name=\"password" + std::to_string(i) + "\">";
    }

    html =+ "            <h2>Configuration</h2>";

    //Parametrizacion especial
    html =+ "            <p>Current machine ID: " + machine_id + "</p>";            
    html =+ "            <label>Machine ID:</label>";
    html =+ "            <input type=\"text\" name=\"machineid\">";

    html =+ "            <p>Current client ID: " + client_id + "</p>";            
    html =+ "            <label>Client ID:</label>";
    html =+ "            <input type=\"text\" name=\"clientid\">";

    html =+ R"rawliteral(            
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
    if(server.arg("ssid"+std::to_string(i+1))) ssids[i] = server.arg("ssid"+std::to_string(i+1));
    if(server.arg("password"+std::to_string(i+1))) passwords[i] = server.arg("password"+std::to_string(i+1));
  }   
  
  //Parametrizacion especial
  if(server.arg(machineid)) machine_id = server.arg("machineid");
  if(server.arg(clientid)) client_id = server.arg("clientid");

  // ACA TENGO QUE ALMACENAR LA IMFORMACION QUE ENTRA EN MEMORIA NO VOLATIL FRAN

  //server.send(200, "text/html", "<h3>Datos recibidos correctamente</h3>");
  server.send(200, "text/html", html);
}