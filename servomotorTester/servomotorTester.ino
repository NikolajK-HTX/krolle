// ##### SERVO SETTINGS START #####
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

const int servo1Pin = ?;
const int servo2Pin = ?;
const int servo3Pin = ?;
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
  if (Serial.available() > 0) {
    for (int i = 0; i < 3; i++) {
      // forventer 30;20;60; med andre tal
      String incomingString = Serial.readStringUntil(';');
      int incomingValue = incomingString.toInt();
      // skriv til den rigtige servo
      switch (i) {
        case 0:
          servo1.write(incomingValue);
          break
        case 1:
          servo2.write(incomingValue);
          break
        case 2:
          servo3.write(incomingValue);
          break
      }
    }
  }
}
