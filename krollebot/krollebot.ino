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

// man burde overveje hvor præcist udregningerne skal være
// og måske bruge float i stedet for int. Jeg bruger allerede
// en opganget version af tallet i int for at få mere præcision
int calculateAngleFromPos(int boardIndex) {
  // Ifølge Arduino reference skal der ikke stå en funktion
  // inde i sq(), hvilket der gjorde før hvor abs() stod derinde
  int x = abs(realPositions[boardIndex*2]);
  int y = abs(realPositions[boardIndex*2+1]);
  int square = sq(x)+sq(y);
  // Arduino reference siger ikke noget om der må stå funktioner
  // inde i sqrt(), men for en sikkerhedsskyld gøres det ikke
  int distance = sqrt(square);
  distance = distance/100;
  int servoAngle = acos((sq(12)+sq(distance)-sq(13))/(2*12*distance))+HALF_PI;
  
  return servoAngle;
}
