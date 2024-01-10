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
}
void loop() {
  //delay(1000);
  movXUP();
  delay(1000);
  movXUP();
}
void movXUP() {
   //int pasostotal6 = 200; // 200 pasos * 27 que es la reduccion
   for(int x = 0; x < 30000; x++){
    digitalWrite(dirPin6,LOW); //LOW HACIA EL ARRIBA
    digitalWrite(enPin6,LOW);
    digitalWrite(stepPin6,HIGH);
    delayMicroseconds(30);
    digitalWrite(stepPin6,LOW);
    delayMicroseconds(30);
  }
    //delay(2000);
 }
