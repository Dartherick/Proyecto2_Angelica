#include <ezButton.h>

ezButton limitSwitchX1(39);  // create ezButton object that attach to pin 32 DRON;
ezButton limitSwitchX2(44);  // create ezButton object that attach to pin 32 DRON;

// defines pins motor se mueve arriba y abajo

int stepPin6 = 45; //define Pulse pin
int dirPin6 = 46; //define Direction pin
int enPin6= 47; //define Enable Pin

//float pasos = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin6,OUTPUT); 
  pinMode(dirPin6,OUTPUT);
  pinMode(enPin6,OUTPUT);

  Serial.begin(9600);
  limitSwitchX1.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitchX2.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  //X1_TAKEBAT();
  //movXUP();
  X1_HOME();
  
  //X2_TOP();
  //Serial.print("ESPERA");
//  movXUP();
  delay(5000);
//  Serial.print("SALIO");
  //X1_HOME();
}

void X1_HOME() {
  limitSwitchX1.loop(); // MUST call the loop() function first

  if(limitSwitchX1.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchX1.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

   int state = limitSwitchX1.getState();
  while(state == HIGH){
    digitalWrite(dirPin6,HIGH); //LOW HACIA EL ARRIBA
    digitalWrite(enPin6,LOW);
    digitalWrite(stepPin6,HIGH);
    delayMicroseconds(10);
    digitalWrite(stepPin6,LOW);
    delayMicroseconds(10);
    limitSwitchX1.loop(); // MUST call the loop() function first
    state = limitSwitchX1.getState();
  }
    digitalWrite(dirPin6,HIGH); //LOW HACIA EL ARRIBA
    digitalWrite(enPin6,LOW);
    digitalWrite(stepPin6,LOW);
    delayMicroseconds(10);
    digitalWrite(stepPin6,LOW);
    delayMicroseconds(10);
}

void X2_TOP() {
  limitSwitchX2.loop(); // MUST call the loop() function first

  if(limitSwitchX2.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchX2.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

   int state = limitSwitchX2.getState();
  while(state == HIGH){
    digitalWrite(dirPin6,LOW); //LOW HACIA EL ARRIBA
    digitalWrite(enPin6,LOW);
    digitalWrite(stepPin6,HIGH);
    delayMicroseconds(10);
    digitalWrite(stepPin6,LOW);
    delayMicroseconds(10);
    limitSwitchX2.loop(); // MUST call the loop() function first
    state = limitSwitchX2.getState();
  }
    digitalWrite(dirPin6,LOW); //LOW HACIA EL ARRIBA
    digitalWrite(enPin6,LOW);
    digitalWrite(stepPin6,LOW);
    delayMicroseconds(10);
    digitalWrite(stepPin6,LOW);
    delayMicroseconds(10);
}

void X1_TAKEBAT() { //Se mueve para tomar la batería
  movXUP();
  delay(1000);//CAMBIAR DELAY!!!!!!!!!!!!!!!
  movXUP();
  delay(1000);
  movXUP();
  delay(1000);
  movXUP();
  delay(1000);
  movXUP();
  delay(1000);
  movXUP();
  delay(1000);
  movXUP();
  delay(1000);
}

void movXUP() { //Movimiento de alturas, repetir cuantas veces sea necesario 
  
  int pasostotal6 = 30000; // 200 pasos * 27 que es la reduccion
   for(int x = 0; x < pasostotal6; x++){
    digitalWrite(dirPin6,LOW); //LOW HACIA EL ARRIBA
    digitalWrite(enPin6,LOW);
    digitalWrite(stepPin6,HIGH);
    delayMicroseconds(30);//CAMBIAR DELAY!!!!!!!!!!!!!!!
    digitalWrite(stepPin6,LOW);
    delayMicroseconds(30);
  }
    delay(1000);
    Serial.print("DEBE SALIR");
 }
