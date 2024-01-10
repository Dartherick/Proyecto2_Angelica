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

int D;

void setup() {
  // Inicializar los pines del sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
    
  // Inicializar el monitor serial
  Serial.begin(9600);
}

void loop() {
  disponibilidad1();
  disponibilidad2();

  if(disponibilidad1 == 0 && disponibilidad1 == 0){
    D = 0;
    sendMessage("D0");
  }
  else{
    D = 1;
    sendMessage("D1");
  }
}

void disponibilidad1() {
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

void disponibilidad2() {
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

void sendMessage(const char* message) {
  // Create a buffer to store the formatted message
  char Formatted_Message[10];

  Formatted_Message[0] = '<';
  strcpy(Formatted_Message + 1, message);
  Formatted_Message[strlen(message) + 1] = '>';
  Formatted_Message[strlen(message) + 2] = '\0'; // Null-terminate the string

  // Send the formatted message via serial communication
  Serial.write(Formatted_Message);
}
