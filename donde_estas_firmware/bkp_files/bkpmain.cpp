#define DEBUG_MODE

#include <BlockNot.h>
#include "controllers/configuration_files_controller.h"
#include "controllers/wifimanager_controller.h"
#include "controllers/telemetry_controller.h"
#include "controllers/web_rtc_controller.h"
#include "controllers/datalogger_controller.h"
#include "controllers/leds_controller.h"
// #include "controllers/reset_button_handler.h"

#define RESET_TIMER 86400000ULL // 1 day in milliseconds
#define DATALOGGER_TIMER 10ULL // 10 seconds

BlockNot dataloggerNonBlocking;
BlockNot telemetryNonBlocking;

void setup() {
  #ifdef DEBUG_MODE
  Serial.begin(115200);
  delay(500);
  #endif
  
  // initResetButtonTimer();
  initLEDs();
  powerLedOn();
  initSpiffs();
  initConfigurationFiles();
  initWifiManager();

  initWebRtc();
  dataloggerNonBlocking = BlockNot(DATALOGGER_TIMER, SECONDS);
  telemetryNonBlocking = BlockNot(INTERVAL_READING_SECONDS, SECONDS);

  listDir("/",1);
  telemetryReadAndTransfer();
}

void loop() {
  // runResetButtonTimer(&enableConfigurationMode);
  if(millis() > RESET_TIMER) {
    ESP.restart();
  }
  if(telemetryNonBlocking.TRIGGERED) {
    telemetryReadAndTransfer();
  }
  if(dataloggerNonBlocking.TRIGGERED) {
    dataloggerTransfer();
  }
}