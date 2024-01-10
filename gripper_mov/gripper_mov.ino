#include <Servo.h>

Servo servoMotor;  // Crea un objeto Servo

int anguloAbierto = 0;  // Ángulo para la posición "abierta" del servo
int anguloCerrado = 180;  // Ángulo para la posición "cerrada" del servo

void setup() {
  servoMotor.attach(43);  // Especifica el pin del servo
  servoMotor.write(anguloAbierto);
}

void loop() {
  open_gripper();
  delay(15000);  // Espera 1 segundo
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
