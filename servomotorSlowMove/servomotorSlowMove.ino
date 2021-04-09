#include <Servo.h>

/*

*/

Servo servo;

const int servoPin = 3;

void setup() {
  // put your setup code here, to run once:
  servo.attach(servoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveServoTo(120);
  delay(2000);
  moveServoTo(60);
  delay(2000);
}

void moveServoTo(int angleTo) {
  while (true) {
    int writeAngle = 0;
    int angleFrom = servo.read();
    
    if (angleFrom < angleTo) {
      writeAngle = angleFrom+1;
    } else if (angleFrom > angleTo) {
      writeAngle = angleFrom-1;
    } else if (angleFrom == angleTo) {
      break;
    }
    servo.write(writeAngle);
    delay(50);
  }

}
