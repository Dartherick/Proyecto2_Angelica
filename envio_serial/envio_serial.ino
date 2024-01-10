void setup() {
  Serial.begin(9600);
}

void loop() {
  const char* sendMessage("klk");
}

void sendSerialMessage(const char* message) {
  // creating a buffer to hold the message
  const char* formattedMessage;
  char SerialMessage[20];

  // insert the message inside "<>"
  snprintf(formattedMessage, sizeof(formattedMessage), "<%s>", message);

  // Send the final message via serial communication. idk if it's println or print
  Serial.print(formattedMessage);
}
