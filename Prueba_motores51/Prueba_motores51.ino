/*   
 *   Basic example code for controlling a stepper without library
 *      
 *   by Dejan, https://howtomechatronics.com
 */

// defines pins

int stepPin= 5; //define Pulse pin
int dirPin= 6; //define Direction pin
int enPin= 7; //define Enable Pin

  //float ang = 360;
  //int ppv = 3200;
  //float mult = (ang*10.2)/360;
  //float pasos = 1350*16;//90
  //float pasos = 150*16;//10
  //float pasos = mult*ppv;//90 para 51:1
  float pasos = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
}

void loop() {
  delay(3000);
  mov2CW();
  delay(10);
  mov2CW();
  delay(10);
  mov2CW();
  delay(10);
  mov2CW();
  delay(10);
  mov2CW();
  delay(20000); // One second delay
}
  
void mov2CW() {
   float ang = 90; 
  //float ang = 3.2589;
  int ppv = 3200;
  float mult = (ang*10.2)/360;
  float pasos = mult*ppv;//90 para 51:1
  
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < pasos; x++) {
    digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
    digitalWrite(enPin,LOW);
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }
  
}

//    for(int x = 0; x < 2250; x++) {
//    digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
//    digitalWrite(enPin,LOW);
//    digitalWrite(stepPin,HIGH); 
//    delayMicroseconds(200);    // by changing this time delay between the steps we can change the rotation speed
//    digitalWrite(stepPin,LOW); 
//    delayMicroseconds(200); 
//  }
//  delay(10000); // One second delay
  
  // Makes 400 pulses for making two full cycle rotation
//  for(int x = 0; x < pasos; x++) {
//    digitalWrite(dirPin,HIGH); //Changes the rotations direction
//    digitalWrite(enPin,LOW);
//    digitalWrite(stepPin,HIGH);
//    delayMicroseconds(500);
//    digitalWrite(stepPin,LOW);
//    delayMicroseconds(500);
//  }
//  delay(5000);
