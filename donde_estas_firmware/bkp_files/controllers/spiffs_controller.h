#ifndef _SPIFFS_CONTROLLER_H_
#define _SPIFFS_CONTROLLER_H_

// enable the macro bellow to show logs in console
// #define SPIFFS_CONTROLLER_DEBUG_MODE

#include "FS.h"
#include "SPIFFS.h"
#include "ctype.h"
#include "leds_controller.h"

void initSpiffs() {
  if(!SPIFFS.begin(true)){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] Flash memory mount failed");
    #endif
    rgbStatusHardwareError();
    return;
  }
}

float getUsedSpace() {
  unsigned long total = SPIFFS.totalBytes();
  unsigned long used = SPIFFS.usedBytes();
  float usedSpace = ((float) used/ (float) total)*100;
  
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.printf("[SPIFFS_CONTROLLER] Total space: %lu bytes\n", total);
  Serial.printf("[SPIFFS_CONTROLLER] Used space: %lu bytes\n", used);
  Serial.printf("[SPIFFS_CONTROLLER] Percentage Used space: %.2f\n", usedSpace);
  #endif
  
  return usedSpace;
}

void listDir(const char * dirname, uint8_t levels){
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.printf("[SPIFFS_CONTROLLER] Listing directory: %s\n", dirname);
  #endif

  File root = SPIFFS.open(dirname);
  if(!root){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("Failed to open directory");
    #endif
    return;
  }
  if(!root.isDirectory()){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("Not a directory");
    #endif
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
          listDir(file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

bool isFolderEmpty(const char * dirname) {
  File root = SPIFFS.open(dirname);
  if(!root){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] Failed to open directory");
    #endif
    return false;
  }
  if(!root.isDirectory()){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] Not a directory");
    #endif
    return false;
  }

  File file = root.openNextFile();
  if(file){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] folder not empty");
    #endif
    return false;
  }
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.println("[SPIFFS_CONTROLLER] folder empty");
  #endif
  return true;
}

bool createDir(const char * path){
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.printf("[SPIFFS_CONTROLLER] Creating Dir: %s\n", path);
  #endif
  
  if(!SPIFFS.mkdir(path)){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] mkdir failed");
    #endif
    
    return false;
  }
  
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.println("[SPIFFS_CONTROLLER] Dir created");
  #endif
  
  return true;
}

bool deleteDir(const char * path){
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.printf("[SPIFFS_CONTROLLER] Removing Dir: %s\n", path);
  #endif
  
  if(!SPIFFS.rmdir(path)){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] rmdir failed");
    #endif

    return false;
  }
    
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.println("[SPIFFS_CONTROLLER] Dir removed");
  #endif

  return true;
}

String readFile(const char * path){
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.printf("[SPIFFS_CONTROLLER] Reading file: %s\n", path);
  #endif

  File file = SPIFFS.open(path);
  if(!file){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] Failed to open file for reading");
    #endif
    return "";
  }

  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.print("[SPIFFS_CONTROLLER] Read from file:\n");
  #endif

  String finalString = "";
  while(file.available()){
    finalString += (char)file.read();
  }

  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.println(finalString);
  #endif

  return finalString;
}

bool isFileEmpty(const char * path) {
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.printf("[SPIFFS_CONTROLLER] Empty checking file: %s\n", path);
  #endif

  File file = SPIFFS.open(path);
  if(!file){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] Failed to open file for empty checking");
    #endif
    return false;
  }

  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.print("[SPIFFS_CONTROLLER] Read from file: ");
  #endif

  String finalString = "";
  if(file.available()){
    return false;
  }

  return true;
}

bool writeFile(const char * path, const char * message){
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.printf("[SPIFFS_CONTROLLER] Writing file: %s\n", path);
  #endif

  File file = SPIFFS.open(path, FILE_WRITE);
  if(!file){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] Failed to open file for writing");
    #endif
    
    return false;
  }
  if(!file.print(message)){

    // need this verification, because if the message is empty, 
    // then the function would return that the writing failed
    if(message == "") {
      return true;
    }

    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] Write failed");
    #endif

    return false;
  }
  
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.println("[SPIFFS_CONTROLLER] File written");
  #endif

  return true;
}

bool appendFile(const char * path, const char * message){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.printf("[SPIFFS_CONTROLLER] Appending to file: %s\n", path);
    #endif

    File file = SPIFFS.open(path, FILE_APPEND);
    if(!file){
        #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
        Serial.println("[SPIFFS_CONTROLLER] Failed to open file for appending");
        #endif
      
        return false;
    }
    if(!file.print(message)){
      #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
      Serial.println("[SPIFFS_CONTROLLER] Append failed");
      #endif
    
      return false;
    }
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] Message appended");
    #endif
    
    return true;
}

bool renameFile(const char * path1, const char * path2){
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.printf("[SPIFFS_CONTROLLER] Renaming file %s to %s\n", path1, path2);
  #endif
  
  if (!SPIFFS.rename(path1, path2)) {
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] Rename failed");
    #endif

    return false;
  }
    
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.println("[SPIFFS_CONTROLLER] File renamed");
  #endif
  
  return true;
}

bool moveFile(const char * path1, const char * path2) {
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  // Moving File is the same as Renaming a File
  Serial.println("[SPIFFS_CONTROLLER] Moving File");
  #endif
  return renameFile(path1, path2);
}

bool existsFile(const char * path) {
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.printf("[SPIFFS_CONTROLLER] Checking if %s exists\n", path);
  #endif
  
  if (!SPIFFS.exists(path)) {
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] file do not exist");
    #endif

    return false;
  }
  
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.println("[SPIFFS_CONTROLLER] file do exist");
  #endif
  
  return true;
}

bool deleteFile(const char * path){
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.printf("[SPIFFS_CONTROLLER] Deleting file: %s\n", path);
  #endif
  
  if(!SPIFFS.remove(path)){
    #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
    Serial.println("[SPIFFS_CONTROLLER] Delete failed");
    #endif
    
    return false;
  }
  
  #ifdef SPIFFS_CONTROLLER_DEBUG_MODE
  Serial.println("[SPIFFS_CONTROLLER] File deleted");
  #endif
  
  return true;
}

#endif /*_SPIFFS_CONTROLLER_H_*/
