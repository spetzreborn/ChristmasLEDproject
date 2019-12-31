/*
   Blink without Delay

   Turns on and off a light emitting diode (LED) connected to a digital pin,
   without using the delay() function. This means that other code can run at the
   same time without being interrupted by the LED code.

  http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
*/

// constants won't change. Used here to set a pin number:
const int intPin =  LED_BUILTIN;// the number of the LED pin

// Variables will change:
int ledStateInternal = HIGH; // Start internal LED
int blinkCounter = 0; // Variable for keeping track of blinkmode iterator

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 500;           // interval at which to blink (milliseconds)
const long delayShort = 100;
const long delayLong  = 1500;


typedef struct pinProgram {
  int pinsNumber;
  int pinsSerial[6];
  int pinsClockwise[6];
  int pinsCross[6];
  int pinsLadder[6];
} pinProgram;

// PWM: 3, 5, 6, 9, 10, and 11
pinProgram ledRed = {
  .pinsNumber    = 6,
  .pinsSerial    = {3, 5, 6, 9, 10, 11},
  .pinsClockwise = {3, 5, 9, 11, 10, 6},
  .pinsCross     = {3, 9, 10, 11, 6, 5},
  .pinsLadder    = {3, 5, 6, 10, 9, 11}
};
// redLed.size = sizeof(redLed.pins[0]) / sizeof(ledRed.pins[0][0]); // Something like this should be better.


pinProgram chipSet = {
  .pinsNumber = 5,
  .pinsSerial = {A0, A1, A2, A3, A5}
};


void setup() {
  // set the digital pin as output and turn them on
  pinMode(intPin,  OUTPUT);
  for (int i = 0; i < ledRed.pinsNumber; i++) {
    pinMode(ledRed.pinsSerial[i], OUTPUT);
    digitalWrite(ledRed.pinsSerial[i], HIGH);
    delay(delayShort);
  }
  for (int i = 0; i < ledRed.pinsNumber; i++) {
    digitalWrite(ledRed.pinsSerial[i], LOW);
  }

  //Initialaze analog pins as OUTPUT and turn them on
  for (int i = 0; i < chipSet.pinsNumber; i++) {
    pinMode(chipSet.pinsSerial[i], OUTPUT);
    digitalWrite(chipSet.pinsSerial[i], HIGH);
    delay(delayShort);
  }
  for (int i = 0; i < chipSet.pinsNumber; i++) {
    digitalWrite(chipSet.pinsSerial[i], LOW);
  }
  delay(delayLong);
}

void loop() {
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  binDisplay(blinkCounter);

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the Internal LED is off turn it on and vice-versa:
    if (ledStateInternal == LOW) {
      ledStateInternal = HIGH;
    } else {
      ledStateInternal = LOW;
    }
    // set the LED with the ledState of the variable:
    digitalWrite(intPin, ledStateInternal);

    if (blinkCounter == 0) { // Blink redled clockwise
      for (int i = 0; i <= 3 ; i++) { // Times to run program 0
        for (int pin = 0 ; pin < ledRed.pinsNumber ; pin++) {
          digitalWrite(ledRed.pinsClockwise[pin], HIGH);
          delay(delayShort);
          digitalWrite(ledRed.pinsClockwise[pin], LOW);
        }
      }
    }
    else if (blinkCounter == 1) { // Blink redled x-pattern
      for (int i = 0; i <= 3; i++) { // Times to run program 1
        for (int pin = 0 ; pin < ledRed.pinsNumber ; pin++) {
          digitalWrite(ledRed.pinsCross[pin], HIGH);
          delay(delayShort);

          digitalWrite(ledRed.pinsCross[pin], LOW);
        }
      }
    }
    else if (blinkCounter == 2) { // Light all redled
      for (int pin = 0 ; pin < ledRed.pinsNumber ; pin++) {
        digitalWrite(ledRed.pinsSerial[pin], HIGH);
      }
      delay(delayLong);
      for (int pin = 0 ; pin < ledRed.pinsNumber ; pin++) {
        digitalWrite(ledRed.pinsSerial[pin], LOW);
      }
    }
    else if (blinkCounter == 3) { // Ladder down and up
      int step   = 0;
      int steps = ledRed.pinsNumber / 2;
      for (int r = 0; r < 4; r++) { // Repetition of program
        int a1     = 0;
        int a2     = 1;
        for (; step < steps; step++) { // Light up
          digitalWrite(ledRed.pinsSerial[a1], HIGH);
          digitalWrite(ledRed.pinsSerial[a2], HIGH);
          delay(delayShort);
          a1 += 2;
          a2 += 2;
        }
        a1 -= 2;
        a2 -= 2;
        for (; step > 0; step--) { // Light down
          digitalWrite(ledRed.pinsSerial[a1], LOW);
          digitalWrite(ledRed.pinsSerial[a2], LOW);
          delay(delayShort);
          a1 -= 2;
          a2 -= 2;
        }
      }
    }
    else if (blinkCounter == 4) { // Dim lights

      for (int i = 0; i <= 255; i++) { // Dim up
        for (int pin = 0; pin < ledRed.pinsNumber ; pin++) {
          analogWrite(ledRed.pinsSerial[pin], i);
        }
        delay(5);
      }

      for (int i = 255; i >= 0; i--) { // Dim up
        for (int pin = 0; pin < ledRed.pinsNumber ; pin++) {
          analogWrite(ledRed.pinsSerial[pin], i);
        }
        delay(5);
      }

    }
    else if (blinkCounter == 5){
      delay(500);
    }



    blinkCounter++;
    if (blinkCounter > 5)
      blinkCounter = 0;
  }
}


void binDisplay(int program) {
  // A0, A1, A2, A3, A5

  // Turn display off
  for (int i = 0; i < chipSet.pinsNumber; i++) {
    digitalWrite(chipSet.pinsSerial[i], LOW);
  }

  int res;
  int bitwise = 1;
  program++;

  for (int i = chipSet.pinsNumber - 1; i > 0; i--) {

    res = program & bitwise;
    if (res > 0) {
      digitalWrite(chipSet.pinsSerial[i], HIGH);
    }

    bitwise = bitwise * 2;
  }

}


