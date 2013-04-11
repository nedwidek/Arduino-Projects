/**
  *  Program: MultiSenso
  *  Author:  Erik Nedwidek
  *  Date:    2013/03/18
  *  License: BSD
  *  Requires:
  *     Bounce            - http://playground.arduino.cc/code/bounce
  *     Adafruit_MPL115A2 - https://github.com/adafruit/Adafruit_MPL115A2
  *     ParallaxLCD       - https://github.com/nedwidek/Arduino-Libraries/tree/master/ParallaxLCD
  *     Gyroscope         - https://github.com/nedwidek/Arduino-Libraries/tree/master/Gyroscope
  *     TMP36             - https://github.com/nedwidek/Arduino-Libraries/tree/master/TMP36
  *     Compass           - https://github.com/nedwidek/Arduino-Libraries/tree/master/Compass
  *
  *  Uses:
  *     TMP36 temperature sensor
  *     Parallax 3-Axis Compass (29133-RT)
  *     Parallax LCD 2x16 LCD Display (#27976-RT, #27977-RT, #27978-RT, #27979-RT)
  *     Adafruit MPL115A2 barometer
  *     Parallax 3-Axis Gyroscope (27911-RT)
  *
  *  Allows user to select between the outputs of multiple sensors using a momentary switch. Use included 
  *  Fritzing files for setup.
  */

#include <Bounce.h>
#include <SoftwareSerial.h>
#include <ParallaxLCD.h>
#include <Wire.h>
#include <Adafruit_MPL115A2.h>
#include <Gyroscope.h>
#include <Compass.h>
#include <TMP36.h>

// Debouncer for the switch as well as objects for the sensors and LCD screen.
Bounce debouncer(8, 20);
ParallaxLCD lcd(7, 19200);
TMP36 tmp36(0);
Adafruit_MPL115A2 mpl115a2;
Gyroscope gyro;
Compass compass;


// state management and delay (ms) between sensor readings. 
int state = 0;
long lastSensorRead = 0;
long sensorDelay = 500;

void setup() {
  Serial.begin(9600);

  // Set the pin mode for the switch
  pinMode(8, INPUT);

  // Welcome the user.
  lcd.displayMode(true, false, false);
  lcd.clear();
  lcd.backlight(true);
  lcd.print("Welcome to");
  lcd.moveCursor(1,0);
  lcd.print("MultiSensor!");
  delay(1000);
  lcd.clear();
  
  // Display temperature and start other sensors.
  setupTemperature();
  mpl115a2.begin();
  gyro.setModeDefault();
  compass.setGain8_1();
  compass.setModeContinuous();
}

void loop() {
  
  // read the momentary switch.
  handleButton();
  
  // Display the correct sensor if at least the requisite # of ms have passed since last read.
  if((millis() - lastSensorRead) > sensorDelay) {
    switch(state) {
    case 0:
      doTemperature();
      break;
    case 1:
      doGyroscope();
      break;
    case 2:
      doCompass();
      break;
    case 3:
      doBarometer();
      break;
    }
    lastSensorRead = millis();
  }
}


// Listen fror the falling edge of the button signal.
void handleButton() {
  debouncer.update();
  if(debouncer.fallingEdge()) {
    state++;
    if(state > 3) {
      state = 0;
    }
    switch(state) {
    case 0:
      setupTemperature();
      break;
    case 1:
      setupGyroscope();
      break;
    case 2:
      setupCompass();
      break;
    case 3:
      setupBarometer();
      break;
    }
  }
}

void setupTemperature() {
  sensorDelay = 1000;

  lcd.clear();
  lcd.print("Temperature:");
  doTemperature();
}

void doTemperature() {
  float temperature = tmp36.temperatureF();
  lcd.moveCursor(1,3);
  lcd.print(temperature); 
  lcd.print(" F");
}

void setupGyroscope() {
  sensorDelay = 1000;

  lcd.clear();
  lcd.print("Gyroscope:");
  doGyroscope();
}

void doGyroscope() {
  
  // Read the gyro and scale the result so we can fit it on the LCD.
  gyro.read();
  lcd.moveCursor(1,0);
  printGyroReading(gyro.x/60); 
  lcd.print(" ");
  printGyroReading(gyro.y/60); 
  lcd.print(" ");  
  printGyroReading(gyro.z/60);
} 

void setupCompass() {
  sensorDelay = 500;

  lcd.clear();
  lcd.print("Compass:");
  doCompass();
}

void doCompass() {
  float heading = compass.heading();
  lcd.moveCursor(1,3);
  printCompassHeading(heading);
}

void setupBarometer() {
  sensorDelay = 1000;

  lcd.clear();
  lcd.print("Barometer:");
  doBarometer();
}

void doBarometer() {
  float pressure = 0, temperature = 0;

  mpl115a2.getPT(&pressure, &temperature);
  lcd.moveCursor(1,3);
  lcd.print(pressure); 
  lcd.print(" kPA");  
}

void printGyroReading(int value) {
  if(value > -1)  { 
    lcd.print(" "); 
  }
  if(abs(value) < 100) { 
    lcd.print(" "); 
  }
  if(abs(value) < 10)  { 
    lcd.print(" "); 
  }
  lcd.print(value);
}

void printCompassHeading(float heading) {
  if(heading < 100) { 
    lcd.print(" "); 
  }
  if(heading < 10)  { 
    lcd.print(" "); 
  }
  lcd.print(heading);
  lcd.print("    ");
}

