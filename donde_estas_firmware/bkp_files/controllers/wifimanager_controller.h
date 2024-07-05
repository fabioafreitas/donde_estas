#ifndef _WIFIMANAGER_CONTROLLER_H_
#define _WIFIMANAGER_CONTROLLER_H_

// #define WIFIMANAGER_CONTROLLER_DEBUG

#include <WiFiManager.h>
#include "leds_controller.h"
#include "configuration_files_controller.h"

WiFiManager wm;

void wifiManagerResetEsp() {
  #ifdef WIFIMANAGER_CONTROLLER_DEBUG
  Serial.println("[WIFI MANAGER_CONTROLLER] ESP Restart");
  #endif
  ESP.restart();
}

void enableConfigurationMode() {
  #ifdef WIFIMANAGER_CONTROLLER_DEBUG
  Serial.println("[WIFI MANAGER_CONTROLLER] Configuration Mode Enabled");
  #endif
  wm.erase();
  ESP.restart();
}

void initWifiManager(){
  #ifdef WIFIMANAGER_CONTROLLER_DEBUG
  Serial.println("[WIFI MANAGER_CONTROLLER] init wifi manager");
  #endif
  rgbStatusConfigutationMode();
  wm.setConfigPortalTimeoutCallback(&wifiManagerResetEsp);
  wm.setAPClientCheck(true);
  wm.setConfigPortalTimeout(60);
  wm.autoConnect(SMARTRURAL_DEVICE_ID.c_str());
  rgbOff();

}

#endif /*_WIFIMANAGER_CONTROLLER_H_*/