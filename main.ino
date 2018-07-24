/*
Five LEDs will flash in order at a certain speed,
the player must click on the button when the green
LED is lit in order to score a point which will lead
to the white LED flashing twice and a two tone from
the piezo element to signal this, the servo is then
updated to a new position for the score. If the red
LED is lit when clicking on the button the player
will lose a point which is signalled by the white LED
giving a single flash and a single tone from the piezo
element, the servo will then be updated to show the current
score. The speed of the game can be controlled through
the potentiometer from a slow, easy game to a quick
paced difficult game. If the player reaches a score of
18, the scoring mechanism is reset back to zero to
play again.

Author: Freddie John Millman
Version: 28/11/2014
*/

#include <Servo.h>
int intPosition = 0; // Variable to store the servo position.
int intScore = 0; // Stores the score in the game.
Servo myservo; // Create a servo object to control a servo.

int sensorPin = A0;  // Select the input pin for the potentiometer.
int sensorValue = 0; // Variable to store the value coming from the sensor.

int piezoPin = 9; // Select the output pin for the piezo element.

void setup() {
  // Initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  // Initialize digital pins 3 - 7 as outputs.
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // Initialize the pushbutton pin as an input.
  pinMode(2, INPUT);

  // Attaches the servo on pin 10 to the servo object.
  myservo.attach(10);

  // Initialise piezo element as an output.
  pinMode(piezoPin, OUTPUT);
}

void loop() {
  // Variable for reading the pushbutton status.
  int buttonState = 0;
  // Variable for testing if green LED is lit.
  boolean boolIsGreen;
  // Variable for testing if button pressed after any LED is lit.
  boolean boolIsValid;

  // Turn each LED on and off in turn ope after another.
  for (int i = 3; i < 8; i++) {
    digitalWrite(i, HIGH);

    // If the green LED is lit, the isGreen variable is set to true.
    if (i == 4) {
      boolIsGreen = true;
    }
    else {
      // If a red LED is selected, the variable is set to false.
      boolIsGreen = false;
    }

    // Allow status of variables and button to be read for the...
    // ... duration of the LED being lit.
    for (int intDelay = 0; intDelay < analogRead(sensorPin); intDelay++) {
      buttonState = digitalRead(2);

      // Player can only score if the button has been clicked after
      // ... the current LED has been lit.
      if (buttonState == HIGH) {
         boolIsValid = true;
      }

      buttonState = digitalRead(2);

      // Initiates success if button has been clicked after...
      // ... the green LED has been lit.
      if (buttonState == LOW && boolIsGreen && boolIsValid) {
        updateScore('+');
        ledFlash(2);
        toneEvent('w');
        boolIsGreen = false;
        boolIsValid = false;
      }
      // Initiates failure if button has been clicked after...
      // ... any red LED has been lit.
      else if (buttonState == LOW && !boolIsGreen && boolIsValid) {
        updateScore('-');
        ledFlash(1);
        toneEvent('l');
        boolIsValid = false;
      }
      delay(1);
    }
    digitalWrite(i, LOW);
    delay(100);
  }
}

/**
 * The score is incremented if the button is clicked at the right time
 * if not, the score is decremented. The servo is updated for both occasions.
 */
void updateScore(char charSign) {
  switch (charSign) {
    case '+':
      // Once the maximum score has been reached, the score is reset to zero.
      if (intScore < 18) {
        intScore++;
      }
      else {
        intScore = 0;
      }
      break;
    case '-':
      // Once zero has been reached, the score remains at zero.
      if (intScore > 0) {
        intScore--;
      }
      break;
  }
  myservo.write(intScore * 10);
}

/**
 * The white LED will flash a certain number of times.
 */
void ledFlash(int intFlashes) {
  for (int i = 0; i < intFlashes; i++) {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
  }
}

/**
 * The tone played depends on when the button is clicked.
 */
void toneEvent(char charEvent) {
  switch (charEvent) {
    case 'w':
      playTone(1915, 300);
      playTone(1700, 300);
      break;
    case 'l':
      playTone(500, 600);
      break;
  }
}

/**
 * A tone can be played for a certain duration of time.
 */
void playTone(int intTone, int intDuration) {
  for (long i = 0; i < intDuration * 1000L; i += intTone * 2) {
    digitalWrite(piezoPin, HIGH);
    delayMicroseconds(intTone);
    digitalWrite(piezoPin, LOW);
    delayMicroseconds(intTone);
  }
}
