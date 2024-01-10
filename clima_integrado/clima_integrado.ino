// Incluimos librería
#include <Adafruit_Sensor.h>
#include <DHT.h>
 
// Defining a pin for data
#define DHTPIN A14

// Defining what type of sensor I'm using
#define DHTTYPE DHT11

// Defining wind pin data
#define WINDPIN A15
 
// Inizialized sensor instance
DHT dht(DHTPIN, DHTTYPE);

//anemómetro
float lectura;
float volt;

//clima
float h;    // Reading humidity
float t_c;  // Reading temperature in C (By default)


void setup() {
  // Inizialited serial comunication
  Serial.begin(9600);
 
  // Starting sensor DHT
  dht.begin();
 
  //anemometro
  pinMode(WINDPIN,INPUT);
}
 
void loop() {
  delay(1000);
  Clima();
  delay(1000);
  Viento();
}

void Clima(){
  // Reading humidity
  h = dht.readHumidity();
  // Reading temperature in C (By default)
  t_c = dht.readTemperature();
 
  // Checking in reading values are correct or valid.
  if (isnan(h) || isnan(t_c)) {
    Serial.println("Error while reading DHT Sensor");
  }
  else{
    SendFloatMessage(h,"8");
    SendFloatMessage(t_c,"9");
  }
}

void Viento() {
  lectura = analogRead(WINDPIN);
  volt = lectura /1023 * 5.0;
  //Serial.println(volt);
  SendFloatMessage(volt,"11");
}

void SendFloatMessage(float Float,char Function[3]){
  char message[15];
  char floatStr[10];

  dtostrf(Float, 2, 2, floatStr);
  sprintf(message,"<%s%s>",Function,floatStr); // Create the message in the desired structure
  Serial.println(message);  // Send the complete message
}

void SendStrMessage(char message[10]){
  char FormattedMessage[10];
  sprintf(FormattedMessage, "<%s>", message);  // Combine the float string with "<" and ">"
  Serial.println(FormattedMessage);  // Send the complete message
}
