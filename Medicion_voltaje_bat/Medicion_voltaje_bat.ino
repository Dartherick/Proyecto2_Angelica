float lectura;
float volt;
void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
}
void loop() {
  lectura = analogRead(A0);
  volt = lectura /1023 * 5.0;
  Serial.println(volt);
}
