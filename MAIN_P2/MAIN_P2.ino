//CÓDIGO PRINCIPAL PROYECTO 2

#include <ezButton.h> //incluir librería de limit switch

//LS Compuerta
ezButton limitSwitchC1(32);  // create ezButton object that attach to pin 32 CERRADA;
ezButton limitSwitchC2(33);  // create ezButton object that attach to pin 33 ABIERTA;

//LS Mecanismo ajuste
ezButton limitSwitchE1(37);  // create ezButton object that attach to pin 37 EXTREMO;


//VARIABLES RECEPCIÓN COM
uint8_t idx = 0;
char input_message[20]; // String to hold incoming message
uint8_t str_length = 0;
bool new_message_available = false;
char expected_message;
int compare;
bool status_message = false;
char* message;

//VARIABLES LLEGADA DE DRON 
// Definir los pines del sensor ultrasónico 3
const int trigPin3 = 27;
const int echoPin3 = 28;
// Variables para calcular la distancia
long duration3;
int distance3;
bool arrived = false;

//VARIABLES DISPONIBILIDAD
// Definir los pines del sensor ultrasónico 1
const int trigPin = 23;
const int echoPin = 24;
// Definir los pines del sensor ultrasónico 2
const int trigPin2 = 25;
const int echoPin2 = 26;
// Variables para calcular la distancia
long duration;
int distance;
long duration2;
int distance2;

// VARIABLES MOTOR COMPUERTA
int stepPin9 = 30; //define Pulse pin
int dirPin9 = 31; //define Direction pin

// VARIABLES Motor de doble efecto perfiles 1 y 2
int stepPin12= 15; //define Pulse pin
int dirPin12= 16; //define Direction pin
int enPin12= 14; //define Enable Pin

// VARIABLES Motor de simple efecto perfil 3
int enPin3= 10; //define Enable pin
int stepPin3= 9; //define Pulse pin
int dirPin3= 8; //define Direction pin

// VARIABLES Motor de simple efecto perfil 4
int stepPin4= 12; //define Pulse pin
int dirPin4= 11; //define Direction pin
int enPin4= 13; //define Enable Pin

// VARIABLES Motor de base giratoria TB6600
int stepPin5 = 3; //define Pulse pin
int dirPin5 = 2; //define Direction pin


void setup() {
  // Inicializar el monitor serial
  Serial.begin(9600);
  
  // Inicializar los pines del sensor ultrasónico llegada de dron
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  
  // Inicializar los pines del sensor ultrasónico disponibilidad
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Sets the pins as Outputs MOTOR COMPUERTA
  pinMode(stepPin9,OUTPUT);
  pinMode(dirPin9,OUTPUT);

  // LS MOTOR COMPUERTA
  limitSwitchC1.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitchC2.setDebounceTime(50); // set debounce time to 50 milliseconds

  // Sets the pins as Outputs MOTOR PERFILES 1 Y 2
  pinMode(stepPin12,OUTPUT); 
  pinMode(dirPin12,OUTPUT);
  pinMode(enPin12,OUTPUT);

  // Sets the pins as Outputs MOTOR PERFIL 4 
  pinMode(stepPin4,OUTPUT); 
  pinMode(dirPin4,OUTPUT);
  pinMode(enPin4,OUTPUT);

  // Sets the pins as Outputs MOTOR PERFIL 3
  pinMode(stepPin3,OUTPUT); 
  pinMode(dirPin3,OUTPUT);
  pinMode(enPin3,OUTPUT);

  // Sets the pins as Outputs MOTOR BASE GIRATORIA
  pinMode(stepPin5,OUTPUT); 
  pinMode(dirPin5,OUTPUT);

  //LS Mecanismo de ajuste
  limitSwitchE1.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  //Verifica llegada de dron
  while(arrived == false){
    dron_arrived();
  }
  //Verifica disponibilidad con 2 ultrasónico
  int disp1 = disponibilidad1();
  int disp2 = disponibilidad2();

  if(disp1 == 0 && disp2 == 0){
    tx_message("30001");//Disponible
  }
  else{
    tx_message("30002");//No Disponible
  }
  //Espera confirmación para abrir compuerta
  while(status_message == false){
    rx_message("20001");//Abrir compuerta
  }
  ABRIR_COMPUERTA();

  while(disp1 == 0 && disp2 == 0){
  disponibilidad1();
  disponibilidad2();  
  }
  delayMicroseconds(5000);
  
  CERRAR_COMPUERTA();
  //INICIA ETAPA 1 DE MOVIMIENTO
  movA();//movimiento de los 4 a la vez centralizando el dron
  delay(1000);
  movB();//movimiento de los 4 perfiles soltando el dron
  delay(1000);
  movGIR();//movimiento de giro comunicación con cámara
  delay(1000);
  movC();//movimiento de los 4 a la vez centralizando el dron full
  delay(1000);
  movD_END();//movimiento de los 2 perfiles llevando el dron hacia el frente
  delay(1000);
  //Comunicación para confirmar que el dron se encuentra en posición de cambio
  tx_message("40002");
  //Espera a recibir la información de la batería a cambiar
  while(true){  
    message = rx_message_extract();
    if(message == "54001" || message == "54002" || message == "54003"){
      break;
    }
  }
  //INICIA ETAPA 2 DE MOVIMIENTO
  
 
  delay(15000);
}

void tx_message(const char* message) {//ENVÍO MENSAJES
  // Create a buffer to store the formatted message
  char Formatted_Message[10];

  Formatted_Message[0] = '<';
  strcpy(Formatted_Message + 1, message);
  Formatted_Message[strlen(message) + 1] = '>';
  Formatted_Message[strlen(message) + 2] = '\0'; // Null-terminate the string

  // Send the formatted message via serial communication
  Serial.write(Formatted_Message);
}

void rx_message(char* expected_message) {//RECEPCIÓN MENSAJES
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
      status_message = true;
      }
      else{
      status_message = false;
      }
      return status_message;
      Serial.print(status_message);
    }
    new_message_available = false;
  }
 
}

char* rx_message_extract() {//RECEPCIÓN MENSAJES
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
      return parsed_message;
    }
    new_message_available = false;
  }
 
}

void dron_arrived() {
  // Enviar un pulso de 10 microsegundos al pin TRIG
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);

  // Calcular la duración del eco del pulso
  duration3 = pulseIn(echoPin3, HIGH);

  // Calcular la distancia en centímetros
  distance3 = duration3 * 0.034 / 2;

  if (distance3 >= 1 && distance3 <= 15) {
    //Serial.print("Distancia: ");
    Serial.print(distance3);
    //Serial.println(" cm");
    Serial.println("ARRIVED");
    tx_message("A0");
    return arrived = true;
  } else {
    Serial.println("NO ARRIVED");
    tx_message("A1");
  }

  delay(500);
}

int disponibilidad1() {
  // Enviar un pulso de 10 microsegundos al pin TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calcular la duración del eco del pulso
  duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centímetros
  distance = duration * 0.034 / 2;

  if (distance >= 10 && distance <= 100) {
    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");
    Serial.println("No disponible");
    return 1;
  } else {
    Serial.println("Disponible");
    return 0;
  }

  delay(500);
}

int disponibilidad2() {
  // Enviar un pulso de 10 microsegundos al pin TRIG
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  // Calcular la duración del eco del pulso
  duration = pulseIn(echoPin2, HIGH);

  // Calcular la distancia en centímetros
  distance2 = duration2 * 0.034 / 2;

  if (distance2 >= 10 && distance2 <= 100) {
    Serial.print("Distancia: ");
    Serial.print(distance2);
    Serial.println(" cm");
    Serial.println("No disponible");
    return 1;
  } else {
    Serial.println("Disponible");
    return 0;
  }

  delay(500);
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

void movA (){ //movimiento de los 4 a la vez centralizando el dron
  int pasostotal2 = 9100*1.8; // 21 cm se debe mover
  int pasostotal3 = 9100*1.8; // 22
  int pasostotal4 = 9100*1.8; // 21
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
  int pasostotal2 = 9100*0.05; // 21 cm se debe mover
  int pasostotal3 = 9100*0.05;
  int pasostotal4 = 9100*0.05;
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
    counter_array2[1]--;
    }
  }
}

void movC (){ //movimiento de los 4 a la vez centralizando el dron full
  int pasostotal2 = 9100*0.1; // 9100*1.85
  int pasostotal3 = 9100*0.1; // 9100*1.85
  int pasostotal4 = 9100*0.1; // 9100*1.85
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

void movD_END() {
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

void movGIR() {
  while(status_message == false){
    motorgir();
    delay(1);
    rx_message("41001");
  }
    if(status_message == true){
    motorgir_stop();
    }
    delay(1);
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
