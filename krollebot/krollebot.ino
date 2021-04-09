#include <Servo.h>

/*
   Lavet af Buster, Emil og Nikolaj Jehøj-Krogager
   21/03/2021
   Dette er hovedprogrammet til Krollebot: Kryds og
   bolle med robotarm.
   Følgende pins er brugt
    - REED input         = 2
    - 4017 clock         = 4
    - Servo 1            = 3
    - Servo 2            = 5
    - Servo 3            = 6
    - Clock til LED-Grid = 7
    - 4017 reset         = 8
    - Reset til LED-Grid = 9
*/

const int boardSize = 3;

int board [3][3];
String currentTurn = "X";

// positioner målt fra robotarmens origin
// størrelserne er ganget med 100 og står altså
// i 10^(-4) meter.
float realPositions [18] = { -7, 21.50, 0, 21.50, 7, 21.50,
                             -7, 14.50, 0, 14.50, 7, 14.50,
                             -7,  7.50, 0,  7.50, 7,  7.50
                           };
float r1 = 11.5;
float r2 = 13;


// servomotorernes vinkler i robotarmen
int angleZero = 90;
int angleOne = 90;
int angleTwo = 0;


Servo servo1;
Servo servo2;
Servo servo3;

const int servo1Pin = 3;
const int servo2Pin = 5;
const int servo3Pin = 6;

void setup() {
  // ##### SERVO SETUP START #####
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  // ##### SERVO SETUP END   #####
  Serial.begin(9600);

}
int positionc = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    String incomingString = Serial.readStringUntil('\n');
    positionc = incomingString.toInt();

    calculateAngleFromPos(positionc);
    moveServoTo(servo1, angleZero);
    moveServoTo(servo2, angleOne);
    moveServoTo(servo3, angleTwo);
    Serial.print(String(positionc) + "  -  ");
    Serial.print(angleZero);
    Serial.print("   ");
    Serial.print(angleOne);
    Serial.print("   ");
    Serial.print(angleTwo);
    Serial.println();
  }
}

bool checkWin() {
  for (int i = 0; i < boardSize; i++) {
    // tjek vandret
    if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
      // vundet
    }
    // tjek lodret
    if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
      // vundet
    }
    // tjek diagonaler
    if ((board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
        (board[0][2] == board[1][1] && board[1][1] == board[2][0])) {
      // vundet
    }
  }
}

// man burde overveje hvor præcist udregningerne skal være
// og måske bruge float i stedet for int. Jeg bruger allerede
// en opganget version af tallet i int for at få mere præcision.
// De står altså i 10^(-4) meter i stedet for 10^(-2), som er cm.
void calculateAngleFromPos(int boardIndex) {
  // Ifølge Arduino reference skal der ikke stå en funktion
  // inde i sq(), hvilket der gjorde før hvor abs() stod derinde
  float x = abs(realPositions[boardIndex * 2]);
  float y = abs(realPositions[boardIndex * 2 + 1]);
  float square = sq(x) + sq(y);
  // Arduino reference siger ikke noget om der må stå funktioner
  // inde i sqrt(), men for en sikkerhedsskyld gøres det ikke
  float distance = sqrt(square);
  // First servo
  float servoAngle = HALF_PI - asin((realPositions[boardIndex * 2]) / distance);
  angleZero = 180 / PI * servoAngle;
  // Second servo
  servoAngle = acos((sq(r1) + sq(distance) - sq(r2)) / (2 * r1 * distance)) + HALF_PI;
  angleOne = 180 / PI * servoAngle;
  // Third servo
  servoAngle = acos((sq(r1) + sq(r2) - sq(distance)) / (2 * r1 * r2));
  angleTwo = 180 - (180 / PI * servoAngle);
}


// bevæger servomotorerne langsommere på en blokerende måde
void moveServoTo(Servo servo, int angleTo) {
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
