/*
   Lavet af Buster, Emil og Nikolaj Jehøj-Krogager
   23/03/2021

   Til test af REED switch grid. Du skal bruge følgende setup:
    - Clock til 4017 på pin 4
    - REED ground til Arduino Uno pin 2
*/

const int reedInputPin = 2;
const int counterClockPin = 4;
const int counterResetPin = 8;

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
}

void loop() {
  // reset 4017 tælleren
  resetCounter();

  // læs alle REED switch og gem i array
  for (int i = 0; i < 9; i++) {
    bool sensorVal = digitalRead(reedInputPin);
    reedBoard[i] = sensorVal;
    // Skift til næste REED switch
    clockCounter();
  }

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
}

void resetCounter() {
  digitalWrite(counterResetPin, HIGH);
  delay(5);
  digitalWrite(counterResetPin, LOW);
}

void clockCounter() {
  digitalWrite(counterClockPin, HIGH);
  delay(5);
  digitalWrite(counterClockPin, LOW);
}
