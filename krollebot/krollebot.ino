const int boardSize = 3;

int board [3][3];

// positioner målt fra robotarmens origin
// størrelserne er ganget med 100
int realPositions [18] = {-1050, 2200, 0, 2200, 1050, 2200,
                          -1050, 1500, 0, 1500, 1050, 1500,
                          -1050,  800, 0,  800, 1050,  800};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

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

float calculateAngleFromPos(int boardIndex) {
  float distance = sqrt(sq(abs(realPositions[boardIndex*2))+sq(abs(realPositions[boardIndex*2+1])));
  float servoAngle = acos((sq(12)+sq(distance)-sq(13))/(2*12*distance))+HALF_PI;
  return servoAngle;
}
