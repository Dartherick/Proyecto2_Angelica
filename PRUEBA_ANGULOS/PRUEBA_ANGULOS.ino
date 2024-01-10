#include <math.h>
#include <Servo.h>

#include "SmartStepper.h"

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
#define s_hombro 27
#define s_codo 28

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
void Desenfundar (void);
void homing (void);
void aimbot (void);

float B = 0;
float H = -90;
float C = 0;
Servo Muneca;
Servo Gatillo;
Servo Agarre;

int M_actual = 95;
int G_actual = 0;
int A_actual = 0;
int B_actual = 0;
int H_actual = 0;
int C_actual = 0;

int M_req = 95;
int G_req = 0;
int A_req = 0;
int B_req = 0;
int H_req = 0;
int C_req = 0;

bool control = true;
bool Hcontrol = true;

int ang, pos, angs;
int td = 20;
String cad,cad1,cad2,ant;

int B_Fundado[21] = {  0,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15};
int H_Fundado[21] = {-65, -65, -60, -55, -50, -47, -45, -40, -37, -35, -35, -35, -35, -35, -35, -35, -35, -35, -35, -35, -35};
int C_Fundado[21] = {-95, -95, -95, -95, -95, -95, -95, -95, -95, -95, -90, -86, -83, -80, -76, -73, -70, -67, -64, -60, -50};
int M_Fundado[21] = {130, 130, 127, 125, 122, 120, 118, 115, 112, 110, 108, 105, 102, 100,  97,  95,  92,  90,  87,  85,  90};
int A_Fundado[21] = {  0,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60};
int G_Fundado[21] = {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};


void setup() {
  //put your setup code here, to run once:
  BaseStepper.setUp(0, -90, 90);//  home,min,max
  HombroStepper.setUp(0, -90, 90);//
  CodoStepper.setUp(0, -95, 95);//
  Serial.begin(9600);
  
  Agarre.attach(13);
  Muneca.attach(14);
  Gatillo.attach(15);
  
  Agarre.write (A_actual);
  Gatillo.write (G_actual);
  Muneca.write (M_actual);
}



void loop() 
{
  if (Serial.available())
  {
    cad = Serial.readString();
    Serial.println(cad);
    //if (ant != cad)
    //{
      ant = cad;
      pos = cad.indexOf (',');
      cad1 = cad.substring (0,pos);
      cad1.toUpperCase();
      cad2 = cad.substring (pos+1);
      ang = cad2.toInt();
  
      if (cad1 == 'M')
      {
        //    95     < 100
        if (M_actual < ang)
        {
          for (int i = M_actual; i < ang; i++)
          {
            Muneca.write (i);
            delay(td);
          }
          M_actual = ang;
        }
        else
        {
          for (int i = M_actual; i > ang; i--)
          {
            Muneca.write (i);
            delay(td);
          }
          M_actual = ang;
        }
      }
      else if (cad1 == 'G')
      {
        if (G_actual < ang)
        {
          for (int i = G_actual; i < ang; i++)
          {
            Gatillo.write (i);
            delay(td);
          }
          G_actual = ang;
        }
        else
        {
          for (int i = G_actual; i > ang; i--)
          {
            Gatillo.write (i);
            delay(td);
          }
          G_actual = ang;
        }
      }
      else if (cad1 == 'A')
      {
        if (A_actual < ang)
        {
          for (int i = A_actual; i < ang; i++)
          {
            Agarre.write (i);
            delay(td);
          }
          A_actual = ang;
        }
        else
        {
          for (int i = A_actual; i > ang; i--)
          {
            Agarre.write (i);
            delay(td);
          }
          A_actual = ang;
        }
      }
      else if (cad1 == 'B')
      {
        BaseStepper.moveAngle(ang,6000);
      }
      else if (cad1 == 'H')
      {
        HombroStepper.moveAngle(ang,500);
      }
      else if (cad1 == 'C')
      {
        CodoStepper.moveAngle(ang,6000);
      }
      else if (cad1 == 'D')
      {
        Desenfundar();
      }
      else if (cad1 == 'Q')
      {
        homing ();
      }
      else if (cad1 == 'P')
      {
        Serial.println("hola");
        aimbot();
      }
    //}
  }
}



void Desenfundar()
{ 
  for (int i = 0; i<21; i++)
  {
    control = true;
    M_req = M_Fundado [i];
    G_req = G_Fundado [i];
    A_req = A_Fundado [i];
    B_req = B_Fundado [i];
    H_req = H_Fundado [i];
    C_req = C_Fundado [i];
  
    while (control)
    {
      if (M_actual != M_req)
      {
        if (M_actual < M_req)
        {
          M_actual = M_actual + 1;
          Muneca.write (M_actual);
          //delay (td);
        }
        else if (M_actual > M_req)
        {
          M_actual = M_actual - 1;
          Muneca.write (M_actual);
          //delay (td);
        }
        Serial.print ("M = ");
        Serial.println (M_actual);
      }
  
      if (G_actual != G_req)
      {
        if (G_actual < G_req)
        {
          G_actual = G_actual + 1;
          Gatillo.write (G_actual);
          //delay (td);
        }
        else if (G_actual > G_req)
        {
          G_actual = G_actual - 1;
          Gatillo.write (G_actual);
          //delay (td);
        }
        Serial.print ("G = ");
        Serial.println (G_actual);
      }
  
      if (A_actual != A_req)
      {
        if (A_actual < A_req)
        {
          A_actual = A_actual + 1;
          Agarre.write (A_actual);
          //delay (td);
        }
        else if (A_actual > A_req)
        {
          A_actual = A_actual - 1;
          Agarre.write (A_actual);
          //delay (td);
        }
        Serial.print ("A = ");
        Serial.println (A_actual);
      }
  
      if (B_actual != B_req)
      {
        if (B_actual < B_req)
        {
          B_actual = B_actual + 1;
          BaseStepper.moveAngle (B_actual,6000);
        }
        else if (B_actual > B_req)
        {
          B_actual = B_actual - 1;
          BaseStepper.moveAngle (B_actual,6000);
        }
        Serial.print ("B = ");
        Serial.println (B_actual);
      }
  
      if (H_actual != H_req)
      {
        if (H_actual < H_req)
        {
          H_actual = H_actual + 1;
          HombroStepper.moveAngle (H_actual);
        }
        else if (H_actual > H_req)
        {
          H_actual = H_actual - 1;
          HombroStepper.moveAngle (H_actual);
        }
        Serial.print ("H = ");
        Serial.println (H_actual);
      }
      
      if (C_actual != C_req)
      {
        if (C_actual < C_req)
        {
          C_actual = C_actual + 1;
          CodoStepper.moveAngle (C_actual,6000);
        }
        else if (C_actual > C_req)
        {
          C_actual = C_actual - 1;
          CodoStepper.moveAngle (C_actual,6000);
        }
        Serial.print ("C = ");
        Serial.println (C_actual);
      }
  
      if ((M_actual == M_req) && (G_actual == G_req) && (A_actual == A_req) &&
          (B_actual == B_req) && (H_actual == H_req) && (C_actual == C_req))
      {
        control = false;
      }
      
    }
    Serial.println ("--------------------");
    delay (100);
    
  }
  Serial.println ("Sali del for"); 
}

void homing() 
{ 
  if (Hcontrol)
  {
    while ((!digitalRead(CodoStepper.S_HOME)) ) 
    {
      CodoStepper.moveAngle (C_actual,8000);
      C_actual = C_actual + 1;
      Serial.println (C_actual);
    }
    
    Serial.println("------- Codo Hommed--------");
    
    while ((!(digitalRead(HombroStepper.S_HOME)))) 
    {
      HombroStepper.moveAngle (H_actual,500);
      H_actual = H_actual + 1;
      Serial.println (H_actual);
    }
  
    Serial.println("------- Hombro Hommed--------");
    
    while ((!(digitalRead(BaseStepper.S_HOME))) ) 
    {
      BaseStepper.moveAngle (B_actual,8000);
      B_actual = B_actual + 1;
      Serial.println (B_actual);
    }
    
    Serial.println("------- Base Hommed--------");
    BaseStepper.AngActual = 0;
    HombroStepper.AngActual = 0;
    CodoStepper.AngActual = 0;
  
    BaseStepper.moveAngle (-60,6000);
    HombroStepper.moveAngle (7, 500);
    CodoStepper.moveAngle (4, 6000);
    
    BaseStepper.AngActual = 0;
    HombroStepper.AngActual = 0;
    CodoStepper.AngActual = 0;
  
    B_actual = 0;
    H_actual = 0;
    C_actual = 0;
    
    Serial.println("------- Done Homing --------");
    Hcontrol = false;
  }
  else
  {
    BaseStepper.moveAngle (0,6000);
    HombroStepper.moveAngle (0, 500);
    CodoStepper.moveAngle (0, 6000);
    Agarre.write (0);
    Gatillo.write (0);
    Muneca.write (95);
  }
}

void aimbot ()
{
  if (ang == 1)
  {
    BaseStepper.moveAngle (-32,6000);
    CodoStepper.moveAngle  (-50,6000);
    angs = 125;
    if (M_actual < angs)
    {
      for (int i = M_actual; i < angs; i++)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    else
    {
      for (int i = M_actual; i > angs; i--)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    Gatillo.write (60);
    delay (500);
    Gatillo.write (0);
  }
  
  else if (ang == 2)
  {
    BaseStepper.moveAngle  (-2,6000);
    CodoStepper.moveAngle  (-50,6000);
    angs = 125;
    if (M_actual < angs)
    {
      for (int i = M_actual; i < angs; i++)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    else
    {
      for (int i = M_actual; i > angs; i--)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    Gatillo.write (60);
    delay (500);
    Gatillo.write (0);
  }

  else if (ang == 3)
  {
    BaseStepper.moveAngle  (-30,6000);
    CodoStepper.moveAngle  (-50,6000);
    angs = 112;
    if (M_actual < angs)
    {
      for (int i = M_actual; i < angs; i++)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    else
    {
      for (int i = M_actual; i > angs; i--)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    Gatillo.write (60);
    delay (500);
    Gatillo.write (0);
  }

  else if (ang == 4)
  {
    BaseStepper.moveAngle  (-2,6000);
    CodoStepper.moveAngle  (-50,6000);
    angs = 112;
    if (M_actual < angs)
    {
      for (int i = M_actual; i < angs; i++)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    else
    {
      for (int i = M_actual; i > angs; i--)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    Gatillo.write (60);
    delay (500);
    Gatillo.write (0);
  }

  else if (ang == 5)
  {
    BaseStepper.moveAngle  (-28,6000);
    CodoStepper.moveAngle  (-50,6000);
    angs = 95;
    if (M_actual < angs)
    {
      for (int i = M_actual; i < angs; i++)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    else
    {
      for (int i = M_actual; i > angs; i--)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    Gatillo.write (60);
    delay (500);
    Gatillo.write (0);
  }

  else if (ang == 6)
  {
    BaseStepper.moveAngle  (-2,6000);
    CodoStepper.moveAngle  (-50,6000);
    angs = 100;
    if (M_actual < angs)
    {
      for (int i = M_actual; i < angs; i++)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }
    else
    {
      for (int i = M_actual; i > angs; i--)
      {
        Muneca.write (i);
        delay(td);
      }
      M_actual = angs;
    }

    Gatillo.write (60);
    delay (500);
    Gatillo.write (0);
  }
  /////////////
}
