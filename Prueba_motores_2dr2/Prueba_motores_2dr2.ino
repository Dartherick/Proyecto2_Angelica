// Codigo motor de doble efecto driver 2 semart sc24
// defines pins

int stepPin12= 15; //define Pulse pin
int dirPin12= 16; //define Direction pin
int enPin12= 14; //define Enable Pin

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin12,OUTPUT); 
  pinMode(dirPin12,OUTPUT);
  pinMode(enPin12,OUTPUT);
}
void loop() {
  delayMicroseconds(5000);
  int pasostotal2 = 9100*1.8; //910 es 1cm 
  
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < pasostotal2; x++) {
    digitalWrite(dirPin12,LOW); //LOW HACIA EL CENTRO
    digitalWrite(enPin12,LOW);
    digitalWrite(stepPin12,HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin12,LOW);
    delayMicroseconds(2000);
  }
  delay(5000);
}
