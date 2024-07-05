#ifndef _CONFIGURATION_FILES_H_
#define _CONFIGURATION_FILES_H_

// enable the macro bellow to show logs in console
#define CONFIGURATION_FILES_DEBUG_MODE

#include "ArduinoJson.h"
#include "spiffs_controller.h"

String SMARTRURAL_DEVICE_ID;
String AQUATIC_HARDWARE_VERSION;
String AQUATIC_FIRMWARE_VERSION;
String THINGSBOARD_URL;
String THINGSBOARD_HTTP_ACCESS_TOKEN;
unsigned long INTERVAL_READING_SECONDS;

void createConfigurationFiles() {
  if(!existsFile("/config.json")) {
    JsonDocument doc;

    doc["smartrural_device_id"] = "smartrural_factory_device";
    doc["aquatic_hardware_version"] = "v0.7";
    doc["aquatic_firmware_version"] = "v0.7";
    doc["thingsboard_url"] = "thingsboard.cloud";
    doc["thingsboard_http_access_token"] = "testing_device";
    doc["interval_reading_seconds"] = 60;

    String configJson;
    serializeJsonPretty(doc, configJson);
    bool status = writeFile("/config.json", configJson.c_str());

    #ifdef CONFIGURATION_FILES_DEBUG_MODE
    if(status) {
      Serial.println("[CONFIGURATION_FILES] /config.json creation success");
    } else {
      Serial.println("[CONFIGURATION_FILES] /config.json creation error");
    }
    #endif
  }
}

void initConfigurationFiles() {
    createConfigurationFiles();

    JsonDocument doc;
    String configContent = readFile("/config.json");
    DeserializationError error = deserializeJson(doc, configContent);

    if (error) {
        #ifdef CONFIGURATION_FILES_DEBUG_MODE
        Serial.print("[CONFIGURATION_FILES] deserializeJson() failed: ");
        Serial.println(error.c_str());
        #endif
        rgbStatusHardwareError();
        return;
    }

    SMARTRURAL_DEVICE_ID = (const char*) doc["smartrural_device_id"];
    AQUATIC_HARDWARE_VERSION = (const char*) doc["aquatic_hardware_version"];
    AQUATIC_FIRMWARE_VERSION = (const char*) doc["aquatic_firmware_version"];
    THINGSBOARD_URL = (const char*) doc["thingsboard_url"];
    THINGSBOARD_HTTP_ACCESS_TOKEN = (const char*) doc["thingsboard_http_access_token"];
    INTERVAL_READING_SECONDS = (unsigned long) doc["interval_reading_seconds"]; //INTEIRO

    #ifdef CONFIGURATION_FILES_DEBUG_MODE
    Serial.println("\n[CONFIG INIT]\n---------------------------------------------");
    Serial.println("SMARTRURAL_DEVICE_ID = " + String(SMARTRURAL_DEVICE_ID));
    Serial.println("THINGSBOARD_URL = " + String(THINGSBOARD_URL));
    Serial.println("THINGSBOARD_HTTP_ACCESS_TOKEN = " + String(THINGSBOARD_HTTP_ACCESS_TOKEN));
    Serial.println("INTERVAL_READING_SECONDS = " + String(INTERVAL_READING_SECONDS) + " segundos");
    Serial.println("---------------------------------------------");
    #endif
}

void updateConfigs() {
    createConfigurationFiles();

    JsonDocument doc;

    doc["smartrural_device_id"] = SMARTRURAL_DEVICE_ID;
    doc["thingsboard_url"] = THINGSBOARD_URL;
    doc["thingsboard_http_access_token"] = THINGSBOARD_HTTP_ACCESS_TOKEN;
    doc["interval_reading_seconds"] = INTERVAL_READING_SECONDS;

    String configJson;
    serializeJsonPretty(doc, configJson);
    
    #ifdef CONFIGURATION_FILES_DEBUG_MODE
    Serial.println("---------------------------------------------");
    Serial.println(configJson);
    Serial.println("---------------------------------------------");
    #endif

    bool status = writeFile("/config.json", configJson.c_str());

    #ifdef CONFIGURATION_FILES_DEBUG_MODE
    if(status) {
      Serial.println("[CONFIGURATION_FILES] /config.json update success");
    } else {
      Serial.println("[CONFIGURATION_FILES] /config.json update failed");
    }
    #endif
}

#endif /* _CONFIGURATION_FILES_H_ */