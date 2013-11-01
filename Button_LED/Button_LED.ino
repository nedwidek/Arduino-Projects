/**
  *  Program: Button_LED
  *  Author:  Erik Nedwidek
  *  Date:    2013/03/18
  *  License: BSD
  *  Requires:
  *     Bounce - http://playground.arduino.cc/code/bounce
  
  *  Uses a momentary push button to turn a LED on and off.
  *  Each press of the button will turn the led on if it is
  *  off and vice versa.
  *
  */
  
int ledPin    = 4;
int switchPin = 1;

// We'll use a global variable to track whether the LED is on
// or off.
bool ledOn = true;

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
  Serial.begin(9600);
}

// Loop just calls our handleButton() method. 
void loop() {
  handleButton();
}

void handleButton() {
  
  // Ask the bouncer to update and if we're on the falling edge,
  // flip the state of the LED. Falling edge returning true means
  // the prior time update was called the button read HIGH
  // (pushed in our case), but now it reads LOW.
  bouncer.update(); 
  if(bouncer.fallingEdge()) {
    Serial.println("Button pushed");
    ledOn = !ledOn;
    
    if(ledOn) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
}

