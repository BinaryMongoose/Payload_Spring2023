#include "Sensors.h"

// what's the name of the hardware serial port?
// #define GPSSerial Serial1

// Connect to the GPS on the hardware port
// Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
// #define GPSECHO false

/*** 
 Sensors 
***/
#define SEALEVELPRESSURE_HPA (1013.25)  // We will later reverse engineer altitude from GPS to get the exact sea level pressure.
Adafruit_BME680 bme;
// Adafruit_LTR390 ltr;
// Adafruit_ISM330DHCX ism;

int led = LED_BUILTIN;
int diff = -7;

void setup_sensors(){
  Serial.println("\n=== Sensors ===");

  // BME-680 Setup
  if (!bme.begin()) {
    halt(F("Couldn't find BME-680!"));
  } else {
    Serial.println(" :) BME-680 Good!");
  }
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);

  pinMode(led, OUTPUT);
}


float sensor_readings(unsigned long time_since_start, String info) {

  
  if (info == "bme_temp") {
    return bme.temperature;
  } else if (info == "bme_alt") {
    return bme.readAltitude(SEALEVELPRESSURE_HPA);
  } else if (info == "bme_humidity") {
    return bme.humidity;
  } else if (info == "bme_pressure") {
    return bme.pressure;
  }
  
  
  else
  {
    return 0; 
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
