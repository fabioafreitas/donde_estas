#ifndef _HTTP_SERVICE_H_
#define _HTTP_SERVICE_H_

// #define HTTP_SERVICE_DEBUG_MODE

//libs
#include <WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"

//controllers
#include "controllers/configuration_files_controller.h"

// return unixtime (int) or -1 if an error occurs
int getDateTimeFromAPI() {
  
  #ifdef HTTP_SERVICE_DEBUG_MODE
  Serial.println("[HTTP_SERVICE] get datetime from api");
  #endif
  
  // requisição http get
  HTTPClient http;
  String uri = "https://worldtimeapi.org/api/timezone/America/Sao_Paulo";
  http.begin(uri);
  int httpResponseCode = http.GET();

  #ifdef HTTP_SERVICE_DEBUG_MODE
  Serial.print("[HTTP_SERVICE] Response code: ");
  Serial.println(httpResponseCode);
  #endif

  if(httpResponseCode <= 0 || httpResponseCode != 200){
    #ifdef HTTP_SERVICE_DEBUG_MODE
    Serial.print("[HTTP_SERVICE] Error on sending GET: ");
    Serial.println(httpResponseCode);
    #endif
    return -1;
  }
  
  String json = http.getString();
  #ifdef HTTP_SERVICE_DEBUG_MODE
  Serial.print("[HTTP_SERVICE] JSON: ");
  Serial.println(String(json));
  #endif
  
  JsonDocument doc;
  deserializeJson(doc, json);
  int jsonUnixtime = doc["unixtime"];
  #ifdef HTTP_SERVICE_DEBUG_MODE
  Serial.print("[HTTP_SERVICE] Unixtime: ");
  Serial.println(jsonUnixtime);
  #endif

  http.end();
  
  return jsonUnixtime;
}

// make a http request sending json telemetry to thigsboard
// returns the http status code
int telemetryTransfer(String json) {
  #ifdef HTTP_SERVICE_DEBUG_MODE
  Serial.println("[HTTP_SERVICE] send telemetry to thingsboard:");
  Serial.println("json length = " + String(json.length()));
  Serial.print(json);
  #endif

  HTTPClient http;
  String uri = "https://"+String(THINGSBOARD_URL)+"/api/v1/"+String(THINGSBOARD_HTTP_ACCESS_TOKEN)+"/telemetry";
  http.begin(uri);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(json);

  #ifdef HTTP_SERVICE_DEBUG_MODE
  Serial.print("[HTTP_SERVICE] Response code: ");
  Serial.println(httpResponseCode);
  #endif
  
  http.end();

  #ifdef HTTP_SERVICE_DEBUG_MODE
  if(httpResponseCode <= 0 || httpResponseCode != 200){
    Serial.print("[HTTP_SERVICE] Error on sending POST: ");
  }
  #endif

  return httpResponseCode;
}

#endif /*_HTTP_SERVICE_H_*/
