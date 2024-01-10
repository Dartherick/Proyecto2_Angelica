// Incluimos librería
#include <Adafruit_Sensor.h>
#include <DHT.h>
 
// Defining a pin for data
#define DHTPIN A14

// Defining what type of sensor I'm using
#define DHTTYPE DHT11
 
// Inizialized sensor instance
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  // Inizialited serial comunication
  Serial.begin(9600);
 
  // Starting sensor DHT
  dht.begin();
 
}
 
void loop() {
  delay(1000);

  // Reading humidity
  float h = dht.readHumidity();
  // Reading temperature in C (By default)
  float t_c = dht.readTemperature();
  // Reading temperature in Fahreheit
  float t_f = dht.readTemperature(true);
 
  // Checking in reading values are correct or valid.
  if (isnan(h) || isnan(t_c) || isnan(t_f)) {
    Serial.println("Error while reading DHT Sensor");
  }
 
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t_c);
  Serial.print(" *C ");
  Serial.print(t_f);
  Serial.print(" *F\n");
}
