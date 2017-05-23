#include <TinyGPS.h>
#include <stdint.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <SoftwareSerial.h>

TinyGPS gps;

static unsigned int TS_MINX, TS_MAXX, TS_MINY, TS_MAXY;
static unsigned int MapX1, MapX2, MapY1, MapY2;
TouchScreen ts = TouchScreen(17, A2, A1, 14, 300); 

long startTime,lastTime,currTime;
boolean gpsReady = false;

float lat,lon;
unsigned long age, chars;
unsigned short sentences, failed_cs;

void setup() {
  Serial.begin(9600);
  
  Serial1.begin(4800);
  
  Tft.init();
  initTouchScreenParameters();
  
  Tft.drawString("GPS Initializing.",8,166,1,WHITE);
  Tft.drawNumber(0, 8, 178, 1, GREEN);
  lastTime=0;
  
  startTime=millis();
  //Serial.println("Starting");
}

void loop() {
  updateTime(gpsReady);
  //Serial.println("loop top");
  if (!gpsReady) {
    //Serial.println("GPS not ready");
    if(feedGps()) {
      //Serial.println("GPS data ready");
      gps.f_get_position(&lat, &lon, &age);
      //Serial.println(age);      
      if(age != gps.GPS_INVALID_FIX_TIME) {
        gpsReady = true;
        Tft.fillRectangle(7,165,200,20,BLACK);
        Tft.drawString("GPS Ready.",8,300,1,WHITE);
        drawButton(false);
        Tft.drawRectangle(10,56,200,32,BLUE);
        Tft.drawHorizontalLine(12,56,70,BLACK);
        Tft.drawString("Latitude",15,50,1,BLUE);
        Tft.drawRectangle(10,106,200,32,BLUE);
        Tft.drawHorizontalLine(12,106,78,BLACK);
        Tft.drawString("Longitude",15,100,1,BLUE);
        Tft.drawRectangle(10,156,200,32,BLUE);
        Tft.drawHorizontalLine(12,156,110,BLACK);
        Tft.drawString("Date/Time UTC",15,150,1,BLUE); 
        updateLocation();
      }
    }
  } else {
    //<Serial.println("Checking for touch");
    feedGps();
    
    Point p = ts.getPoint();
    p.x = map(p.x, TS_MINX, TS_MAXX, MapX1, MapX2);
    p.y = map(p.y, TS_MINY, TS_MAXY, MapY1, MapY2);
    if(p.z > ts.pressureThreshhold) {
      //Serial.println("Touch registered");
      //Serial.println(p.x);
      //Serial.println(p.y);
      if(wasPressed(p)) {
        drawButton(true);
        updateLocation();
        drawButton(false);
      }
    }
  }
}


boolean wasPressed(Point p) {
  //Serial.println("in wasPressed");
  if(p.x>7 && p.x<114 && p.y>242 && p.y<273) return true;
  return false;
}

void drawButton(boolean pressed) {
  //Serial.println("in drawButton");
  if(pressed) {
    Tft.drawRectangle(8,243,106,29,WHITE);
    Tft.fillRectangle(9,244,105,28,BLACK);
    Tft.drawString("Update",15,250,2,GREEN);
  } else {
    Tft.drawRectangle(8,243,106,29,WHITE);
    Tft.fillRectangle(9,244,105,28,GREEN);
    Tft.drawString("Update",15,250,2,BLACK);
  }
}

void updateLocation() {
  //Serial.println("in updateLocation");
  gps.f_get_position(&lat, &lon, &age);
  
  //gps.stats(&chars, &sentences, &failed_cs);
  //Serial.print(chars); Serial.print(", "); Serial.print(sentences); Serial.print(", ");
  //Serial.println(failed_cs);

  int posX = 36;
  Tft.fillRectangle(12,58,196,28,BLACK);
  Tft.fillRectangle(12,108,196,28,BLACK);
  Tft.fillRectangle(12,158,196,28,BLACK);
  if(lat<0) posX=20;
  Tft.drawFloat(lat,7,posX,66,2,RED);
  if(lon<0) {
    posX=20;
  } else {
    posX=36;
  }
  Tft.drawFloat(lon,7,posX,116,2,RED);  
  
  unsigned long date, time;
  gps.get_datetime(&date, &time, &age);
  
  if (date == 0) {
    Tft.drawString("xxxx/xx/xx",20,170,1,RED);
  } else {  
    int year  = date%100;
    int month = (date%10000)/100;
    int day   = date/10000;
    year += 2000;
  
    Tft.drawNumber(year,20,170,1, RED);
    Tft.drawString("/",52,170,1, RED);
    if (month < 10) {
      Tft.drawNumber(0,60,170,1,RED);
      Tft.drawNumber(month,68,170,1,RED);
    } else {
      Tft.drawNumber(month,60,170,1,RED);
    }
    Tft.drawString("/",76,170,1,RED);
    if(day < 10) {
      Tft.drawNumber(0,84,170,1,RED);
      Tft.drawNumber(day,92,170,1,RED);
    } else {
      Tft.drawNumber(day,84,170,1,RED);
    }
  }
  
  int hour,minute,second;
  time/=100; // ignore hundredths of a second
  second = time%100;
  minute = (time%10000)/100;
  hour = time/10000;
  if(hour<10) {
    Tft.drawNumber(0,108,170,1,RED);
    Tft.drawNumber(hour,116,170,1,RED);
  } else {
    Tft.drawNumber(hour,108,170,1,RED);
  }
  Tft.drawString(":",124,170,1,RED);
  if(minute<10) {
    Tft.drawNumber(0,132,170,1,RED);
    Tft.drawNumber(minute,140,170,1,RED);
  } else {
    Tft.drawNumber(minute,132,170,1,RED);
  }
  Tft.drawString(":",148,170,1,RED);
  if(second<10) {
    Tft.drawNumber(0,156,170,1,RED);
    Tft.drawNumber(second,164,170,1,RED);
  } else {
    Tft.drawNumber(second,156,170,1,RED);
  }
}

bool feedGps() {
  char in;
  while (Serial1.available()) {
    in = Serial1.read();
    //Serial.print(in);
    if (gps.encode(in)) {
      //gpsdump(gps);
      return true;
    }
  }
  return false;
}

void gpsdump(TinyGPS &gps) {
  float flat, flon;
  unsigned long age, chars;
  unsigned short sentences, failed_cs;
  gps.f_get_position(&flat, &flon, &age);
  Serial.print(flat, 7); Serial.print(", "); 
  Serial.println(flon, 7);
  gps.stats(&chars, &sentences, &failed_cs);
  Serial.print(chars); Serial.print(", "); Serial.print(sentences); Serial.print(", ");
  Serial.println(failed_cs);
  
}

void updateTime(boolean gpsReady) {
  int x,y,width;
  currTime = (long)(millis() - startTime)/1000;
  
  if(currTime != lastTime) {
    lastTime = currTime;
    if (gpsReady) {
      width = 10 + 8 * floor(log10(currTime));
      x = 223 - width;
      y = 299;
    } else {
      x = 7;
      y = 177;
      width = 100;
    }
    Tft.fillRectangle(x,y,width,10,BLACK);
    Tft.drawNumber(currTime, ++x, ++y, 1, GREEN);
  }
}

void initTouchScreenParameters()
{
  //This function initializes Touch Screen parameters based on the detected TFT Touch Schield hardware
 
  if(Tft.IC_CODE == 0x5408) //SPFD5408A TFT driver based Touchscreen hardware detected
  {
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ts = TouchScreen(54, A1, A2, 57, 300); //init TouchScreen port pins
#else 
    ts = TouchScreen(14, A1, A2, 17, 300); //init TouchScreen port pins
#endif
    //Touchscreen parameters for this hardware
    TS_MINX = 120;
    TS_MAXX = 910;
    TS_MINY = 120;
    TS_MAXY = 950;
 
    MapX1 = 239;
    MapX2 = 0;
    MapY1 = 0;
    MapY2 = 319;
  }
  else //ST7781R TFT driver based Touchscreen hardware detected
  {
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ts = TouchScreen(57, A2, A1, 54, 300); //init TouchScreen port pins
#else 
    ts = TouchScreen(17, A2, A1, 14, 300); //init TouchScreen port pins
#endif 
 
    //Touchscreen parameters for this hardware
    TS_MINX = 140;
    TS_MAXX = 900;
    TS_MINY = 120;
    TS_MAXY = 940;
 
    MapX1 = 239;
    MapX2 = 0;
    MapY1 = 319;
    MapY2 = 0;
  }
}
