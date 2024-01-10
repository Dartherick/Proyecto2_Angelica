#include <ezButton.h>

ezButton limitSwitchE1(37);  // create ezButton object that attach to pin 37 EXTREMO;

// Codigo motor de simple efecto driver 2 semart sc24
// defines pins

// Motor de simple efecto driver smart s42c perfil 4
int stepPin4= 12; //define Pulse pin
int dirPin4= 11; //define Direction pin
int enPin4= 13; //define Enable Pin

// Motor de simple efecto driver 2 smart s42c perfil 3
int enPin3= 10; //define Enable pin
int stepPin3= 9; //define Pulse pin
int dirPin3= 8; //define Direction pin
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin4,OUTPUT); 
  pinMode(dirPin4,OUTPUT);
  pinMode(enPin4,OUTPUT);

  pinMode(stepPin3,OUTPUT); 
  pinMode(dirPin3,OUTPUT);
  pinMode(enPin3,OUTPUT);

  Serial.begin(9600);
  limitSwitchE1.setDebounceTime(50); // set debounce time to 50 milliseconds
}
void loop() {
  D_END();
}

void D_END() {
  limitSwitchE1.loop(); // MUST call the loop() function first

  if(limitSwitchE1.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchE1.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

  int state = limitSwitchE1.getState();
  if(state == HIGH){
    digitalWrite(dirPin4,LOW); //HIGH HACIA EL CENTRO
    digitalWrite(enPin4,LOW);
    digitalWrite(stepPin4,HIGH);
    delayMicroseconds(600);
    digitalWrite(stepPin4,LOW);
    delayMicroseconds(600);
    digitalWrite(dirPin3,HIGH); //HIGH HACIA EL CENTRO
    digitalWrite(enPin3,LOW);
    digitalWrite(stepPin3,HIGH);
    delayMicroseconds(600);
    digitalWrite(stepPin3,LOW);
    delayMicroseconds(600);
  }
  else{
    digitalWrite(dirPin4,LOW); //HIGH HACIA EL CENTRO
    digitalWrite(enPin4,LOW);
    digitalWrite(stepPin4,LOW);
    delayMicroseconds(600);
    digitalWrite(stepPin4,LOW);
    delayMicroseconds(600);
    digitalWrite(dirPin3,HIGH); //HIGH HACIA EL CENTRO
    digitalWrite(enPin3,LOW);
    digitalWrite(stepPin3,LOW);
    delayMicroseconds(600);
    digitalWrite(stepPin3,LOW);
    delayMicroseconds(600);
  }
}
