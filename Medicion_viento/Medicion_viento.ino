//anemómetro
float lectura;
float volt;
void setup() {
  Serial.begin(9600);
  pinMode(A15,INPUT);
}
void loop() {
  lectura = analogRead(A15);
  volt = lectura /1023 * 5.0;
  Serial.println(volt);

  delay(500);
}
