/*
   Lavet af Buster, Emil og Nikolaj Jehøj-Krogager
   23/04/2021

   Til test af REED switch grid. Du skal bruge følgende setup:
    - Clock til 4017 på pin 4
    - REED ground til Arduino Uno pin 2

   LED-Grid:
    - Clock til LED-Grid 4017 på pin 7
    - Reset til LED-Grid 4017 på pin 9

   Man skal huske at have dioder mellem REED-kontakten mellem
   den og jord, før jord forbindes med de andre REED-kontakter.
   Hvis man ikke gør det, virker opstillingen ikke efter hensigten
   da den ikke kan registrere mere end en tændt REED-kontakt
   (det var lidt for mange IKKE'r).

   Servomotorer:
    - Servo 1 (bunden)  = 3
    - Servo 2 (midten)  = 5
    - Servo 3 (toppen)  = 6
*/

// ##### SERVO SETTINGS START #####
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

const int servo1Pin = 3;
const int servo2Pin = 5;
const int servo3Pin = 6;
// ##### SERVO SETTINGS END   #####

// ##### LED AND REED SETTINGS ####
const int reedInputPin = 2;
const int counterClockPin = 4;
const int counterResetPin = 8;

const int ledGridClockPin = 7;
const int ledGridResetPin = 9;

bool reedBoard[9];
bool pastReedBoard[9];
// ##### LED AND REED END     #####

const int elektroPin = 11;

void setup() {
  pinMode(elektroPin, OUTPUT);
  
  // ##### SERVO SETUP START #####
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  // ##### SERVO SETUP END   #####
  // ##### LED AND REED SETUP ####
  pinMode(reedInputPin, INPUT);
  pinMode(counterClockPin, OUTPUT);
  pinMode(counterResetPin, OUTPUT);
  digitalWrite(counterClockPin, LOW);
  digitalWrite(counterResetPin, LOW);
  resetCounter(counterResetPin);

  for (int i = 0; i < 9; i++) {
    reedBoard[i] = false;
    pastReedBoard[i] = false;
  }

  // LED-Grid
  pinMode(ledGridClockPin, OUTPUT);
  pinMode(ledGridResetPin, OUTPUT);
  digitalWrite(ledGridClockPin, LOW);
  digitalWrite(ledGridResetPin, LOW);
  resetCounter(ledGridResetPin);
  // parkerer 4017 tælleren til LED på plads 10 (OUTPUT 9)
  for (int i = 0; i < 9; i++) {
    clockCounter(ledGridClockPin);
  }
  // ##### LED AND REED END #####



  Serial.begin(9600);
  Serial.setTimeout(100);
}

bool elektroOn = false;

void loop() {
  while (Serial.available() > 0) {
    for (int i = 0; i < 3; i++) {
      // forventer 30;20;60; med andre tal
      String incomingString = Serial.readStringUntil(';');
      Serial.print(incomingString + "   ");
      if (incomingString == "elek") {
        elektroOn = !elektroOn;
        digitalWrite(elektroPin, elektroOn);
        break;
      }
      else {
        int incomingValue = incomingString.toInt();
        // skriv til den rigtige servo
        if (i == 0) {
          moveServoTo(incomingValue, servo1);
        } else if (i == 1) {
          moveServoTo(incomingValue, servo2);
        } else if (i == 2) {
          moveServoTo(incomingValue, servo3);
        }
      }

    }
    Serial.println();
  }
  // ### LED AND REED LOOP ###
  // reset 4017 tælleren
  resetCounter(counterResetPin);

  // læs alle REED switch og gem i array
  for (int i = 0; i < 9; i++) {
    delayMicroseconds(10);
    bool sensorVal = digitalRead(reedInputPin);
    reedBoard[i] = sensorVal;
    // Skift til næste REED switch
    clockCounter(counterClockPin);
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

  // reset LED-grid
  resetCounter(ledGridResetPin);
  // vis på LED-Grid
  for (int i = 0; i < 9; i++) {
    if (reedBoard[i] == true) {
      delay(2);
    }
    clockCounter(ledGridClockPin);
  }
  delay(1);
}

void moveServoTo(int angleTo, Servo servo) {
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

void resetCounter(int pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(1);
  digitalWrite(pin, LOW);
  delayMicroseconds(1);
}

void clockCounter(int pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(1);
  digitalWrite(pin, LOW);
  delayMicroseconds(1);
}
