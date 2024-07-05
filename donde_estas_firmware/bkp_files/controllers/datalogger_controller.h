#ifndef _DATALOGGER_CONTROLLER_H_
#define _DATALOGGER_CONTROLLER_H_

#define DATALOGGER_CONTROLLER_DEBUG_MODE

//libs
#include "ArduinoJson.h"

//controllers
#include "configuration_files_controller.h"

//services
#include "../services/http_service.h"

#define DATALOGGER_EMPTY            1
#define DATALOGGER_TRANSFER_SUCCESS 2
#define DATALOGGER_TRANSFER_FAILED  3

// checks if the filename if a filename has a numeric name before the .Extension
// example: 1.json returns true, config.json returns false
bool isDataloggerFile(String filename) {
  for(int i = 0 ; i < filename.length() ; i++) {
    char val = filename.charAt(i);
    if(val == '.') {
      break;
    }
    if(!isDigit(val)) {
      return false;
    }
  }
  return true;
}

// empty then no files with [0-9]+.json files are stored in spiffs
// returns true if empty, false if not
bool isDataloggerEmpty(const char * dirname) {
  File root = SPIFFS.open(dirname);
  if(!root){
    #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
    Serial.println("[DATALOGGER_CONTROLLER] Failed to open directory");
    #endif
    return false;
  }

  File file = root.openNextFile();
  while(file){
    String filename = file.name();
    if(isDataloggerFile(filename)) {
      #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
      Serial.println("[DATALOGGER_CONTROLLER] datalogger not empty");
      #endif
      return false;
    }
    file = root.openNextFile();
  }
  #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
  Serial.println("[DATALOGGER_CONTROLLER] datalogger empty");
  #endif
  return true;
}

// returns the filename with the smallest number from the /datalogger
// if searchMax == true it searches the largest one instead
// if any error happens it returns -1
int getDataloggerMaxOrMin(const char * dirname, bool searchMax){
  #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
  Serial.printf("[DATALOGGER_CONTROLLER] dataloger max number filename: %s\n", dirname);
  #endif

  File root = SPIFFS.open(dirname);
  if(!root){
    #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
    Serial.println("[DATALOGGER_CONTROLLER] Failed to open directory");
    #endif
    return -1;
  }
  if(!root.isDirectory()){
    #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
    Serial.println("[DATALOGGER_CONTROLLER] Not a directory");
    #endif
    return -1;
  }
  if(isDataloggerEmpty("/")) {
    return -1;
  }

  int test_number = searchMax? INT_MIN : INT_MAX;
  File file = root.openNextFile();
  while(file){
    String filename = file.name();

    // if its not a datalogger file, don't process
    // the commands below if and go to the next file
    if(!isDataloggerFile(filename)) {
      file = root.openNextFile();
      continue;
    }

    // strtok: split filename in the dot. Only the first 
    // token represent the numeric part of the filename
    // atoi: converts the token str into an integer
    char* str_number = strtok((char*) file.name(), ".");
    int file_number = atoi(str_number);

    if(searchMax) {
      test_number = 
        file_number > test_number ? file_number : test_number;
    } else { // search min
      test_number = 
        file_number < test_number ? file_number : test_number;
    }

    #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
    Serial.println("filename = " + filename + ", number = " + String(file_number));
    #endif

    file = root.openNextFile();
  }

  return test_number;
}

int getDataloggerMax(const char * dirname) {
  #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
  Serial.println("[DATALOGGER_CONTROLLER] search max datalogger");
  #endif

  int max = getDataloggerMaxOrMin(dirname, true);

  #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
  Serial.println("[DATALOGGER_CONTROLLER] max file = " + String(max) + ".json");
  #endif

  return max;
}

int getDataloggerMin(const char * dirname) {
  #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
  Serial.println("[DATALOGGER_CONTROLLER] search min datalogger");
  #endif

  int min = getDataloggerMaxOrMin(dirname, false);

  #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
  Serial.println("[DATALOGGER_CONTROLLER] min file = " + String(min) + ".json");
  #endif
  
  return min;
}

/*
  sends the datalogger data to thingsboard server

  if DATALOGGER_EMPTY enters deep sleep
  if DATALOGGER_TRANSFER_SUCCESS led status transfer success and startDeepSleep();
  if DATALOGGER_TRANSFER_FAILED led status transfer failed
*/
void dataloggerTransfer(bool enableDeepSleep = true) {
  #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
  Serial.println("[DATALOGGER_CONTROLLER] datalogger transfer to thingsboard");
  #endif

  if(isDataloggerEmpty("/")) {
    return;
  }

  int olderFileNumber = getDataloggerMin("/");
  String filename = "/"+String(olderFileNumber)+".json";
  String json = readFile(filename.c_str());
  #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
  Serial.println("[DATALOGGER_CONTROLLER] json data");
  Serial.println(json);
  #endif

  // Init wifi communication
  if(WiFi.status() != WL_CONNECTED) {
    #ifdef TELEMETRY_CONTROLLER_DEBUG_MODE
    Serial.println("[TELEMETRY_CONTROLLER] wifi disconected");
    #endif
    rgbTelemetryTransferFailed();
    return;
  }

  // Thingsboard HTTP transfer
  int httpStatus = telemetryTransfer(json);

  // if transfer failed, 
  if(httpStatus != 200) {
    #ifdef TELEMETRY_CONTROLLER_DEBUG_MODE
    Serial.println("[DATALOGGER_CONTROLLER] http transfer failed, status code: " + String(httpStatus));
    #endif
    rgbTelemetryTransferFailed();
    return;
  }

  #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
  Serial.println("[DATALOGGER_CONTROLLER] transfer success");
  #endif
  deleteFile(filename.c_str());
  rgbTelemetryTransferSuccess();
}

/*
  Receives telemetry in a format of JSON and saves it in the datalogger
  saves only 20 json objects in a single file, in order to prevent the 
  file content to exeed 2048 bytes.

  if the /datalogger is empty, saves the telemetry in a new file
  if its not empty and the older file has space, append telemetry in it
  if its not empty and the older file hasn't space, saves the telemetry in a new file
*/
void dataloggerSaver(String jsonTelemetry) {
  #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
  Serial.println("[DATALOGGER_CONTROLLER] datalogger save");
  #endif
  JsonDocument docDatalogger;
  JsonArray dataloggerJsonArray;
  bool appendInExistingFile = false;
  int filenameCount = 0;
  
  
  // if datalogger not empty
  if(!isDataloggerEmpty("/")) {
    // get datalogger file with biggest number
    filenameCount = getDataloggerMax("/");
    #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
    Serial.print("[DATALOGGER_CONTROLLER] filename with max value = ");
    Serial.println(filenameCount);
    #endif

    // get datalogger file list length
    String filename = "/"+String(filenameCount)+".json";
    String jsonDatalogger = readFile(filename.c_str());
    deserializeJson(docDatalogger, jsonDatalogger);
    dataloggerJsonArray = docDatalogger.as<JsonArray>();
    int dataloggerJsonLength = dataloggerJsonArray.size();
    
    // current file stil has space to save telemetry
    if(dataloggerJsonLength < 20) {
      appendInExistingFile = true;
    } 
    // current file doesn't have enough space, create new one
    else {
      appendInExistingFile = false;
      filenameCount++;
    }

    #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
    Serial.print("[DATALOGGER_CONTROLLER] json array length = ");
    Serial.println(dataloggerJsonLength);
    #endif
  }
  
  String jsonPayload;
  String filename = "/"+String(filenameCount)+".json";

  if(appendInExistingFile) {
    JsonDocument docTelemetry;
    deserializeJson(docTelemetry, jsonTelemetry);
    jsonPayload = "";
    dataloggerJsonArray.add(docTelemetry.as<JsonObject>());
    serializeJson(dataloggerJsonArray, jsonPayload);
    #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
    Serial.println("[DATALOGGER_CONTROLLER] append to existing file = " + filename);
    Serial.println(jsonPayload);
    #endif
  } else {
    jsonPayload = "["+jsonTelemetry+"]";
    #ifdef DATALOGGER_CONTROLLER_DEBUG_MODE
    Serial.println("[DATALOGGER_CONTROLLER] creating new file = " + filename);
    Serial.println(jsonPayload);
    #endif
  }
  
  writeFile(filename.c_str(), jsonPayload.c_str());
}

#endif /*_DATALOGGER_CONTROLLER_H_*/
