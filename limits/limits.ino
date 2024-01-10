#include <ezButton.h>

ezButton limitSwitch(44);  // create ezButton object that attach to pin 7;

void setup() {
  Serial.begin(9600);
  limitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  limitSwitch.loop(); // MUST call the loop() function first

//  if(limitSwitch.isPressed())
//    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
//
//  if(limitSwitch.isReleased())
//    Serial.println("The limit switch: TOUCHED -> UNTOUCHED");

  while(limitSwitch.getState()){
    Serial.println("The limit switch: UNTOUCHED");
    limitSwitch.loop(); // MUST call the loop() function first
  }
    Serial.println("The limit switch: TOUCHED");
}
