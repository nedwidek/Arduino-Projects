// Sketch to demonstrate the use of the Parallax pIR motion sensor with
// interrupts.
// Author: Erik Nedwidek
// Date: 2012/7/17
// License: BSD

// Include classes to talk to the Parallax Serial LCD
#include <SoftwareSerial.h>
#include <ParallaxLCD.h>

// Pins for the sensor and LCD. Baud rate for the serial LCD.
int pirPin = 3;
int lcdPin = 7;
int lcdBaud = 19200;

// Housekeeping variables to debounce the pIR sensor. The transition from
// HIGH to LOW is not completely clean.
long lastInterrupt = millis();
long debounce = 5;

ParallaxLCD lcd(lcdPin, lcdBaud);

// Setup everything we need. Play music and display a mock calibrating message.
void setup() {
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
  lcd.clear();
  lcd.backlight(true);
  lcd.displayMode(true, false, false);
  lcd.print("Calibrating...");
  playFalling();
  delay(5000);
  lcd.clear();
  lcd.print("Done.");
  playReady();
  delay(4000);
  lcd.clear();
  lcd.print("Are you still\rthere?");
  
  // Attaching interrupt 1 (pin 3 on Uno/Mega) to spotted() method.
  attachInterrupt(1, spotted, CHANGE);
}

void loop() {
}

// Called by interrupt when signal on pin 3 goes from HIGH to LOW or LOW to HIGH.
void spotted() {
  // The pIR sensor doesn't cleanly go from HIGH to LOW. This method will be called
  // twice on LOW. If the method has been called twice within the debounce period
  // just return.
  if((millis() - lastInterrupt) < debounce) return;
  
  // LOW to HIGH transition (motion sensed)
  if(digitalRead(pirPin) == HIGH) {
    lcd.clear();
    lcd.print("There you are!");
    lcd.set8thNote();
    lcd.playNote(lcd.C);
    lcd.setQuarterNote();
    lcd.playNote(lcd.G);
  } else {
    // HIGH to LOW trasition (no motion sensed)
    lcd.clear();
    lcd.print("Where did you\rgo?");
    lcd.set8thNote();
    lcd.playNote(lcd.G);
    lcd.setQuarterNote();
    lcd.playNote(lcd.C);
  }
  
  // Save the time we were last called so we can debounce the signal.
  lastInterrupt = millis();
}

void playFalling() {
  lcd.set64thNote();
  lcd.playNote(lcd.G);
  lcd.playNote(lcd.F);
  lcd.playNote(lcd.E);
  lcd.playNote(lcd.D);
  lcd.playNote(lcd.C);
  lcd.playNote(lcd.B);
  lcd.playNote(lcd.A);
}

void playReady() {
  lcd.setQuarterNote();
  lcd.playNote(lcd.F);
  lcd.set8thNote();
  lcd.playNote(lcd.F);
  lcd.playNote(lcd.G);
  lcd.setQuarterNote();
  lcd.playNote(lcd.G_SHARP);
  lcd.playNote(lcd.F);
  lcd.setScale4();
  lcd.setFullNote();
  lcd.playNote(lcd.F);
  lcd.setScale3();
}
