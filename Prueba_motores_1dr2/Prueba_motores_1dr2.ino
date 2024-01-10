// Codigo motor de simple efecto driver 2 semart sc24
// defines pins

int stepPin4= 12; //define Pulse pin
int dirPin4= 11; //define Direction pin
int enPin4= 13; //define Enable Pin
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin4,OUTPUT); 
  pinMode(dirPin4,OUTPUT);
  pinMode(enPin4,OUTPUT);
}
void loop() {
  //delayMicroseconds(5000);
  int pasostotal4 = 910; // 910 pasos es 1cm
  
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < pasostotal4; x++) {
    digitalWrite(dirPin4,HIGH); //HIGH HACIA EL CENTRO
    digitalWrite(enPin4,LOW);
    digitalWrite(stepPin4,HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin4,LOW);
    delayMicroseconds(2000);
  }
  delay(5000);
}
