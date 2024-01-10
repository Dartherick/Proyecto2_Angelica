#include <ezButton.h>

ezButton limitSwitchE1(37);  // create ezButton object that attach to pin 37 EXTREMO;

// defines pins

// PIN 37 LIMIT""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// Motor de doble efecto driver 2 smart s42c perfiles 1 y 2
int stepPin12= 15; //define Pulse pin
int dirPin12= 16; //define Direction pin
int enPin12= 14; //define Enable Pin

// Motor de simple efecto driver 2 smart s42c perfil 3
int enPin3= 10; //define Enable pin
int stepPin3= 9; //define Pulse pin
int dirPin3= 8; //define Direction pin

// Motor de simple efecto driver smart s42c perfil 4
int stepPin4= 12; //define Pulse pin
int dirPin4= 11; //define Direction pin
int enPin4= 13; //define Enable Pin

// Motor de base giratoria TB6600
int stepPin5 = 3; //define Pulse pin
int dirPin5 = 2; //define Direction pin
uint8_t idx = 0;
char input_message[20]; // String to hold incoming message
uint8_t str_length = 0;
bool new_message_available = false;
char expected_message;
int compare;
bool CAMR = false;

void setup() {
  pinMode(stepPin12,OUTPUT); 
  pinMode(dirPin12,OUTPUT);
  pinMode(enPin12,OUTPUT);

  pinMode(stepPin4,OUTPUT); 
  pinMode(dirPin4,OUTPUT);
  pinMode(enPin4,OUTPUT);

  pinMode(stepPin3,OUTPUT); 
  pinMode(dirPin3,OUTPUT);
  pinMode(enPin3,OUTPUT);

  pinMode(stepPin5,OUTPUT); 
  pinMode(dirPin5,OUTPUT);

  Serial.begin(9600);
  limitSwitchE1.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  delay(1500);
  movA();
  delay(1000);
  movB();
  delay(7000);
//  movGIR();
//  delay(7000);
//  movC();
//  delay(1000);
  //movD_END();
  delay(1000);
  delay(15000);
//  mov12();
//  mov3();
//  mov4();
}

void mov12() {
  delayMicroseconds(5000);
  int pasostotal2 = 910; //910 es 1cm 
  
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

void mov3() {
  delayMicroseconds(5000);
  int pasostotal3 = 910; // 910 pasos es 1cm
  
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < pasostotal3; x++) {
    digitalWrite(dirPin3,HIGH); //HIGH HACIA EL CENTRO
    digitalWrite(enPin3,LOW);
    digitalWrite(stepPin3,HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin3,LOW);
    delayMicroseconds(1000);
  }
  delay(5000);
}

void mov4() {
  delayMicroseconds(5000);
  int pasostotal4 = 910; // 910 pasos es 1cm
  
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < pasostotal4; x++) {
    digitalWrite(dirPin4,HIGH); //HIGH HACIA EL CENTRO
    digitalWrite(enPin4,LOW);
    digitalWrite(stepPin4,HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin4,LOW);
    delayMicroseconds(2000);
  }
  delay(5000);
}

void mov5() {

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

void movA (){ //movimiento de los 4 a la vez centralizando el dron
  int pasostotal2 = 9100*1.75; // 21 cm se debe mover
  int pasostotal3 = 9100*1.75; // 22
  int pasostotal4 = 9100*1.75; // 21
  float counter_array1[] = {pasostotal2, pasostotal3,pasostotal4};

  int size1 = sizeof(counter_array1) / sizeof(counter_array1[0]);
  int mayor1 = counter_array1[0];

  for (int i = 1; i < size1; i++) {
    if (counter_array1[i] > mayor1) {
      mayor1 = counter_array1[i];
    }
  }
  
 for (int i=0; i<mayor1; i++)
  {
    if(counter_array1[0] > 0){
    digitalWrite(dirPin12,LOW); //LOW HACIA EL CENTRO
    digitalWrite(enPin12,LOW);
    digitalWrite(stepPin12,HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin12,LOW);
    delayMicroseconds(800);
    counter_array1[0]--;
    }
    if(counter_array1[1] > 0){
    digitalWrite(dirPin3,HIGH); //HIGH HACIA EL CENTRO
    digitalWrite(enPin3,LOW);
    digitalWrite(stepPin3,HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin3,LOW);
    delayMicroseconds(800);
    counter_array1[1]--;
    }
    if(counter_array1[2] > 0){
    digitalWrite(dirPin4,HIGH); //HIGH HACIA EL CENTRO
    digitalWrite(enPin4,LOW);
    digitalWrite(stepPin4,HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin4,LOW);
    delayMicroseconds(800);
    counter_array1[2]--;
    }
  }
}

void movB (){ //movimiento de los 4 perfiles soltando el dron
  int pasostotal2 = 9100*0.30; // 21 cm se debe mover
  int pasostotal3 = 9100*0.30;
  int pasostotal4 = 9100*0.30;
  float counter_array2[] = {pasostotal2, pasostotal3,pasostotal4};

  int size2 = sizeof(counter_array2) / sizeof(counter_array2[0]);
  int mayor2 = counter_array2[0];

  for (int i = 1; i < size2; i++) {
    if (counter_array2[i] > mayor2) {
      mayor2 = counter_array2[i];
    }
  }
  
 for (int i=0; i<mayor2; i++)
  {
    if(counter_array2[0] > 0){
    digitalWrite(dirPin12,HIGH); //LOW HACIA EL CENTRO
    digitalWrite(enPin12,LOW);
    digitalWrite(stepPin12,HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin12,LOW);
    delayMicroseconds(800);
    counter_array2[0]--;
    }
    if(counter_array2[1] > 0){
    digitalWrite(dirPin3,LOW); //HIGH HACIA EL CENTRO
    //digitalWrite(enPin,LOW);
    digitalWrite(stepPin3,HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin3,LOW);
    delayMicroseconds(800);
    counter_array2[1]--;
    }
    if(counter_array2[2] > 0){
    digitalWrite(dirPin4,LOW); //HIGH HACIA EL CENTRO
    //digitalWrite(enPin,LOW);
    digitalWrite(stepPin4,HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin4,LOW);
    delayMicroseconds(800);
    counter_array2[2]--;
    }
  }
}

void movC (){ //movimiento de los 4 a la vez centralizando el dron full
  int pasostotal2 = 9100*0.25; // 9100*1.85
  int pasostotal3 = 9100*0.25; // 9100*1.85
  int pasostotal4 = 9100*0.25; // 9100*1.85
  float counter_array3[] = {pasostotal2, pasostotal3,pasostotal4};

  int size3 = sizeof(counter_array3) / sizeof(counter_array3[0]);
  int mayor3 = counter_array3[0];

  for (int i = 1; i < size3; i++) {
    if (counter_array3[i] > mayor3) {
      mayor3 = counter_array3[i];
    }
  }
  
 for (int i=0; i<mayor3; i++)
  {
    if(counter_array3[0] > 0){
    digitalWrite(dirPin12,LOW); //LOW HACIA EL CENTRO
    digitalWrite(enPin12,LOW);
    digitalWrite(stepPin12,HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin12,LOW);
    delayMicroseconds(800);
    counter_array3[0]--;
    }
    if(counter_array3[1] > 0){
    digitalWrite(dirPin3,HIGH); //HIGH HACIA EL CENTRO
    //digitalWrite(enPin,LOW);
    digitalWrite(stepPin3,HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin3,LOW);
    delayMicroseconds(800);
    counter_array3[1]--;
    }
    if(counter_array3[2] > 0){
    digitalWrite(dirPin4,HIGH); //HIGH HACIA EL CENTRO
    //digitalWrite(enPin,LOW);
    digitalWrite(stepPin4,HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin4,LOW);
    delayMicroseconds(800);
    counter_array3[2]--;
    }
  }
}

void movD_END() {
  limitSwitchE1.loop(); // MUST call the loop() function first

  if(limitSwitchE1.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  }

  if(limitSwitchE1.isReleased()){
     Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  }

  int state = limitSwitchE1.getState();
  while(state == HIGH){
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
    limitSwitchE1.loop(); // MUST call the loop() function first
    state = limitSwitchE1.getState();
  }
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

void movGIR() {
  while(CAMR == false){
    motorgir();
    delay(1);
    RCOM("41001");
  }
    if(CAMR == true){
    motorgir_stop();
    }
    delay(1);
}

void RCOM(char* expected_message) {
  while(Serial.available() && new_message_available == false)
  {
    char data_byte = Serial.read();
    //Serial.write(data_byte);
    if(data_byte != '\n')
    {
      input_message[idx] = data_byte;
      idx++;
      if(idx >= 20){
        idx = 19;
      }
    }else{
      input_message[idx] = '\0';
      str_length = idx - 1;
      idx = 0;
      new_message_available = true;
    }
   
  }
  
  if(new_message_available)
  {
    uint8_t str_sz = strlen(input_message);
    if ((input_message[0] == '<') && (input_message[str_sz - 1] == '>'))
    //if (*input_message == '<') //&& *(input_message + str_length - 1) == '>')
      {
      char parsed_message[20];
      strncpy(parsed_message, &(input_message[1]), str_sz - 2);
      parsed_message[str_sz - 2] = '\0';
      Serial.print(parsed_message);
      
      int compare = strcmp(parsed_message,expected_message);
      if(compare == 0){
      CAMR = true;
      }
      return CAMR;
      Serial.print(CAMR);
    }
    new_message_available = false;
  }
 
}

void motorgir() {
  //Makes 400 pulses for making two full cycle rotation
  digitalWrite(dirPin5,HIGH); //Changes the rotations direction
    //digitalWrite(enPin5,LOW);
    digitalWrite(stepPin5,HIGH);
    delay(1);
    digitalWrite(stepPin5,LOW);
    delay(1);
}

void motorgir_stop() {
 
  //Makes 400 pulses for making two full cycle rotation
  digitalWrite(dirPin5,HIGH); //Changes the rotations direction
    //digitalWrite(enPin5,LOW);
    digitalWrite(stepPin5,LOW);
    delay(1);
    digitalWrite(stepPin5,LOW);
    delay(1);
}
