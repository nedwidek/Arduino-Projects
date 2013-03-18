/**
  *  Program: Button_LED
  *  Author:  Erik Nedwidek
  *  Date:    2013/03/18
  *  License: BSD
  *  
  *  Uses a momentary push button to turn a LED on and off.
  *  Each press of the button will turn the led on if it is
  *  off and vice versa.
  *
  */
  
int ledPin    = 6;
int switchPin = 7;

// We'll use a global variable to track whether the LED is on
// or off.
bool ledOn = false;

// Include and instantiate an instance of the Bounce object.
// We want to debounce the switch signal with a 20ms interval.
#include <Bounce.h>
Bounce bouncer(switchPin, 20);


// In our setup we will be setting the LED pin to output and
// the switch pin to input.
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  
  // Set the initial state of the LED.
  if(ledOn) {
    digitalWrite(ledPin, HIGH);
  }
}

// Loop just calls our handleButton() method. 
void loop() {
  handleButton();
}

void handleButton() {
  
  // Read the button state and if pressed, wait for the user to
  // release it. Then flip the state of ledOn.
  bouncer.update(); 
  if(bouncer.fallingEdge()) {
    ledOn = !ledOn;
    
    if(ledOn) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
}

