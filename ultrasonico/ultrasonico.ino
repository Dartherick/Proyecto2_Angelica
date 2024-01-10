// Definir los pines del sensor ultrasónico
const int trigPin = 25;
const int echoPin = 26;

// Variables para calcular la distancia
long duration;
int distance;

void setup() {
  // Inicializar los pines del sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Inicializar el monitor serial
  Serial.begin(9600);
}

void loop() {
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

  // Imprimir la distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Esperar un momento antes de volver a medir
  delay(500);
}
