/*
   Dette program er til at bevæge en enkelt servomotor
   på pin 3 langsomt. Den bevæger sig frem og tilbage
   mellem vinklerne 60 og 120. Man kan ændre hastigheden
   enten ved at mindske delayet i slutningen af funktionen
   eller bevæge flere grader.
*/

#include <Servo.h>

Servo servo1;

const int servoPin = 3;

void setup() {
  // put your setup code here, to run once:
  servo1.attach(servoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveServoTo(servo1, 120);
  delay(2000);
  moveServoTo(servo1, 60);
  delay(2000);
}

void moveServoTo(Servo servo, int angleTo) {
  while (true) {
    int writeAngle = 0;
    int angleFrom = servo.read();

    if (angleFrom < angleTo) {
      writeAngle = angleFrom + 1;
    } else if (angleFrom > angleTo) {
      writeAngle = angleFrom - 1;
    } else if (angleFrom == angleTo) {
      break;
    }
    servo.write(writeAngle);
    delay(50);
  }

}
