// defines pins

int stepPin5 = 3; //define Pulse pin
int dirPin5 = 2; //define Direction pin
//int enPin5 = 4; //define Enable Pin

uint8_t idx = 0;
char input_message[20]; // String to hold incoming message
uint8_t str_length = 0;
bool new_message_available = false;
char expected_message;
int compare;
bool status_message = false;

//float pasos = 0;
 
void setup() {
  // Sets the two pins as Outputs
  
  Serial.begin(9600); // Start serial communication at 9600 baud rate
  pinMode(stepPin5,OUTPUT); 
  pinMode(dirPin5,OUTPUT);
  //pinMode(enPin5,OUTPUT);
}
void loop() {
  while(status_message == false){
    motorgir1();
    delay(1);
    RCOM("41001");
  }
    if(status_message == true){
    //delay(2000);
    motorgir1_stop();
    }
    delay(1);
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
      status_message = true;
      }
      else{
      status_message = false;
      }
      return status_message;
      Serial.print(status_message);
    }
    new_message_available = false;
  }
 
}

void motorgir1() {
  //Makes 400 pulses for making two full cycle rotation
  digitalWrite(dirPin5,HIGH); //Changes the rotations direction
    //digitalWrite(enPin5,LOW);
    digitalWrite(stepPin5,HIGH);
    delay(1);
    digitalWrite(stepPin5,LOW);
    delay(1);
}

void motorgir1_stop() {
 
  //Makes 400 pulses for making two full cycle rotation
  digitalWrite(dirPin5,HIGH); //Changes the rotations direction
    //digitalWrite(enPin5,LOW);
    digitalWrite(stepPin5,LOW);
    delay(1);
    digitalWrite(stepPin5,LOW);
    delay(1);
}
