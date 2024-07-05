#define DEBUG_MODE

#ifdef TTGO_BOARD
  #define SIM800L_AXP192_VERSION_20200327
  #include "utilities.h"
  #define RXD2 0
  #define TXD2 4
#endif

#ifndef TTGO_BOARD
  #define RXD2 33
  #define TXD2 32  
#endif


#include <TinyGPS++.h>


HardwareSerial neogps(1);

TinyGPSPlus gps;


void print_speed(){
  if (gps.location.isValid() == 1)   {
    Serial.print("Lat: ");
    Serial.println(gps.location.lat(),6);
    Serial.print("Lng: ");
    Serial.println(gps.location.lng(),6);
    Serial.print("Speed: ");
    Serial.println(gps.speed.kmph());
    Serial.print("SAT:");
    Serial.println(gps.satellites.value());
    Serial.print("ALT:");
    Serial.println(gps.altitude.meters(), 0);
  }
  else  {
    Serial.print("No Data");
  }  
}

void setup() {
    #ifdef DEBUG_MODE
    Serial.begin(115200);
    delay(500);
    #endif
    neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
    delay(500);
}

void loop() {
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (neogps.available()) {
      if (gps.encode(neogps.read())) {
        newData = true;
      }
    }
  }

  //If newData is true
  if(newData == true) {
    newData = false;
    Serial.println(gps.satellites.value());
    print_speed();
  }
  else {
    Serial.print("No Data Loop");
  }  
}
