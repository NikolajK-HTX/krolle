// ##### SERVO SETTINGS START #####
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

const int servo1Pin = 3;
const int servo2Pin = 5;
const int servo3Pin = 6;
// ##### SERVO SETTINGS END   #####

void setup() {
  // ##### SERVO SETUP START #####
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  // ##### SERVO SETUP END   #####

  Serial.begin(9600);
  Serial.setTimeout(100);
}

void loop() {
  while (Serial.available() > 0) {
    for (int i = 0; i < 3; i++) {
      // forventer 30;20;60; med andre tal
      String incomingString = Serial.readStringUntil(';');
      Serial.println(incomingString);
      int incomingValue = incomingString.toInt();
      // skriv til den rigtige servo
      if (i==0) {
        servo1.write(incomingValue);
      } else if (i==1) {
        servo2.write(incomingValue);
      } else if (i==2) {
        servo3.write(incomingValue);
      }
    }
  }
}
