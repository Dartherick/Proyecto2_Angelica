// Incluimos librería ///////////ENVIAR DATOS DESPUÉS DE CADA PASO DE LA SECUENCIA EN EL MAIN
#include <Adafruit_Sensor.h>
#include <DHT.h>
 
// Defining a pin for data
#define DHTPIN A14

// Defining what type of sensor I'm using
#define DHTTYPE DHT11
 
// Inizialized sensor instance DHT
DHT dht(DHTPIN, DHTTYPE);

//anemómetro
float lectura;
float volt;

char humedad;
char temp_c;
char volt_vel;
 
void setup() {
  // Inizialited serial comunication
  Serial.begin(9600);
 
  // Starting sensor DHT
  dht.begin();

  // Pin de lectura de anemometer
  pinMode(A15,INPUT);
 
}

void loop() {
  dth();
  sendMessage(humedad);
  sendMessage(temp_c);
  anemometer();
  sendMessage(volt_vel);
}
 
void dth() {
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

  dtostrf(h, 5, 2, humedad);
  dtostrf(t_c, 5, 2, temp_c);
  return humedad;
  return temp_c;

  delay(1000);
}

void anemometer() {
  lectura = analogRead(A15);
  volt = lectura /1023 * 5.0;
  Serial.println(volt);
  dtostrf(volt, 4, 2, volt_vel);
  return volt_vel;

  delay(500);
}

void sendMessage(const char* message) {
  // Create a buffer to store the formatted message
  char Formatted_Message[10];

  Formatted_Message[0] = '<';
  strcpy(Formatted_Message + 1, message);
  Formatted_Message[strlen(message) + 1] = '>';
  Formatted_Message[strlen(message) + 2] = '\0'; // Null-terminate the string

  // Send the formatted message via serial communication
  Serial.write(Formatted_Message);
}
