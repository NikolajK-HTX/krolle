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
    - Elektromagnet IN1  = 11
*/

#include <Servo.h>

// En klasse til 4017-kredse som gør det
// mere overskueligt styre dem
class CD4017BE {
  public:
    int clockPin;
    int resetPin;
    CD4017BE(int clock_Pin, int reset_Pin) {
      clockPin = clock_Pin;
      resetPin = reset_Pin;
    }
    // clock er allerede en funktion
    void nclock() {
      pulse(clockPin);
    }
    // reset er allerede en funktion
    void nreset() {
      pulse(resetPin);
    }
  private:
    void pulse(int pin) {
      digitalWrite(pin, HIGH);
      delayMicroseconds(1);
      digitalWrite(pin, LOW);
      delayMicroseconds(1);
    }
};

const int boardSize = 3;

// [i][j] i er række nr. og j er kolonne nr.
char board [3][3] = {
  {' ', ' ', ' '},
  {' ', ' ', ' '},
  {' ', ' ', ' '}
};
// Robotarmen er altid bolle! Så man starter med at placere en brik
// på brættet
char currentTurn = 'X';

int positionc = 0;

// Manuelt målte vinkler servomotorerne skal have
// for at elektromagneten passer præcist til den plads
int anglePositions [33] = {100, 107,  60,  // Position 0
                           85,  115,  75,  // Position 1
                           72,  110,  65,  // Position 2
                           103, 130,  105, // Position 3
                           81,  132,  112, // Position 4
                           62,  129,  105, // Position 5
                           118, 148,  142, // Position 6
                           82,  155,  153, // Position 7
                           50,  146,  136, // Position 8
                           83,  150,  100, // Position over brættet
                           115,  90,   25  // Position til at hente brikker
                          };

// tror vi ender med ikke at bruge det her
// hvor langt ned skal armen gå i cm
float armDownOffset = 0;
float robotArmHeight = 11;

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

const int elektroPin = 11;

// ##### LED AND REED SETTINGS  #####
const int reedInputPin = 2;
const int counterClockPin = 4;
const int counterResetPin = 8;

const int ledGridClockPin = 7;
const int ledGridResetPin = 9;

CD4017BE reedCounter(counterClockPin, counterResetPin);
CD4017BE ledCounter(ledGridClockPin, ledGridResetPin);

bool reedBoard[9];
bool pastReedBoard[9];
// ##### LED AND REED END       #####

void setup() {
  // ##### SERVO SETUP START    #####
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);

  servo1.write(angleZero);
  servo2.write(angleOne);
  servo3.write(angleTwo);

  // ##### SERVO SETUP END      #####
  pinMode(elektroPin, OUTPUT);

  // ##### LED AND REED SETUP   #####
  pinMode(reedInputPin, INPUT);
  pinMode(counterClockPin, OUTPUT);
  pinMode(counterResetPin, OUTPUT);
  digitalWrite(counterClockPin, LOW);
  digitalWrite(counterResetPin, LOW);
  reedCounter.nreset();

  for (int i = 0; i < 9; i++) {
    reedBoard[i] = false;
    pastReedBoard[i] = false;
  }

  // LED-Grid
  pinMode(ledGridClockPin, OUTPUT);
  pinMode(ledGridResetPin, OUTPUT);
  digitalWrite(ledGridClockPin, LOW);
  digitalWrite(ledGridResetPin, LOW);
  ledCounter.nreset();
  // parkerer 4017 tælleren til LED på plads 10 (OUTPUT 9)
  for (int i = 0; i < 9; i++) {
    ledCounter.nclock();
  }
  // ##### LED AND REED END     #####

  Serial.begin(9600);
  Serial.print("Debug: Husk new line. ");
  Serial.println("Skriv positionen robotarmen skal dreje hen til:");
}

void loop() {
  // ##### INPUT FRA MODSTANDER + FLYT ARM START #####
  // det sker kun hvis det er dens tur (bolles tur)
  if (currentTurn == 'O') {
    // venter input for at bestemme hvilken position robotarmen skal
    // gå hen til. Det er af debug grunde.
    if (Serial.available()) {
      String incomingString = Serial.readStringUntil('\n');
      positionc = incomingString.toInt();
      Serial.print("Debug: Flytter robotarmen til " + String(positionc));

      // man har modtaget en position spilleren gerne vil lægge en brik på
      // først tændes der for servomotoren
      digitalWrite(elektroPin, HIGH);
      // dernæst hentes en brik
      angleZero = anglePositions[10 * 3 + 0];
      angleOne  = anglePositions[10 * 3 + 1];
      angleTwo  = anglePositions[10 * 3 + 2];
      moveServoTo(servo1, angleZero);
      moveServoTo(servo3, angleTwo);
      moveServoTo(servo2, angleOne);

      delay(1500);

      // Derefter flyttes den op i pause positionen
      angleOne  = anglePositions[9 * 3 + 1];
      angleTwo  = anglePositions[9 * 3 + 2];
      moveServoTo(servo2, angleOne);
      moveServoTo(servo3, angleTwo);

      // Så flyttes den ned til der hvor brikken skal lægges
      angleZero = anglePositions[positionc * 3 + 0];
      angleOne  = anglePositions[positionc * 3 + 1];
      angleTwo  = anglePositions[positionc * 3 + 2];

      moveServoTo(servo1, angleZero);
      // servomotoren går ned til en position tæt på robotarmen
      if (servo3.read() < angleTwo) {
        moveServoTo(servo2, angleOne);
        moveServoTo(servo3, angleTwo);
      }
      // servomotoren går hen til en af de øvre positioner (0, 1 og 2)
      if (servo3.read() > angleTwo) {
        moveServoTo(servo3, angleTwo);
        moveServoTo(servo2, angleOne);
      }

      // og slipper for brikken
      digitalWrite(elektroPin, LOW);

      // Og til sidst flyttes den op i pause positionen
      angleZero = anglePositions[9 * 3 + 0];
      angleOne  = anglePositions[9 * 3 + 1];
      angleTwo  = anglePositions[9 * 3 + 2];
      moveServoTo(servo2, angleOne);
      moveServoTo(servo3, angleTwo);
      moveServoTo(servo1, angleZero);

      updateReedArray();

      currentTurn = 'X';
    }
  }
  // ##### INPUT FRA MODSTANDER + FLYT ARM END   #####

  updateReedArray();

  // reset LED-grid
  ledCounter.nreset();
  // vis på LED-Grid
  for (int i = 0; i < 9; i++) {
    if (reedBoard[i] == true) {
      delay(2);
    }
    ledCounter.nclock();
  }
  delay(1);
  // ##### LED AND REED LOOP END   #####
}

// Denne funktion tjekker board array for om nogen har vundet
//
bool checkWin() {
  bool won = false;
  for (int i = 0; i < boardSize; i++) {
    // tjek vandret
    if (board[i][0] != ' ' && board[i][0] == board[i][1] &&
        board[i][1] == board[i][2]) {
      won = true;
    }
    // tjek lodret
    if (board[0][i] != ' ' && board[0][i] == board[1][i] &&
        board[1][i] == board[2][i]) {
      won = true;
    }
  }
  // tjek diagonaler
  if ((board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
      (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])) {
    won = true;
  }
  return won;
}

// bevæger servomotorerne langsommere på en blokerende måde
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

void moveServosToCentral() {
  int t_angleZero = anglePositions[positionc * 3 + 0];
  int t_angleOne  = anglePositions[positionc * 3 + 1];
  int t_angleTwo  = anglePositions[positionc * 3 + 2];

  moveServoTo(servo1, t_angleZero);
  moveServoTo(servo2, t_angleOne);
  moveServoTo(servo3, t_angleTwo);
}

void resetBoard() {
  for (int r = 0; r < 3; r++) {
    for (int k = 0; k < 3; k++) {
      board[r][k] = ' ';
    }
  }
}

void updateReedArray() {
  // ##### LED AND REED LOOP START #####
  // reset 4017 tælleren
  reedCounter.nreset();

  // læs alle REED switch og gem i array
  for (int i = 0; i < 9; i++) {
    delayMicroseconds(10);
    bool sensorVal = digitalRead(reedInputPin);
    reedBoard[i] = sensorVal;
    // Skift til næste REED switch
    reedCounter.nclock();
  }

  // sammenlign de to arrays og undersøg om der er noget på
  // pladsen
  for (int i = 0; i < 9; i++) {
    if (reedBoard[i] != pastReedBoard[i] && reedBoard[i] != false) {
      Serial.println("Debug: En brik er på plads " + String(i));
      if (currentTurn == 'X') {
        Serial.println(i);
        int r = floor(i / 3);
        int k = i % 3;
        board[r][k] = 'X';
        // skifter tur
        currentTurn = 'O';
      };
    }
  }

  // gem array
  for (int i = 0; i < 9; i++) {
    pastReedBoard[i] = reedBoard[i];
  }
}
