/**
  *  Program: RGB_led
  *  Author:  Erik Nedwidek
  *  Date:    2013/09/10
  *
  *  REQUIRED LIBRARIES: ICMPPing https://github.com/BlakeFoster/Arduino-Ping
  *                      RS2760249 https://github.com/nedwidek/Arduino-Libraries/tree/master/RS2760249
  *
  *  Uses the Ethernet Shield to ping a server defined below 10x a second. The
  *  measured ping is displayed using a Radioshack 1m RGB LED strip (276-0249).
  *
  *  No wiring diagram, just place the Ethernet sheild on your Uno. Refer to
  *  the manual that came with your RGB LED strip. For mine the black wire
  *  goes to ground. The red wire goes to Vin (you need to use 9V-12V to power
  *  the strip. 5V won't work). The serial (green) wire goes to A0. If you want
  *  to use a different pin, 
  *
  *  **** IMPORTANT ****
  *  Before compiling this program you will need to change the values for the
  *  network settings and top and bottom of the ping scale (in milliseconds).
  */
  
#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>
#include <RS2760249.h>

// Change these values as needed!
#define SCALE_BOTTOM 50 // pings at or below this level will only light the first led segment.
#define SCALE_TOP 80 // pings above this will light the whole strip
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address for Ethernet shield
byte ip[]  = { 192, 168, 100, 200 }; // IP address to use on local network
byte DNS[] = { 8, 8, 8, 8 };   // DNS server address
byte gw[]  = { 192, 168, 100, 1 };   // Gateway address
byte sn[]  = { 255, 255, 255, 0 };   // Subnet mask
//IPAddress pingIp( 192, 168, 101, 1 );
IPAddress pingIp( 192, 168, 100, 1 ); // IP address to ping on the internet

#define GREEN 0x0000FF
#define RED 0xFF0000
#define YELLOW 0xFF00FF

unsigned long pattern[10] = { GREEN, GREEN, GREEN, GREEN, GREEN, YELLOW, YELLOW, RED, RED, RED };

SOCKET pingSocket = 0;
ICMPPing ping(pingSocket, (uint16_t)random(0,255));
// The strip is connected to A0.
RS2760249 strip(0);

void setup() {  
  Ethernet.begin(mac, ip, DNS, gw, sn);

  // Light the whole strip up for a quarter sencond and then clear it.
  strip.sendPattern(pattern, 10);
  delay(250);
  strip.clear();
}

void loop() {
  
  // Ping the server and if successful, display the time.
  ICMPEchoReply echoReply = ping(pingIp, 4);
  if(echoReply.status == SUCCESS) {
    int time = millis() - echoReply.data.time;
    int bars = 1;
    if(time > SCALE_TOP) { 
      bars = 10; 
    } else if(time >= SCALE_BOTTOM) {
      bars = (time - SCALE_BOTTOM) / ((SCALE_TOP - SCALE_BOTTOM)/10);
    }
    strip.clear();
    // Create a temporary pattern that is filled from the master pattern
    // with the number of bars to show and then fill the rest with *off*
    // LEDs.
    unsigned long data[10];
    int i;
    for(i=0; i<bars; i++) {data[i] = pattern[i]; }
    for(; i<10; i++) { data[i] = 0x000000; }
    strip.sendPattern(data, bars);
  }
  delay(100);
}
