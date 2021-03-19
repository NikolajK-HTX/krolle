// ##### SERVO SETTINGS START #####
#include <Servo.h>

Servo servo1;

const int servo1Pin = 3;
// ##### SERVO SETTINGS END   #####

void setup() {
  // ##### SERVO SETUP START #####
  servo1.attach(servo1Pin);
  // ##### SERVO SETUP END   #####

  Serial.begin(9600);
  Serial.setTimeout(100);
}

void loop() {
  if (Serial.available() > 0) {
    for (int i = 0; i < 1; i++) {
      // forventer 30;20;60; med andre tal
      String incomingString = Serial.readStringUntil('\n');
      int incomingValue = incomingString.toInt();
      
      servo1.write(incomingValue);
      Serial.println("Hello " + String(incomingValue));
    }
  }
}
