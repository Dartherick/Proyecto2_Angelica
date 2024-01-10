// Definir los pines del sensor ultrasónico 3
const int trigPin3 = 27;
const int echoPin3 = 28;

// Variables para calcular la distancia
long duration3;
int distance3;

void setup() {
  // Inicializar los pines del sensor ultrasónico
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  
  // Inicializar el monitor serial
  Serial.begin(9600);
}

void loop() {
  dron_arrived();
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
    Serial.print("Distancia: ");
    Serial.print(distance3);
    Serial.println(" cm");
    Serial.println("ARRIVED");
    sendMessage("A0");
  } else {
    Serial.println("NO ARRIVED");
    sendMessage("A1");
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
