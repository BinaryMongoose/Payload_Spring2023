#include "Payload_master.h"

/*** 
 GPS Stuff 
***/
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
File currentFile;


/*** 
 Sensors 
***/
#define SEALEVELPRESSURE_HPA (1013.25)  // We will later reverse engineer altitude from GPS to get the exact sea level pressure.

Adafruit_BME680 bme;
Adafruit_SHT4x sht4;


/*** 
 Misc. 
***/
int led = LED_BUILTIN;
int diff = -7;



void setup() {
  Serial.begin(1115200);
  delay(5000);
  Serial.println("===== Welcome to the Real Deal ===== ");
  Serial.println();

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
   Sensors
  ***/

  Serial.println("\n=== Sensors ===\n");

  if (!sht4.begin()) {
    halt(F("Failed to setup SHT-40!"));
  } else {
    Serial.println(" :) SHT-40 Good!");
  }

  sht4.setPrecision(SHT4X_MED_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);


  if (!bme.begin()) {
    halt(F("Failed to setip BME-680!"));
  } else {
    Serial.println(" :) BME-680 Good!");
  }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);


  /***
   Creating Initial file. 
   - Named FILE_000 so we can increment the file number.
  ***/
  currentFile = SD.open("FILE_000.CSV", FILE_WRITE);
  if(!currentFile) {
    halt(F("Failed create open log file!"));
  }

  currentFile.println("time,sht_temp,bme_temp,bme_alt,latitude,longitude,gps_alt,speed,sats");
  currentFile.flush();


  /***
  GPS Setup
   - ALWAYS DO THIS LAST!
   - Or else everything will break and be bad. 
  ***/
  setup_gps();


  /***
  Setting up onboard LED. 
   - Will change to be a GPS FIX LED,
   - and a RGB LED later. 
  ***/
  pinMode(led, OUTPUT);

  delay(10000);  // Give the board some time to rest. 
  Serial.println("Let's Go!");
}

void loop() {
  unsigned long currentMillis = millis();

  GPS.read();

  if (GPS.newNMEAreceived()) {
    GPS.parse(GPS.lastNMEA());
  }

  if (millis() - timer > 5000) {
    timer = millis(); // reset the timer
    sensors_event_t humidity, temp;
  
    uint32_t timestamp = millis();
    sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
    /*
    if (GPS.hour < 10) { currentFile.print('0'); }
    currentFile.print(GPS.hour, DEC); currentFile.print(':');
    if (GPS.minute < 10) { currentFile.print('0'); }
    currentFile.print(GPS.minute, DEC); currentFile.print(':');
    if (GPS.seconds < 10) { currentFile.print('0'); }
    */


    // currentFile.print(GPS.seconds, DEC);
    currentFile.print(",");
    currentFile.print(temp.temperature);
    currentFile.print(",");
    currentFile.print(bme.temperature);
    currentFile.print(",");
    currentFile.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    currentFile.print(",");
    currentFile.flush();

    int hour = GPS.hour + diff;

    if (hour < 0) {
      hour = 24+hour;
    }
    if (hour > 23) {
      hour = 24-hour;
    }

    // Handle when hour are past 12 by subtracting 12 hour (1200 value).
    if (hour > 12) {
      hour -= 12;
    }
    // Handle hour 0 (midnight) being shown as 12.
    else if (hour == 0) {
      hour += 12;
    }

    Serial.print("Time: ");
    Serial.println(hour);

    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 10); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 10); Serial.println(GPS.lon);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);

      currentFile.print(GPS.latitudeDegrees, 10); currentFile.print(GPS.lat);
      currentFile.print(",");
      currentFile.print(GPS.longitudeDegrees, 10); currentFile.print(GPS.lon);
      currentFile.print(",");
      currentFile.println(GPS.speed * 1.151, 5);
      currentFile.print(",");
      currentFile.println((int)GPS.satellites);
      currentFile.print(",");
      currentFile.println(GPS.altitude);
      currentFile.flush();

    } else {
      Serial.println("No fix.");
      currentFile.println();
    }
  }
}

void halt(const __FlashStringHelper *error){
  Serial.print(" - Error: ");
  Serial.println(error);
  while(1) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
  }
}

void setup_gps(){
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