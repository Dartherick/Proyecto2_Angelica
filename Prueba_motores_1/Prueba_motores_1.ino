// Codigo motor de simple efecto driver 1 misfit tech perfil 3
// defines pins

int enPin3= 10; //define Enable pin
int stepPin3= 9; //define Pulse pin
int dirPin3= 8; //define Direction pin
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin3,OUTPUT); 
  pinMode(dirPin3,OUTPUT);
  pinMode(enPin3,OUTPUT);
}
void loop() {
  //delayMicroseconds(5000);
  int pasostotal3 = 9100; // 910 pasos es 1cm
  
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < pasostotal3; x++) {
    digitalWrite(dirPin3,HIGH); //HIGH HACIA EL CENTRO
    digitalWrite(enPin3,LOW);
    digitalWrite(stepPin3,HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin3,LOW);
    delayMicroseconds(1000);
  }
  delay(5000);
}
