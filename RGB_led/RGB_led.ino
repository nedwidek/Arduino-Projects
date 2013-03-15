/**
  *  Program: RGB_led
  *  Author:  Erik Nedwidek
  *  Date:    2012/10/24
  *
  *  REQUIRED LIBRARIES: none
  *
  *  Drives an RGB LED (RadioShack 276-0028) through random colors using an eased or linear 
  *  transition. If you do not know what PWM is, read http://www.arduino.cc/en/Tutorial/PWM.
  *  Normally LEDs are wired so that the cathod is to ground and the anode is to a pin on the
  *  Arduino. When the pin goes HIGH, the LED is lit. The RGB LED is wired in reverse with the
  *  anode wired to +5V and the cathodes for each color LED goes to a PWM pin on the Arduino.
  *  In this case the LED is off when the pin goes HIGH the LED is unlit and lit when it goes
  *  LOW.
  *
  *  Colors on the LED are created by mixing the red, green, and blue. PWM is used to give a
  *  scale of reds, blues, and greens. Note that we are using 0 - 255 internally to represent
  *  off to full on. Since the wiring is reversed, the setColor method inverts this value
  *  by XORing it with 255. 
  *
  *  See accompanying schematic for hardware setup. 
  */
  
// Change if you are using different PWM pins on your Arduino than the wiring in the schematic.
const int LED_R = 11;
const int LED_G = 10;
const int LED_B = 9;

// Internal use constants.
const int LINEAR = 1;
const int EASED = 2;
const float RAD90 = 1.5708;

// Internal globals.
int Rold, Gold, Bold;
int Rnew, Gnew, Bnew;
int duration;

/**
  *  Function: setup
  *
  *  Sets up pin modes and does a quick "boot" sequence to verify the LED wiring. LED should
  *  Fade in and out for each color (red, green, blue).
  */
void setup() {
  // Set the pins for OUTPUT
  pinMode(LED_R, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_G, OUTPUT);

  // Boot sequence to verify wiring.
  // Fade in and out red
  xFade(0, 0, 0, 255, 0, 0, 100, EASED);
  xFade(255, 0, 0, 0, 0, 0, 100, EASED);
  
  // Fade in and out green
  xFade(0, 0, 0, 0, 255, 0, 100, EASED);
  xFade(0, 255, 0, 0, 0, 0, 100, EASED);
  
  // Fade in and out blue
  xFade(0, 0, 0, 0, 0, 255, 100, EASED);
  xFade(0, 0, 255, 0, 0, 0, 100, EASED);
  
  // Seed the random number generator with noise from an analog pin.
  randomSeed(analogRead(0));
  // Start from off.
  Rnew = Bnew = Gnew = 0;
}

/**
  *  Function: loop
  *
  *  Picks a new random color and fades to it from the previous color. 
  */
void loop() {
  // Move the last color to the "old" globals.
  Rold = Rnew;
  Gold = Gnew;
  Bold = Bnew;

  // Pick a new color  with random(0 ... 255).  
  Rnew = random(255);
  Bnew = random(255);
  Gnew = random(255);
 
  // Use a random time to do the crossfade.
  duration = random(50, 200); 
  
  // Do the crossfade. EASED will give a smoother appearance. LINEAR gives sharper
  // transitions.
  xFade(Rold, Gold, Bold, Rnew, Gnew, Bnew, duration, EASED);  
}


/**
  *  Function: xFade
  *  Ro : old red value
  *  Go : old green value
  *  Bo : old blue value
  *  Rn : new red value
  *  Gn : new green value
  *  Bn : new blue value
  *  time : The amount of time to do the fade (~100ths of a second)
  *  mode : The type of fade. LINEAR goes between the two colors in equal steps. EASED
  *         will go between the two colors using the sin function to provide a smoother
  *         transition.
  *
  *  Crossfade between two colors.
  */
void xFade(int Ro, int Go, int Bo, int Rn, int Gn, int Bn, long time, int mode) {
  if (mode == EASED) {
    easedFade(Ro, Go, Bo, Rn, Gn, Bn, time);
  } else {
    linearFade(Ro, Go, Bo, Rn, Gn, Bn, time);
  }

}
 
/**
  *  Function: linearFade
  *
  *  Implements the eased fade stepping. See xFade for argument definitions
  */           
void linearFade(int Ro, int Go, int Bo, int Rn, int Gn, int Bn, long time) {           
  int tempR, tempG, tempB;
  float stepR, stepG, stepB;
  
  stepR = (Rn - Ro) * 1.0 / (time);
  stepG = (Gn - Go) * 1.0 / (time);
  stepB = (Bn - Bo) * 1.0 / (time);
  for (long i=0; i<time; i++) {
    tempR = Ro + (stepR * i);
    tempG = Go + (stepG * i);
    tempB = Bo + (stepB * i);
    setColor(tempR, tempG, tempB);
    delay(10);
  }
  setColor(Rn, Gn, Bn); 
}

/**
  *  Function: easedFade
  *
  *  Implements the eased fade stepping. See xFade for argument definitions
  */
void easedFade(int Ro, int Go, int Bo, int Rn, int Gn, int Bn, long time) {           
  int tempR, tempG, tempB, rangeR, rangeG, rangeB;
  float radStep, tmpAng, tmpSin;
  
  radStep = RAD90 / time;
  rangeR = Rn - Ro;
  rangeG = Gn - Go;
  rangeB = Bn - Bo;
  
  for (long i=0; i<time; i++) {
    tmpSin = sin(radStep * i);
    tempR = Ro + rangeR * tmpSin;
    tempG = Go + rangeG * tmpSin;
    tempB = Bo + rangeB * tmpSin;
    setColor(tempR, tempG, tempB);
    delay(10);
  }
  setColor(Rn, Gn, Bn); 
}

/**
  *  Function: setColor
  *  R: the amound of red (0-255).
  *  G: the amount of green (0-255).
  *  B: the amount of blue (0-255).
  *
  *  Sets the color on the LED. The amount of each color is 0 (0%)to 255 (100%).
  */
void setColor(int R, int G, int B) {
  // We will XOR each color value with 255 since hardwarewise 255 represents off and 0
  // is 100% on. 
  analogWrite(LED_R, R^255); 
  analogWrite(LED_G, G^255);
  analogWrite(LED_B, B^255);
}
