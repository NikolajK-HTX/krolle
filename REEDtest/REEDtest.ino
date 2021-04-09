/*
   Lavet af Buster, Emil og Nikolaj Jehøj-Krogager
   23/03/2021

   Til test af REED switch grid. Du skal bruge følgende setup:
    - Clock til 4017 på pin 4
    - REED ground til Arduino Uno pin 2

   LED-Grid:
    - Clock til LED-Grid 4017 på pin 7
    - Reset til LED-Grid 4017 på pin 9
*/

const int reedInputPin = 2;
const int counterClockPin = 4;
const int counterResetPin = 8;

const int ledGridClockPin = 7;
const int ledGridResetPin = 9;

bool reedBoard[9];
bool pastReedBoard[9];

void setup() {
  Serial.begin(9600);

  pinMode(reedInputPin, INPUT);
  pinMode(counterClockPin, OUTPUT);
  pinMode(counterResetPin, OUTPUT);
  digitalWrite(counterClockPin, LOW);
  digitalWrite(counterResetPin, LOW);
  resetCounter();

  for (int i = 0; i < 9; i++) {
    reedBoard[i] = false;
    pastReedBoard[i] = false;
  }

  // LED-Grid
  pinMode(ledGridClockPin, OUTPUT);
  pinMode(ledGridResetPin, OUTPUT);
  digitalWrite(ledGridClockPin, LOW);
  digitalWrite(ledGridResetPin, LOW);
}

void loop() {
  // reset 4017 tælleren
  resetCounter();

  // læs alle REED switch og gem i array
  for (int i = 0; i < 9; i++) {
    delayMicroseconds(10);
    bool sensorVal = digitalRead(reedInputPin);
    reedBoard[i] = sensorVal;
    // Skift til næste REED switch
    clockCounter();
  }

  for (int i = 0; i < 9; i++) {
    Serial.print(reedBoard[i]);
  }
  Serial.println();

  // sammenlign de to arrays og sørg for at der er noget på
  // pladsen
  for (int i = 0; i < 9; i++) {
    if (reedBoard[i] != pastReedBoard[i] && reedBoard[i] != false) {
      Serial.println("En brik er på plads " + String(i));
    }
  }

  // gem array
  for (int i = 0; i < 9; i++) {
    pastReedBoard[i] = reedBoard[i];
  }

  // reset LED-grid
  digitalWrite(ledGridResetPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(ledGridResetPin, LOW);
  // vis på LED-Grid
  for (int i = 0; i < 9; i++) {
    if(reedBoard[i] == true) {
      delay(2);
    }

    // clock
    digitalWrite(ledGridClockPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(ledGridClockPin, LOW);
  }
}

void resetCounter() {
  digitalWrite(counterResetPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(counterResetPin, LOW);
}

void clockCounter() {
  digitalWrite(counterClockPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(counterClockPin, LOW);
}
