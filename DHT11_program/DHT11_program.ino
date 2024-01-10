#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2     // El pin donde está conectado el sensor
#define DHTTYPE DHT11   // Definimos el tipo de sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Leyendo datos del sensor DHT11...");
  dht.begin();
}

void loop() {
  // Leemos la temperatura en grados Celsius
  float temperature = dht.readTemperature();
  
  // Leemos la humedad relativa
  float humidity = dht.readHumidity();
  
  // Verificamos si la lectura fue exitosa
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error al leer datos del sensor DHT11");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.print(" °C - Humedad: ");
    Serial.print(humidity);
    Serial.println("%");
  }
  
  delay(2000); // Esperamos 2 segundos entre lecturas
}
