#ifndef _WEB_RTC_CONTROLLER_H_
#define _WEB_RTC_CONTROLLER_H_

// esta biblioteca requer conexão com a internet para funcionar corretamente

// #define WEB_RTC_CONTROLLER_DEBUG_MODE

//libs
#include <Arduino.h>
#include <time.h>
#include "ArduinoJson.h"

//controllers
#include "spiffs_controller.h"

//services
#include "../services/http_service.h"

int refEsp32RtcMillis; // ESP32 RTC time when unixtime was fetched (in milliseconds)
unsigned long int lastFetchedUnixtime = -1;

void initWebRtc() {
    lastFetchedUnixtime = getDateTimeFromAPI();
    #ifdef WEB_RTC_CONTROLLER_DEBUG_MODE
    Serial.println("[WEB_RTC] " + String(lastFetchedUnixtime));
    #endif
    refEsp32RtcMillis = millis();
}

// return -1 if an error happened during fetch unixtime
int getUnixTime() {
    int unixtime = lastFetchedUnixtime;
    if (lastFetchedUnixtime != -1) {

        // number of seconds since the unixtime was fetched
        int refVal = (millis() - refEsp32RtcMillis)/1000;
        unixtime += refVal;

        #ifdef WEB_RTC_CONTROLLER_DEBUG_MODE
        time_t unixTimeAux = unixtime;
        struct tm *tmstruct = gmtime(&unixTimeAux);
        Serial.printf("[WEB_RTC] UTC Time: %04d-%02d-%02d %02d:%02d:%02d\n",
                        tmstruct->tm_year + 1900, tmstruct->tm_mon + 1, tmstruct->tm_mday,
                        tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
        #endif
    }
    return unixtime;
}

#endif /*_WEB_RTC_CONTROLLER_H_*/
