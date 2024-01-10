#include <ezButton.h>

ezButton limitSwitchZ1(35);  // create ezButton object that attach to pin 32 DRON;
ezButton limitSwitchZ2(36);  // create ezButton object that attach to pin 32 RACK;

// defines pins rack a dron

int stepPin7 = 4; //define Pulse pin
int dirPin7 = 29; //define Direction pin

//float pasos = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin7,OUTPUT);
  pinMode(dirPin7,OUTPUT);
  //pinMode(enPin5,OUTPUT);

  Serial.begin(9600);
  limitSwitchZ1.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitchZ2.setDebounceTime(50); // set debounce time to 50 milliseconds
}
void loop() {
  Z_DRON();
}

void Z_DRON() {
  limitSwitchZ1.loop(); // MUST call the loop() function first

  if(limitSwitchZ1.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchZ1.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

  int state = limitSwitchZ1.getState();
  if(state == HIGH){
    digitalWrite(dirPin7,LOW); //LOW HACIA EL DRON
    digitalWrite(stepPin7,HIGH);
    delayMicroseconds(300);
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
  delay(1);
  }
  else{
    digitalWrite(dirPin7,LOW); //LOW HACIA EL DRON
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
  delay(1);
  }
}

void Z_RACK() {
  limitSwitchZ2.loop(); // MUST call the loop() function first

  if(limitSwitchZ2.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchZ2.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

  int state = limitSwitchZ2.getState();
  if(state == HIGH){
    digitalWrite(dirPin7,HIGH); //LOW HACIA EL DRON
    digitalWrite(stepPin7,HIGH);
    delayMicroseconds(300);
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
  delay(1);
  }
  else{
    digitalWrite(dirPin7,HIGH); //LOW HACIA EL DRON
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
  delay(1);
  }
}
