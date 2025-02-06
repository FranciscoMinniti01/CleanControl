#include <Arduino.h>
#include "../../src/Tools/Storage.h"
#include "../../src/Tools/TimerManager.h"
#include "../../src/Tools/Storage.cpp"
#include "../../src/Tools/TimerManager.cpp"

// Variables para Storage
storage_t storageTest;
uint32_t storedValue  = 0;
String storageKey     = "testKey";
#define TEST_VALUE    45

// Variables para TimerManager
my_timer_t testTimer;

// Callback para el temporizador
void timer_cb()
{
  Serial.println("Timer ejecutado!");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n\n\n\n\n==================== Iniciando Prueba de Storage ====================");

  storedValue = TEST_VALUE;

  // Configurar almacenamiento
  set_storage(&storageTest, &storedValue, sizeof(storedValue), storageKey);
  
  // Guardar en NVS
  if (save_storage(&storageTest)) Serial.println("Datos guardados correctamente.");
  else Serial.println("Error al guardar datos.");

  storedValue = 0;  // Reset de valor para validar lectura

  // Leer de NVS
  if (get_storage(&storageTest)) Serial.println("Dato recuperado correctamente");
  else Serial.println("Error al recuperar datos.");

  if(storedValue == TEST_VALUE) Serial.printf("Prueba exitosa: %D = %D\n", storedValue, TEST_VALUE);
  else Serial.printf("Prueba fallida: %D =! %D\n", storedValue, TEST_VALUE);

  delay(2500);

  Serial.println("==================== Iniciando Prueba de TimerManager ====================");

  if (!timer_init()) {
      Serial.println("Error al iniciar el TimerManager.");
      return;
  }
  set_timer(&testTimer, TIME_10S, timer_cb);
}

void loop()
{
  // Verificar si el temporizador se activó
  if (get_flag_timer(&testTimer)) Serial.println("Temporizador alcanzó el tiempo esperado.");
  delay(500);
}
