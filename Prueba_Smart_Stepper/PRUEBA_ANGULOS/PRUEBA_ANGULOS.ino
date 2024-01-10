#include <math.h>

#include "SmartStepper.h"
//#include "BotServo.h"

// Definiciones
//Pines Base
#define PulB 0
#define DirB 1

//Pines Hombro
#define PulH 4
#define DirH 5

//Pines codo
#define PulC 2
#define DirC 3

//Sensores Homing
#define s_base 29
#define s_hombro 28
#define s_codo 27

//factores de transmision PASOS POR REV REV COMPLETA
float TM_Base = (60 / 10) * 360; // Transmision * Microstepping * 360
float TM_Hombro = (26 + (103 / 121)) * (60 / 12) * 360;
float TM_Codo = (60 / 12) * 360;

//Variables
float var_x, var_y, var_z; // Variables para indicar cinematica directa
float AngB, AngC, AngH; // Variables angulos de los motores
const float RAD2DEG = 180.0f / PI;
float q1_deg, q2a_deg, q3a_deg; // Variables de los angulos de cinematica inversa

int Etapa = 0;

bool CCWBase = HIGH;
bool CCWHombro = HIGH;
bool CCWCodo = LOW;

SmartStepper BaseStepper(PulB, DirB, TM_Base, s_base, CCWBase);
SmartStepper HombroStepper(PulH, DirH, TM_Hombro, s_hombro, CCWHombro);
SmartStepper CodoStepper(PulC, DirC, TM_Codo, s_codo, CCWCodo);

// Funciones
void DH_equations(float px, float py, float pz);
void PosicionarMotores(float Posx, float Posy, float Posz);
void Getnumber();

float B = 45;
float H = 17.65;
float C = 92.55;


void setup() {
  //put your setup code here, to run once:
  BaseStepper.setUp(0, -50, 45);//  home,min,max
  HombroStepper.setUp(0, -90, 90);//
  CodoStepper.setUp(0, -95, 95);//
  Serial.begin(9600);
}



void loop() {
  // put your main code here, to run repeatedly:
  BaseStepper.stepAngle(B);
  HombroStepper.stepAngle(H); // primer paso para setear on position
  CodoStepper.stepAngle(C);


  //  BaseStepper.moveAngle(10);





}
