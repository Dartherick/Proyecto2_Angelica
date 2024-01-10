uint8_t idx = 0;
char input_message[20]; // String to hold incoming message
uint8_t str_length = 0;
bool new_message_available = false;
char expected_message;
int compare;
bool CAMR = false;

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud rate
  pinMode(13,OUTPUT);

}

void loop() {
  while(CAMR == false){
    digitalWrite(13,LOW);
    delay(5000);
    RCOM("41001");
  }
    if(CAMR == true){
    digitalWrite(13,HIGH);
    }
    delay(5000);
    CAMR = false;
}

void RCOM(char* expected_message) {
  while(Serial.available() && new_message_available == false)
  {
    char data_byte = Serial.read();
    //Serial.write(data_byte);
    if(data_byte != '\n')
    {
      input_message[idx] = data_byte;
      idx++;
      if(idx >= 20){
        idx = 19;
      }
    }else{
      input_message[idx] = '\0';
      str_length = idx - 1;
      idx = 0;
      new_message_available = true;
    }
   
  }
  
  if(new_message_available)
  {
    uint8_t str_sz = strlen(input_message);
    if ((input_message[0] == '<') && (input_message[str_sz - 1] == '>'))
    //if (*input_message == '<') //&& *(input_message + str_length - 1) == '>')
      {
      char parsed_message[20];
      strncpy(parsed_message, &(input_message[1]), str_sz - 2);
      parsed_message[str_sz - 2] = '\0';
      Serial.print(parsed_message);
      
      int compare = strcmp(parsed_message,expected_message);
      if(compare == 0){
      CAMR = true;
      }
      return CAMR;
      Serial.print(CAMR);
    }
    new_message_available = false;
  }
 
}
