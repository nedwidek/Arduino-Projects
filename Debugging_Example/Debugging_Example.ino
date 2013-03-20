/**
  *  Program: Debugging_Example
  *  Author:  Erik Nedwidek
  *  Date:    2013/03/18
  *  License: BSD
  *  Requires:
  *     Bounce - http://playground.arduino.cc/code/bounce
  *
  *  Wire a button in the same manner as the Button_LED
  *  example. This program is an example of one method to
  *  debug your sketches using the serial monitor.
  *
  */
  
int switchPin = 7;
int baudRate = 9600;

// We'll use a global variable to track how many times the
// button has been pushed.
int pushes = 0;

// Include and instantiate an instance of the Bounce object.
// We want to debounce the switch signal with a 20ms interval.
#include <Bounce.h>
Bounce bouncer(switchPin, 20);

void setup() {
  Serial.begin(baudRate);
  Serial.println("Here we go!");
}

void loop() {
  bouncer.update();
  
  if(bouncer.risingEdge()) {
    Serial.println("Button is pressed");
  }
  
  if(bouncer.fallingEdge()) {
    pushes++;
    Serial.println("Button has been released");
    Serial.print("Button has been pushed ");
    Serial.print(pushes);
    Serial.println(" times\n"); // \n adds a second newline.
  }
}
