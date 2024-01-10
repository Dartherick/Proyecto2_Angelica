#include <ezButton.h>

ezButton limitSwitchY1(41);  // create ezButton object that attach to pin 32 OUT;
ezButton limitSwitchY2(40);  // create ezButton object that attach to pin 32 IN;

// defines pins BASE TOMA BATERÍAS

int stepPin8 = 18; //define Pulse pin
int dirPin8 = 17; //define Direction pin

//float pasos = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin8,OUTPUT); 
  pinMode(dirPin8,OUTPUT);
  //pinMode(enPin8,OUTPUT);

  Serial.begin(9600);
  limitSwitchY1.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitchY2.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  Y_IN();
}

void Y_OUT() {
  limitSwitchY1.loop(); // MUST call the loop() function first

  if(limitSwitchY1.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchY1.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

  int state = limitSwitchY1.getState();
  if(state == HIGH){
    digitalWrite(dirPin8,LOW); //Changes the rotations direction
    digitalWrite(stepPin8,HIGH);
    delay(2);
    digitalWrite(stepPin8,LOW);
    delay(2);
  }
  else{
    digitalWrite(dirPin8,LOW); //Changes the rotations direction
    digitalWrite(stepPin8,LOW);
    delay(2);
    digitalWrite(stepPin8,LOW);
    delay(2);
  }
}

void Y_IN() {
  limitSwitchY2.loop(); // MUST call the loop() function first

  if(limitSwitchY2.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchY2.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

  int state = limitSwitchY2.getState();
  if(state == HIGH){
    digitalWrite(dirPin8,HIGH); //Changes the rotations direction
    digitalWrite(stepPin8,HIGH);
    delay(2);
    digitalWrite(stepPin8,LOW);
    delay(2);
  }
  else{
    digitalWrite(dirPin8,HIGH); //Changes the rotations direction
    digitalWrite(stepPin8,LOW);
    delay(2);
    digitalWrite(stepPin8,LOW);
    delay(2);
  }
}
