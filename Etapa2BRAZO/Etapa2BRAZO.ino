#include <ezButton.h>
#include <Servo.h>

Servo servoMotor;  // Crea un objeto Servo

// Limits eje Z
ezButton limitSwitchZ1(35);  // create ezButton object that attach to pin 32 DRON;
ezButton limitSwitchZ2(36);  // create ezButton object that attach to pin 32 RACK;

// Limit eje X
ezButton limitSwitchX1(39);  // create ezButton object that attach to pin 39 HOME ALTURA;
ezButton limitSwitchX2(44);  // create ezButton object that attach to pin 44 TOPE ALTURA;

ezButton limitSwitchY1(41);  // create ezButton object that attach to pin 41 OUT;
ezButton limitSwitchY2(40);  // create ezButton object that attach to pin 40 IN;

// defines pins rack a dron

int stepPin7 = 4; //define Pulse pin
int dirPin7 = 29; //define Direction pin

// defines pins motor se mueve arriba y abajo

int stepPin6 = 45; //define Pulse pin
int dirPin6 = 46; //define Direction pin
int enPin6= 47; //define Enable Pin

// defines pins BASE TOMA BATERÍAS

int stepPin8 = 18; //define Pulse pin
int dirPin8 = 17; //define Direction pin

// define gripper angles
int anguloAbierto = 0;  // Ángulo para la posición "abierta" del servo
int anguloCerrado = 180;  // Ángulo para la posición "cerrada" del servo

void setup() {
  // Sets the pins as Outputs EJE Z
  pinMode(stepPin7,OUTPUT);
  pinMode(dirPin7,OUTPUT);

  // Sets the pins as Outputs EJE X
  pinMode(stepPin6,OUTPUT); 
  pinMode(dirPin6,OUTPUT);
  pinMode(enPin6,OUTPUT);

  // Sets the pins as Outputs EJE Y
  pinMode(stepPin8,OUTPUT); 
  pinMode(dirPin8,OUTPUT);

  Serial.begin(9600);
  //EJE Z
  limitSwitchZ1.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitchZ2.setDebounceTime(50); // set debounce time to 50 milliseconds

  //EJE X
  limitSwitchX1.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitchX2.setDebounceTime(50); // set debounce time to 50 milliseconds

  //EJE Y
  limitSwitchY1.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitchY2.setDebounceTime(50); // set debounce time to 50 milliseconds

  //Gripper
  servoMotor.attach(43);  // Especifica el pin del servo
  servoMotor.write(anguloAbierto);
}

void loop() {
  delay(15000);
  //SUBE HACIA LA BATERÍA
  X1_TAKEBAT();
  //SALE A TOMAR LA BATERÍA
  Y_OUT();
  //CIERRA EL GRIPPER PARA TOMAR LA BATERÍA
  close_gripper();
  //ENTRA LA BATERÍA AL SOPORTE
  Y_IN();
  //SUBE HACIA EL TOPE
  X2_TOP();
  //SE MUEVE HACIA EL RACK
  Z_RACK();
  //SALE A DEJAR LA BATERÍA
  Y_OUT();
  //ABRE GRIPPER
  open_gripper();
//  //BAJA A X RACK
//  
  //CIERRA GRIPPER
  close_gripper();
  //ENTRA A POSICIÓN ANTERIOR
  Y_IN();  
//  //SUBE HASTA EL TOPE
//  X2_TOP();
  //SE MUEVE HACIA EL DRON
  Z_DRON();
  //BAJA HASTA EL DRON O HACE HOME ABAJO
  X1_HOME();
  //SUBE HACIA EL DRON
  X1_TAKEBAT();
  //SALE A DEJAR EL DRON
  Y_OUT();
  //ABRE GRIPPER
  open_gripper();
  //ENTRA A POSICIÓN ANTERIOR
  Y_IN();
  //CERRAR GRIPPER
  close_gripper();
  //BAJA A POSICIÓN DE HOME
  X1_HOME();
  delay(15000);
}

void Z_DRON() {//Movimiento del rack al dron
  limitSwitchZ1.loop(); // MUST call the loop() function first

  if(limitSwitchZ1.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchZ1.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

  int state = limitSwitchZ1.getState();
  while(state == HIGH){
    digitalWrite(dirPin7,LOW); //LOW HACIA EL DRON
    digitalWrite(stepPin7,HIGH);
    delayMicroseconds(300);
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
    limitSwitchZ1.loop(); // MUST call the loop() function first
    state = limitSwitchZ1.getState();
    
  delay(1);
  }
    digitalWrite(dirPin7,LOW); //LOW HACIA EL DRON
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
  delay(1);
}

void Z_RACK() {//Movimiento del dron al rack
  limitSwitchZ2.loop(); // MUST call the loop() function first

  if(limitSwitchZ2.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchZ2.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

  int state = limitSwitchZ2.getState();
  while(state == HIGH){
    digitalWrite(dirPin7,HIGH); //LOW HACIA EL DRON
    digitalWrite(stepPin7,HIGH);
    delayMicroseconds(300);
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
    limitSwitchZ2.loop(); // MUST call the loop() function first
    state = limitSwitchZ2.getState();
  delay(1);
  }
    digitalWrite(dirPin7,HIGH); //LOW HACIA EL DRON
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
    digitalWrite(stepPin7,LOW);
    delayMicroseconds(300);
  delay(1);
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

void Y_OUT() {
  limitSwitchY1.loop(); // MUST call the loop() function first

  if(limitSwitchY1.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchY1.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

  int state = limitSwitchY1.getState();
  while(state == HIGH){
    digitalWrite(dirPin8,LOW); //Changes the rotations direction
    digitalWrite(stepPin8,HIGH);
    delay(2);
    digitalWrite(stepPin8,LOW);
    delay(2);
    limitSwitchY1.loop(); // MUST call the loop() function first
    state = limitSwitchY1.getState();
  }
    digitalWrite(dirPin8,LOW); //Changes the rotations direction
    digitalWrite(stepPin8,LOW);
    delay(2);
    digitalWrite(stepPin8,LOW);
    delay(2);
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
  while(state == HIGH){
    digitalWrite(dirPin8,HIGH); //Changes the rotations direction
    digitalWrite(stepPin8,HIGH);
    delay(2);
    digitalWrite(stepPin8,LOW);
    delay(2);
    limitSwitchY2.loop(); // MUST call the loop() function first
    state = limitSwitchY2.getState();
  }
    digitalWrite(dirPin8,HIGH); //Changes the rotations direction
    digitalWrite(stepPin8,LOW);
    delay(2);
    digitalWrite(stepPin8,LOW);
    delay(2);
}

void open_gripper() {
  // Abre el servo
  servoMotor.write(anguloAbierto);
  delay(2000);  // Espera 1 segundo
  }

void close_gripper() {
  // Cierra el servo
  servoMotor.write(anguloCerrado);
  delay(2000);  // Espera 1 segundo
  }
