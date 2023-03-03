#include "Log.h"
#include "Sensors.h"

uint32_t timer = millis();


File currentFile;

void setup_sd(String measuring[]){
  /***
   IMPORTANT! 
   - You MUST set up SD Card FIRST! 
   - It's the only way ANYTHING will work. 
  ***/

  if(!SD.begin(4)) {
    halt(F("Failed to setup SD card!"));
  } else {
    Serial.println(" :) SD Card good!");
  }

 /***
   Creating Initial file. 
   - Named FILE_000 so we can increment the file number.
  ***/
  currentFile = SD.open("FILE_000.CSV", FILE_WRITE);
  if(!currentFile) {
    halt(F("Failed create open log file!"));
  }

  // currentFile.println("time_since_start,bme_temp,bme_alt,uv,ism_temp,acc_x,acc_y,acc_z,gps_time,latitude,longitude,gps_alt,speed,sats");
  for(int i = 0; i < (sizeof(measuring)/sizeof(String)); i++) {
    currentFile.print(String(measuring[i]));
    currentFile.print(",");
  }
  currentFile.println();
  currentFile.flush();
}


void log_file(String measuring[], unsigned long currentMillis, Adafruit_GPS GPS){
  for (int i = 0; i < (sizeof(measuring)/sizeof(String)); i++){
    String m = measuring[i];
    currentFile.print(sensor_readings(currentMillis, m, GPS, false), 8);
    currentFile.print(",");
  }
}