#ifndef _TELEMETRY_CONTROLLER_H_
#define _TELEMETRY_CONTROLLER_H_

#define TELEMETRY_CONTROLLER_DEBUG_MODE

//libs
#include "ArduinoJson.h"

//controllers
#include "datalogger_controller.h"
#include "configuration_files_controller.h"
#include "web_rtc_controller.h"

//services
#include "../services/http_service.h"

//sensors
#include "../sensors/disen_sensors_modbus.h"

/*
    * initializes the sensors and required modules for them to work
    * collects the sensors readings
    * sends data to thingsboard via HTTP
    * calls datalogger if transfer fails, to save telemetry in SD
    * it already calls the LED RGB signals
    
    Returns false if the telemetry transfer to thingsboard fails
    Returns true if the telemetry transfer to thingsboard succeeds
    
*/
bool telemetryReadAndTransfer() {
    // Serializing JSON
    String json;
    JsonDocument doc;

    // Get current unixtime (UTC+0)
    doc["ts"] = String(getUnixTime())+"000";

    // Sensors reading
    JsonObject values = doc["values"].to<JsonObject>();
    
    // sending firmware and hardware versions as telemetry
    values["firmware_version"] = AQUATIC_HARDWARE_VERSION;
    values["hardware_version"] = AQUATIC_FIRMWARE_VERSION;
    
    Serial2.begin(9600);
    Serial2.setTimeout(1000);

    ModbusPH ph;
    ph.begin(0x01, Serial2);
    values["ph"] = ph.getPh();
    values["temperature"] = ph.getTemperature();

    // ModbusOD od;
    // od.begin(0x02, Serial2);
    // values["temperature"] = od.getTemperature();
    // values["do_saturation"] = od.getSaturation()*100;
    // values["do_concentration"] = od.getConcentration();

    serializeJson(doc, json);

    // Init wifi communication
    if(WiFi.status() != WL_CONNECTED) {
        #ifdef TELEMETRY_CONTROLLER_DEBUG_MODE
        Serial.println("[TELEMETRY_CONTROLLER] wifi disconected");
        #endif
        dataloggerSaver(json);
        rgbTelemetryTransferFailed();
        return false;
    }

    // Thingsboard HTTP transfer
    int httpStatus = telemetryTransfer(json);

    // If server transfer failed, save telemetry in datalogger
    if(httpStatus != 200) {
        #ifdef TELEMETRY_CONTROLLER_DEBUG_MODE
        Serial.println("[TELEMETRY_CONTROLLER] http transfer failed, status code: " + String(httpStatus));
        #endif
        dataloggerSaver(json);
        rgbTelemetryTransferFailed();
        return false;
    }
    
    // transfer success
    #ifdef TELEMETRY_CONTROLLER_DEBUG_MODE
    Serial.println("[TELEMETRY_CONTROLLER] transfer success");
    #endif
    rgbTelemetryTransferSuccess();
    return true;
}


#endif /* _TELEMETRY_CONTROLLER_H_ */