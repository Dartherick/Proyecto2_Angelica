// Codo microstepping 1:16 reduccion 1:5  (12/60)
/* BASE 0 - 1 HIGH FOR CW
 * CODO 2 - 3 HIGH FOR CW
 * HOMBRO 4 - 5 LOW FOR CW
 *  L1 = 9.3 + 4.8 = 14.1
 *  L2 = 12.4
 */
#define PUL 2
#define DIR 1

int grados;
int relacion_hombro = (60/12)/360;// (26 + (103 / 121))/360 ;
int STEPS_PER_OUT_REV = 5 *  5 * 27  ;  //(26 + (103 / 121)) * 64*400;; REDUCCION ELECTRONICA * GRADOS REV / GRADOS POR PASOS * REDUCCION MECANICA
float Grados_pasos = (60/12)* 16 * (360/1.8)*grados/1.8  ;    // Pasos_rev * grado_deseado / 1.8 grados     // PASOS REV = 360/1.8 * Reduccion * Microstepping
int  StepsRequired ;
void setup() {
  // put your setup code here, to run once:
  pinMode(DIR, OUTPUT);
  pinMode(PUL, OUTPUT);


    while (!Serial);
    Serial.begin(9600);
   
}


void loop()
{
//int  etapa = 0;
//  if (etapa == 0) {
//
//
//
//    for ( int i = 0; i <= 200; i++) {
//      digitalWrite(DIR, HIGH);
//      digitalWrite(PUL, HIGH);
//      delay(5);
//      digitalWrite(PUL, LOW);
//
//      
//
//    }
//etapa = 1;
//  }
  


    while (Serial.available())
    {
     
      char Direction = Serial.read();
      
      Serial.println(Direction);
  
      if (Direction == 'R')
      {
        Serial.println(" CW");
        StepsRequired  =  STEPS_PER_OUT_REV ;
        digitalWrite(DIR, HIGH);
        for (int i = 0; i <=  StepsRequired; i++) {
  
          digitalWrite(PUL, HIGH);
          delay(2);
          digitalWrite(PUL, LOW);
          delay(2);
          //  steppermotor.setSpeed(700);
          //  steppermotor.step(StepsRequired);
        }
      }
  
      if (Direction == 'V')
      {
        Serial.println("CCW");
        // Rotate CCW 1/2 turn quickly
        StepsRequired  =   STEPS_PER_OUT_REV ;
        digitalWrite(DIR, LOW);
        for (int i = 0; i <=  StepsRequired; i++) {
  
          digitalWrite(PUL, HIGH);
          delay(2);
          digitalWrite(PUL, LOW);
          delay(2);
          //  steppermotor.setSpeed(700);
          //  steppermotor.step(StepsRequired);
  
        }
      }
      if (Direction == 'S')
      {
        Serial.println("Stop");
        StepsRequired  =  0;
        digitalWrite(DIR, HIGH);
        for (int i = 0; i <=  StepsRequired; i++) {
  
          digitalWrite(PUL, HIGH);
          delay(5);
          digitalWrite(PUL, LOW);
          //          delay(5);
          //  steppermotor.setSpeed(700);
          //  steppermotor.step(StepsRequired);
  
  
        }
      }
  
    }



}
