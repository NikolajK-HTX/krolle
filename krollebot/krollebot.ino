#include <Servo.h>

/*
 * Lavet af Buster, Emil og Nikolaj Jehøj-Krogager
 * 21/03/2021
 * Dette er hovedprogrammet til Krollebot: Kryds og
 * bolle med robotarm.
 * Følgende pins er brugt
 *  - REED input    = 2
 *  - 4017 clock    = 4
 *  - Servo 1       = 3
 *  - Servo 2       = 5
 *  - Servo 3       = 6
 *  - Elektromagnet = 7
 *  - 4017 reset    = 8
 */

const int boardSize = 3;

int board [3][3];
String currentTurn = "X";

// positioner målt fra robotarmens origin
// størrelserne er ganget med 100 og står altså
// i 10^(-4) meter.
float realPositions [18] = { -10.50, 22.00, 0, 22.00, 10.50, 22.00,
                             -10.50, 15.00, 0, 15.00, 10.50, 15.00,
                             -10.50,  8.00, 0,  8.00, 10.50,  8.00
                         };

// servomotorernes vinkler i robotarmen
int angleZero = 90;
int angleOne = 90;
int angleTwo = 90;


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
  calculateAngleFromPos(positionc);
  servo1.write(angleZero);
  servo2.write(angleOne);
  servo3.write(angleTwo);
  Serial.print(String(positionc) + "  -  ");
  Serial.print(angleZero);
  Serial.print("   ");
  Serial.print(angleOne);
  Serial.print("   ");
  Serial.print(angleTwo);
  Serial.println();
  delay(2000);
  positionc = (positionc+1) % 9;
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
  float servoAngle = HALF_PI-asin((realPositions[boardIndex * 2])/distance);
  angleZero = 180 / PI * servoAngle;
  // Second servo
  servoAngle = acos((sq(12) + sq(distance) - sq(13)) / (2 * 12 * distance)) + HALF_PI;
  angleOne = 180 / PI * servoAngle;
  // Third servo
  servoAngle = acos((sq(12) + sq(13) - sq(distance)) / (2 * 12 * 13));
  angleTwo = 180 / PI * servoAngle;
}
