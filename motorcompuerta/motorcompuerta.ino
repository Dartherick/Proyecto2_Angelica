#include <ezButton.h>

ezButton limitSwitchC1(32);  // create ezButton object that attach to pin 32 CERRADA;
ezButton limitSwitchC2(33);  // create ezButton object that attach to pin 33 ABIERTA;

// defines pins

int stepPin9 = 30; //define Pulse pin
int dirPin9 = 31; //define Direction pin

//float pasos = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin9,OUTPUT);
  pinMode(dirPin9,OUTPUT);
  //pinMode(enPin5,OUTPUT);
  
  Serial.begin(9600);
  limitSwitchC1.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitchC2.setDebounceTime(50); // set debounce time to 50 milliseconds
}
void loop() {
  CERRAR_COMPUERTA();
  delay(5);
}

void CERRAR_COMPUERTA() {
  limitSwitchC1.loop(); // MUST call the loop() function first

  if(limitSwitchC1.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchC1.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }
   
  int state = limitSwitchC1.getState();
  if(state == HIGH){
    //Serial.println("The limit switch: UNTOUCHED");
    digitalWrite(dirPin9,LOW); //LOW HACIA EL CENTRO
    digitalWrite(stepPin9,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin9,LOW);
    delayMicroseconds(500);
  delay(1);
  }
  else{
    //Serial.println("The limit switch: TOUCHED");// Makes 400 pulses for making two full cycle rotation
    digitalWrite(dirPin9,LOW); //LOW HACIA EL CENTRO
    digitalWrite(stepPin9,LOW);
    delayMicroseconds(500);
    digitalWrite(stepPin9,LOW);
    delayMicroseconds(500);
  delay(1);
  }
}

void ABRIR_COMPUERTA() {
  limitSwitchC2.loop(); // MUST call the loop() function first

  if(limitSwitchC2.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchC2.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }
   
  int state = limitSwitchC2.getState();
  if(state == HIGH){
    //Serial.println("The limit switch: UNTOUCHED");
    digitalWrite(dirPin9,HIGH); //LOW HACIA EL CENTRO
    digitalWrite(stepPin9,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin9,LOW);
    delayMicroseconds(500);
  delay(1);
  }
  else{
    //Serial.println("The limit switch: TOUCHED");// Makes 400 pulses for making two full cycle rotation
    digitalWrite(dirPin9,LOW); //LOW HACIA EL CENTRO
    digitalWrite(stepPin9,LOW);
    delayMicroseconds(500);
    digitalWrite(stepPin9,LOW);
    delayMicroseconds(500);
  delay(1);
  }
}
