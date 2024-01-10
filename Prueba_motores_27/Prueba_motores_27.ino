/*   
 *   Basic example code for controlling a stepper without library
 *      
 *   by Dejan, https://howtomechatronics.com
 */

// defines pins

int stepPin= 8; //define Pulse pin
int dirPin= 9; //define Direction pin
int enPin= 10; //define Enable Pin

//float pasos = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
}
void loop() {
  delayMicroseconds(5000);
   int pasostotal3 = 5400; // 200 pasos * 27 que es la reduccion
  float ppg1 = pasostotal3/360; // ppg pasos por grado
  //float ang3 = 19.756; // ángulo cintura
  float ang3 = 90;
  float pasos = ang3*ppg1*16; // pasos a dar cintura
  //float pasos = 150*16;//10
  //float pasos = 3200*10.2;//90 para 51:1

  // Makes 200 pulses for making one full cycle rotation
//  for(int x = 0; x < pasos; x++) {
//    digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
//    digitalWrite(enPin,LOW);
//    digitalWrite(stepPin,HIGH); 
//    delayMicroseconds(1000);    // by changing this time delay between the steps we can change the rotation speed
//    digitalWrite(stepPin,LOW); 
//    delayMicroseconds(1000); 
//  }
//  delay(5000); // One second delay
  
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < pasos; x++) {
    digitalWrite(dirPin,HIGH); //Changes the rotations direction
    digitalWrite(enPin,LOW);
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(2000);
  }
  delay(5000);
}
