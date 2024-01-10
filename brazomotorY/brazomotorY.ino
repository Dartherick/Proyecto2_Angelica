// defines pins BASE TOMA BATERÍAS

int stepPin8 = 18; //define Pulse pin
int dirPin8 = 17; //define Direction pin

//float pasos = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin8,OUTPUT); 
  pinMode(dirPin8,OUTPUT);
  //pinMode(enPin8,OUTPUT);
}
void loop() {
  //delayMicroseconds(5000);

  // Makes 400 pulses for making two full cycle rotation

  for(int x = 0; x < 1000; x++){
    //digitalWrite(enPin8,LOW);
    digitalWrite(dirPin8,HIGH); //Changes the rotations direction
    digitalWrite(stepPin8,HIGH);
    delay(2);
    digitalWrite(stepPin8,LOW);
    delay(2);
  }
  delay(1000);
}
