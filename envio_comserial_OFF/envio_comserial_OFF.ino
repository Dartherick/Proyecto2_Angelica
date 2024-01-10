void SendFloatMessage(float Float,char Function[3]){

  char message[15];
  char floatStr[10];

  dtostrf(Float, 2, 2, floatStr);
  sprintf(message,"<%s%s>",Function,floatStr); // Create the message in the desired structure
  Serial.println(message);  // Send the complete message
}

void SendStrMessage(char message[10]){
  char FormattedMessage[10];
  sprintf(FormattedMessage, "<%s>", message);  // Combine the float string with "<" and ">"
  Serial.println(FormattedMessage);  // Send the complete message
}
