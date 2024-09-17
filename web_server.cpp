/*#include "web_server.h"

WebServerManager::WebServerManager() : servidor(80), dnsServer() { }

bool WebServerManager::iniciar(CredencialesCallback callback) {
    callbackCredenciales = callback; // Almacena la callback para usarla después

    // Inicia el servidor DNS para redirigir todas las solicitudes a la página cautiva
    dnsServer.start(53, "*", WiFi.softAPIP());

    // Configura el servidor web para manejar las rutas
    servidor.on("/", HTTP_GET, std::bind(&WebServerManager::handleRoot, this));
    servidor.on("/guardar", HTTP_POST, std::bind(&WebServerManager::handleCredentials, this));
    servidor.begin();

    return true; // Retorna true para indicar que el servidor se inició correctamente
}

void WebServerManager::manejarServidor() {
    dnsServer.processNextRequest(); // Maneja las solicitudes DNS
    servidor.handleClient(); // Maneja las solicitudes HTTP del cliente
}

void WebServerManager::handleRoot() {
    // Responde con la página HTML para ingresar las credenciales
    String pagina = "<html><body><form action='/guardar' method='post'>"
                    "SSID: <input type='text' name='ssid'><br>"
                    "Password: <input type='password' name='password'><br>"
                    "<input type='submit' value='Guardar'></form></body></html>";
    servidor.send(200, "text/html", pagina);
}

void WebServerManager::handleCredentials() {
    // Procesa las credenciales enviadas por el cliente
    if (servidor.hasArg("ssid") && servidor.hasArg("password")) {
        String ssid = servidor.arg("ssid");
        String password = servidor.arg("password");
        
        // Llama a la callback para entregar las credenciales
        if (callbackCredenciales) {
            callbackCredenciales(ssid.c_str(), password.c_str());
        }
        
        // Muestra un mensaje de éxito
        servidor.send(200, "text/html", "<html><body><h2>Credenciales guardadas. Conectando...</h2></body></html>");
    } else {
        servidor.send(400, "text/html", "<html><body><h2>Error: Credenciales no válidas.</h2></body></html>");
    }
}
*/