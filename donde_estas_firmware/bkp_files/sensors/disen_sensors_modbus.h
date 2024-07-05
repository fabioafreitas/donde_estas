#ifndef _DISEN_SENSORS_MODBUS_H_
#define _DISEN_SENSORS_MODBUS_H_

#include <ModbusMaster.h>

#define MODBUS_DIR_PIN  4 // connect DR, RE pin of MAX485 to gpio 4

class ModbusOD{
  private:
    union recieve_data {
        float f;
        uint32_t data;
        struct {
            uint16_t data1;
            uint16_t data2;
        };
    };
    recieve_data dataInput;
    ModbusMaster node;
    static void modbusPreTransmission()
    {
      digitalWrite(MODBUS_DIR_PIN, HIGH);
      delay(500);
    }
    // Pin 4 made low for Modbus receive mode
    static void modbusPostTransmission()
    {
        digitalWrite(MODBUS_DIR_PIN, LOW);
        delay(500);
    }
  public:
    void begin(uint8_t address , Stream &serial)
    {
        pinMode(MODBUS_DIR_PIN, OUTPUT);
        digitalWrite(MODBUS_DIR_PIN, LOW);
        node.begin(address, serial);
        node.preTransmission(modbusPreTransmission);
        node.postTransmission(modbusPostTransmission);
    }
    float getConcentration()
    {
        uint8_t result;
        uint16_t data[2];
        //Registradores do Sensor
        //1792 ou 0x0700 - temp 2 registradores
        //3072 ou 0x0c00 - pH 4 Registradores
        result = node.readHoldingRegisters(0x2000, 6);
        if (result == node.ku8MBSuccess) {
            data[1]=node.getResponseBuffer(0x04);
            data[0]=node.getResponseBuffer(0x05);
            dataInput.data = (uint32_t(data[1]) << 16) | data[0];
            dataInput.data=(dataInput.data>>16) | (dataInput.data<<16);
            dataInput.data1 = (dataInput.data1>>8) | (dataInput.data1<<8);
            dataInput.data2 = (dataInput.data2>>8) | (dataInput.data2<<8);
        } else {
            Serial.print("Failed, Response Code: ");
            Serial.println(result, HEX);
            dataInput.f=NULL;
        }
        return dataInput.f;
    }
    float getTemperature()
    {
        uint8_t result;
        uint16_t data[2];
        result = node.readHoldingRegisters(0x2000, 6);
        if (result == node.ku8MBSuccess) {
            data[1]=node.getResponseBuffer(0x00);
            data[0]=node.getResponseBuffer(0x01);
            dataInput.data = (uint32_t(data[1]) << 16) | data[0];
            dataInput.data=(dataInput.data>>16) | (dataInput.data<<16);
            dataInput.data1 = (dataInput.data1>>8) | (dataInput.data1<<8);
            dataInput.data2 = (dataInput.data2>>8) | (dataInput.data2<<8);
        } else {
            Serial.print("Failed, Response Code: ");
            Serial.println(result, HEX);
            dataInput.f=NULL;
        }
        return dataInput.f;
    }
    float getSaturation()
    {
        uint8_t result;
        uint16_t data[2];
        result = node.readHoldingRegisters(0x2000, 6);
        if (result == node.ku8MBSuccess) {
            data[1]=node.getResponseBuffer(0x02);
            data[0]=node.getResponseBuffer(0x03);
            dataInput.data = (uint32_t(data[1]) << 16) | data[0];
            dataInput.data=(dataInput.data>>16) | (dataInput.data<<16);
            dataInput.data1 = (dataInput.data1>>8) | (dataInput.data1<<8);
            dataInput.data2 = (dataInput.data2>>8) | (dataInput.data2<<8);
        } else {
            Serial.print("Failed, Response Code: ");
            Serial.println(result, HEX);
            dataInput.f=NULL;
        }
        return dataInput.f;
    }
};

class ModbusPH{
private:
    union recieve_data {
        float f;
        uint32_t data;
        struct {
            uint16_t data1;
            uint16_t data2;
        };
    };
    recieve_data dataInput;
    ModbusMaster node;
    // Pin 4 made high for Modbus transmision mode
    static void modbusPreTransmission()
    {
      digitalWrite(MODBUS_DIR_PIN, HIGH);
      delay(500);
    }
    // Pin 4 made low for Modbus receive mode
    static void modbusPostTransmission()
    {
        digitalWrite(MODBUS_DIR_PIN, LOW);
        delay(500);
    }
public:
    void begin(uint8_t address , Stream &serial)
    {
        pinMode(MODBUS_DIR_PIN, OUTPUT);
        digitalWrite(MODBUS_DIR_PIN, LOW);
        node.begin(address, serial);
        node.preTransmission(modbusPreTransmission);
        node.postTransmission(modbusPostTransmission);
    }
    float getTemperature()
    {
        uint8_t result;
        uint16_t data[2];
        //Registradores do Sensor
        //1792 ou 0x0700 - temp 2 registradores
        //3072 ou 0x0c00 - pH 4 Registradores
        result = node.readHoldingRegisters(0x0700, 2);
        if (result == node.ku8MBSuccess) {
            data[1]=node.getResponseBuffer(0x00);
            data[0]=node.getResponseBuffer(0x01);
            dataInput.data = (uint32_t(data[1]) << 16) | data[0];
            dataInput.data=(dataInput.data>>16) | (dataInput.data<<16);
            dataInput.data1 = (dataInput.data1>>8) | (dataInput.data1<<8);
            dataInput.data2 = (dataInput.data2>>8) | (dataInput.data2<<8);
        } else {
            Serial.print("Failed, Response Code: ");
            Serial.println(result, HEX);
            dataInput.f=NULL;
        }
        return dataInput.f;
    }

    float getPh()
    {
        uint8_t result;
        uint16_t data[2];
        //Registradores do Sensor
        //1792 ou 0x0700 - temp 2 registradores
        //3072 ou 0x0c00 - pH 4 Registradores
        result = node.readHoldingRegisters(0x0600, 2);
        if (result == node.ku8MBSuccess) {
            data[1]=node.getResponseBuffer(0x00);
            data[0]=node.getResponseBuffer(0x01);
            dataInput.data = (uint32_t(data[1]) << 16) | data[0];
            dataInput.data=(dataInput.data>>16) | (dataInput.data<<16);
            dataInput.data1 = (dataInput.data1>>8) | (dataInput.data1<<8);
            dataInput.data2 = (dataInput.data2>>8) | (dataInput.data2<<8);
        } else {
            Serial.print("Failed, Response Code: ");
            Serial.println(result, HEX);
            dataInput.f=NULL;
        }
        return dataInput.f;
    }

    float getEval()
    {
        uint8_t result;
        uint16_t data[2];
        //Registradores do Sensor
        //1792 ou 0x0700 - temp 2 registradores
        //3072 ou 0x0c00 - pH 4 Registradores
        result = node.readHoldingRegisters(0x0c00, 4);
        if (result == node.ku8MBSuccess) {
            data[1]=node.getResponseBuffer(0x00);
            data[0]=node.getResponseBuffer(0x01);
            dataInput.data = (uint32_t(data[1]) << 16) | data[0];
            dataInput.data=(dataInput.data>>16) | (dataInput.data<<16);
            dataInput.data1 = (dataInput.data1>>8) | (dataInput.data1<<8);
            dataInput.data2 = (dataInput.data2>>8) | (dataInput.data2<<8);
        } else {
            Serial.print("Failed, Response Code: ");
            Serial.println(result, HEX);
            dataInput.f=NULL;
        }
        return dataInput.f;
    }
    int getID()
    {
        uint8_t result;
        uint16_t data;
        //Registradores do Sensor
        //1792 ou 0x0700 - temp 2 registradores
        //3072 ou 0x0c00 - pH 4 Registradores
        result = node.readHoldingRegisters(0x0800, 1);
        if (result == node.ku8MBSuccess) {
            data=node.getResponseBuffer(0x00);
            data=data>>8|data<<8;
            //data[1]=node.getResponseBuffer(0x01);
        } else {
            Serial.print("Failed, Response Code: ");
            Serial.println(result, HEX);
            dataInput.f=NULL;
        }
        return data;
    }
    int getIDSeeker(uint8_t adress)
    {
        uint8_t result;
        uint16_t data;
        //Registradores do Sensor
        //1792 ou 0x0700 - temp 2 registradores
        //3072 ou 0x0c00 - pH 4 Registradores
        result = node.readHoldingRegisters(adress, 1);
        if (result == node.ku8MBSuccess) {
            data=node.getResponseBuffer(0x00);
            data=data>>8|data<<8;
            //data[1]=node.getResponseBuffer(0x01);
        } else {
            Serial.print("Failed, Response Code: ");
            Serial.println(result, HEX);
            dataInput.f=NULL;
            return -1;
        }
        return data;
    }
    float getCustom(uint16_t address, int count)
    {
        uint8_t result;
        uint16_t data[2];
        //Registradores do Sensor
        //1792 ou 0x0700 - temp 2 registradores
        //3072 ou 0x0c00 - pH 4 Registradores
        result = node.readHoldingRegisters(address, count);
        if (result == node.ku8MBSuccess) {
            data[1]=node.getResponseBuffer(0x00);
            data[0]=node.getResponseBuffer(0x01);
            dataInput.data = (uint32_t(data[1]) << 16) | data[0];
            dataInput.data=(dataInput.data>>16) | (dataInput.data<<16);
            dataInput.data1 = (dataInput.data1>>8) | (dataInput.data1<<8);
            dataInput.data2 = (dataInput.data2>>8) | (dataInput.data2<<8);
        } else {
            Serial.print("Failed, Response Code: ");
            Serial.println(result, HEX);
            dataInput.f=NULL;
        }
        return dataInput.f;
    }
    
};
#endif //_DISEN_SENSORS_MODBUS_H_
