/**
  *  Program: Serial_Example
  *  Author:  Erik Nedwidek
  *  Date:    2013/03/20
  *  License: BSD
  *  Requires:
  *     ParallaxLCD
  *       Hardware - Parallax.com (or some RadioShack stores) 27976-rt, 27977-rt, or 27979-rt.
  *       Library  - http://github.com/nedwidek/Arduino-Libraries/tree/master/ParallaxLCD
  *
  *  Takes input over the serial monitor (Tools -> Serial Monitor or Ctrl+Shift+M). Displays
  *  on Parallax (or compatible) Serial LCD display.
  *
  */

#include "SoftwareSerial.h"
#include <ParallaxLCD.h>

// Pin and buad rate for the Parallax LCD.
int lcdPin = 7;
int lcdBaud = 19200;

// Baud rate for Serial Monitor
int serialBaud = 9600;

// Instantiate the class to handle the LCD output.
ParallaxLCD lcd(lcdPin, lcdBaud);

// Setup the Serial line. Turn on LCD backlight.
void setup() {
  Serial.begin(serialBaud);
  lcd.backlight(true);
}

void loop() {
  int count = 0;
  char character;
  
  // Did the user send a message?
  while(Serial.available() > 0) {
    
    count++;
    
    // Read the character and store it.
    character = Serial.read();
    
    // We'll convert any newlines or carriate returns to spaces.
    if(character == '\n' || character == '\r') character = ' ';
    
    // Finally we display the character on the LCD.
    lcd.print(character);
  }
  
  // Send a confirmation back
  if(count > 0) {
    Serial.println("Wrote: "); Serial.println(count);
  }
}


