/*
 * Lavet af Buster, Emil og Nikolaj Jehøj-Krogager
 * 23/03/2021
 * 
 * Til test af REED switch grid. Du skal bruge følgende setup: 
 *  - Clock til 4017 på pin 4
 *  - REED ground til Arduino Uno pin 2
 */

const int reedInputPin = 2;
const int counterClockPin = 4;

bool reedBoard[9];
bool pastReedBoard[9];

void setup() {
  Serial.begin(9600);

  pinMode(reedInputPin, INPUT);
  pinMode(counterClockPin, OUTPUT);

  for (int i = 0; i<9; i++) {
    reedBoard[i] = false;
    pastReedBoard[i] = false;
  }
}

void loop() {
  // læs alle REED switch og gem i array
  for (int i = 0; i<9; i++) {
    bool sensorVal = digitalRead(reedInputPin);
    reedBoard[i] = sensorVal;
    // Skift til næste REED switch
    digitalWrite(counterClockPin, HIGH);
    delay(5);
    digitalWrite(counterClockPin, LOW);
  }
  // sammenlign de to arrays
  for (int i = 0; i<9; i++) {
    if (reedBoard[i] != pastReedBoard[i]) {
      Serial.println("En brik er flyttet til plads " + String(i));
    }
  }
  // gem array
  for (int i = 0; i<9; i++) {
    pastReedBoard[i] = reedBoard[i];
  }
}
