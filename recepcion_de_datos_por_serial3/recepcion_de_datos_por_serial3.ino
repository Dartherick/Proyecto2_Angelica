void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char incomingChar = Serial.read();

    //char incomingChar[] = "<10>";
    // Verifica si el carácter es '<' para iniciar la recepción del mensaje
    if (incomingChar == '<') {
      String mensaje = "";

      // Lee los caracteres hasta encontrar '>'
      while (Serial.available() > 0) {
        char tempChar = Serial.read();

        if (tempChar == '>') {
          // Se ha recibido el mensaje completo
          extraerMensaje(mensaje);
          break;
        } else {
          mensaje += tempChar;
        }
      }
    }
  }
}

void extraerMensaje(String mensaje) {
  // Procesa el mensaje extraído
  Serial.println("Mensaje recibido: " + mensaje);

  // Extrae cada carácter del mensaje y realiza alguna operación
  for (int i = 0; i < mensaje.length(); i++) {
    char caracter = mensaje.charAt(i);
    // Realiza operaciones con el carácter, por ejemplo, imprímelo en el puerto serie
    Serial.print("Carácter extraído: ");
    Serial.println(caracter);
  }
  
  // Realiza cualquier operación adicional según tus necesidades
  // Por ejemplo, puedes convertir los caracteres a otros tipos de datos o realizar otras operaciones.
}
