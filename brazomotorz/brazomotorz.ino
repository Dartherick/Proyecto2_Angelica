// defines pins rack a dron

int stepPin7 = 4; //define Pulse pin
int dirPin7 = 29; //define Direction pin

//float pasos = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin7,OUTPUT);
  pinMode(dirPin7,OUTPUT);
  //pinMode(enPin5,OUTPUT);
}
void loop() {
  // Makes 400 pulses for making two full cycle rotation
   int pasostotal2 = 10000; //910 es 1cm 
  
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < pasostotal2; x++) {
    digitalWrite(dirPin7,LOW); //LOW HACIA EL CENTRO
    digitalWrite(stepPin7,HIGH);
    delayMicroseconds(300);
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
  delay(1);
}
}
