// defines pins

int stepPin5 = 3; //define Pulse pin
int dirPin5 = 2; //define Direction pin
//int enPin5 = 4; //define Enable Pin

//float pasos = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin5,OUTPUT); 
  pinMode(dirPin5,OUTPUT);
  //pinMode(enPin5,OUTPUT);
}
void loop() {

  // Makes 400 pulses for making two full cycle rotation
  digitalWrite(dirPin5,HIGH); //Changes the rotations direction
  for(int x = 0; x < 1000; x++){
    //digitalWrite(enPin5,LOW);
    digitalWrite(stepPin5,HIGH);
    delay(1);
    digitalWrite(stepPin5,LOW);
    delay(1);
  }
  delay(100);
}
