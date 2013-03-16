/**
  *  Program: Hello_World
  *  Author:  Erik Nedwidek
  *  Date:    2013/03/15
  *  License: BSD
  *  
  *  Basic Hello World (Arduino) program. Uses the onboard pin 13 LED so there
  *  are no external requirements.
  *
  *  As a twist on the normal just blink the LED, we'll actually make it blink
  *  the morse code for "hello arduino". This program is not meant to be the
  *  most efficient implementation. What is done here is to get the learner to
  *  look into memory allocation, pointers, and related functions. As well as
  *  looking at bits and bytes through bitwise operators.
  *
  */
  
// We'll use bit masks to hold the dit and dah patterns as well as the lengths. 
// The lengths would be simpler to just say if the pattern was 1, 2, 3, or 4 in
// length, but using a mask allows us to do more bit math. Not because it is more
// efficient, but because thinking at the level of bytes and bits is actually a
// good thing for a beginner to do.
// Here's something to remember, in a hexadecimal number, each digit refers to
// 4 bits (a nibble). So 0x7A in binary is 01111010. Or splitting the nibbles to
// make it easier to see: 0111 1010.
//                            A     B     C     D     E     F     G     H     I     J     K     L     M     N     O     P     Q     R     S     T     U     V     W     X     Y     Z
byte morse_characters[] = { 0x01, 0x08, 0x0A, 0x04, 0x00, 0x02, 0x06, 0x00, 0x00, 0x07, 0x05, 0x0B, 0x03, 0x02, 0x07, 0x06, 0x0C, 0x02, 0x00, 0x01, 0x01, 0x01, 0x03, 0x09, 0x0B, 0x0C };
byte morse_masks[]      = { 0x03, 0x0F, 0x0F, 0x07, 0x01, 0x0F, 0x07, 0x0F, 0x03, 0x0F, 0x07, 0x0F, 0x03, 0x03, 0x07, 0x0F, 0x0F, 0x07, 0x07, 0x01, 0x07, 0x0F, 0x07, 0x0F, 0x0F, 0x0F };
byte sequence[]         = { 0x08, 0x04, 0x02, 0x01 };

// This is the duration of a dit in milliseconds. A dah, is 3x this duration.
int duration = 50;

// We are using the onboard LED, but we could use an external LED on a different pin by changing this.
int ledPin = 13;


// In Arduino, the setup method is called once when the micro-controller powers up.
// As it's name indicates, it is where an necessary setup is done.
void setup() { 
  
  // We are going to be writing to the designated pin.
  pinMode(ledPin, OUTPUT);
}

// Once the micro-controller powers up and the setup method is done, the loop
// method is executed repeatedly.
void loop() {
  // tap will tap out our morse code message.
  tap("hello world");
  
  // delay 2 seconds before we do this all again.
  delay(2000);
}

/**
 * "Taps" the morse code message using the LED.
 * 
 * @author nedwidek (2013/03/15)
 *
 * @params message The message to display.
 */
void tap(char* message) {
  
  while(*message != '\0') {
    if(*message == 32) {
      morse_space();
    } else {
      morse_char(*message);
    }
    message++;
  }    
}

/**
 * Displays a morse space. This is the same length as 7 dits with the LED off.
 * This only pauses for 4 because 3 were already done for the prior character.
 * 
 * @author nedwidek (2013/03/15)
 */
void morse_space() {
  digitalWrite(ledPin, LOW);
  delay(duration * 4);
}

/**
 * Displays a morse character. Only A-Z are displayed, all other characters are ignored.
 * 
 * @author nedwidek (2013/03/15)
 *
 * @param character The character to display.
 */
void morse_char(char character) {
  
  // Calculate our array index [0..25]. It must be in the range of 97 to 122 or 65 to 90.
  if(character > 96) {
    character -= 97;
  } else if(character > 64) {
    character -= 65;
  } else {
    // The character is too low to be in either range of acceptable characters.
    return;
  }
  
  if(character > 24) {
    // The character was too high to be in either range.
    return;
  }
  
  byte char_pattern = morse_characters[character];
  byte char_mask    = morse_masks[character];
  
  for(int i=0; i < sizeof(sequence); i++) {
  }
  
  // Pause for a dah between characters.
  digitalWrite(ledPin, LOW);
  delay(duration*3);
}
  


