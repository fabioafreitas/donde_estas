#ifndef _LEDS_HANDLER_H_
#define _LEDS_HANDLER_H_

//#define LEDS_HANDLER_DEBUG_MODE

#ifndef LED_RGB_RED_PIN
#define LED_RGB_RED_PIN 25
#endif

#ifndef LED_RGB_GREEN_PIN
#define LED_RGB_GREEN_PIN 26
#endif

#ifndef LED_RGB_BLUE_PIN
#define LED_RGB_BLUE_PIN 27
#endif

#ifndef POWER_LED_PIN
#define POWER_LED_PIN 33
#endif

#include <RGBLed.h>

RGBLed ledRGB(LED_RGB_RED_PIN, LED_RGB_GREEN_PIN, LED_RGB_BLUE_PIN, RGBLed::COMMON_CATHODE);

void initLEDs() {
  pinMode(POWER_LED_PIN, OUTPUT);
}

// RED led handlers
void powerLedOff() {
  digitalWrite(POWER_LED_PIN, LOW);
}

void powerLedOn() {
  digitalWrite(POWER_LED_PIN, HIGH);
}


// RGB Handlerss
void rgbOff() {
  ledRGB.off();
}

void rgbGreen() {
  ledRGB.setColor(RGBLed::GREEN);
}

void rgbBlue() {
  ledRGB.setColor(RGBLed::BLUE);
}

void rgbRed() {
  ledRGB.setColor(RGBLed::RED);
}


void rgbStatusHardwareError() {
  int blinkTimer = 500;
  for(int i = 0 ; i < 10000 ; i += blinkTimer) {
    ledRGB.setColor(RGBLed::RED);
    delay(blinkTimer);
  }
  ledRGB.off();
}

void rgbStatusConfigutationMode() {
  ledRGB.setColor(RGBLed::CYAN);
}

void rgbTelemetryTransferFailed() {
  ledRGB.setColor(RGBLed::RED);      
  delay(3000);
  ledRGB.off();
}

void rgbTelemetryTransferSuccess() {
  ledRGB.setColor(RGBLed::GREEN);      
  delay(3000);
  ledRGB.off();
}


#endif /*_LEDS_HANDLER_H_*/
