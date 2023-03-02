#include "Payload_master.h"

/*** 
 GPS Stuff 
***/

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();


/*** 
 File Stuff 
***/

/*
Here we are choosing what we want to measure. 
We can measure:
  time_since_start,bme_temp,bme_alt,uv,ism_temp,acc_x,acc_y,acc_z,gps_time,latitude,longitude,gps_alt,speed,sats

To measure one of these put it into the array measuring. Make sure to put it in the order you
want to log the data. 
The name MUST BE THE SAME CASE! If it says ism_temp, it must be typed ism_temp. 
*/
String measuring[] = {"time_since_start", "bme_temp", "bme_alt", "uv", "ism_temp", "acc_x", "acc_y" , "acc_z" , "gps_time" , \
                      "latitude", "longitude", "gps_alt", "speed", "sats"};
File currentFile;


/*** 
 Misc. 
***/



void setup() {
  Serial.begin(1115200);
  delay(5000);
  Serial.println("===== Welcome to the Real Deal ===== ");
  Serial.println();

  setup_sd();

  setup_sensors();

  setup_gps();

  delay(10000);  // Give the board some time to rest. 
  Serial.println("\n*** Let's Go! ***\n");
}

void loop() {
  unsigned long currentMillis = millis();

  GPS.read();

  if (GPS.newNMEAreceived()) {
    GPS.parse(GPS.lastNMEA());
  }

  if (millis() - timer > 5000) {
    timer = millis(); // reset the timer

    if (GPS.fix) {
      // String time = get_time();
/*
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 10); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 10); Serial.println(GPS.lon);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
*/
      for (int i = 0; i < (sizeof(measuring)/sizeof(String)); i++){
        String m = measuring[i];
        currentFile.print(sensor_readings(m, GPS, true));
        currentFile.print(",");
        Serial.println("- Wrote " + m);
      }
    } else {
      for (int i = 0; i < (sizeof(measuring)/sizeof(String)); i++){
        String m = measuring[i];
        currentFile.print(sensor_readings(m, GPS, false));
        currentFile.print(",");
        Serial.println("- Wrote " + m);
      }
    }
    currentFile.println();
    Serial.println("File saved."); 
    currentFile.flush();
  }
}

void setup_sd(){
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

void setup_gps(){
  /***
  GPS Setup
   - ALWAYS DO THIS LAST!
   - Or else everything will break and be bad. 
  ***/

  // 9600 baud is the default rate for the Ultimate GPS
  GPSSerial.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate. Don't use anything lower.

  delay(1000);  // Wait for everyone to catch up.
  
  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
}
