#include "Sensors.h"

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

/*** 
 Sensors 
***/
#define SEALEVELPRESSURE_HPA (1013.25)  // We will later reverse engineer altitude from GPS to get the exact sea level pressure.
Adafruit_BME680 bme;
Adafruit_LTR390 ltr;
Adafruit_ISM330DHCX ism;

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

  // LTR-390 Setup
  if (!ltr.begin()) {
    halt(F("Couldn't find LTR-390!"));
  } else {
    Serial.println(" :) LTR-390 Good!");
  }

  ltr.setMode(LTR390_MODE_UVS);
  ltr.setGain(LTR390_GAIN_3);
  ltr.setResolution(LTR390_RESOLUTION_16BIT);
  ltr.setThresholds(100, 1000);
  ltr.configInterrupt(true, LTR390_MODE_UVS);

  // ISM-330 Setup
  if (!ism.begin_I2C()) {
    halt(F("Couldn't find ISM-330!"));
  } else {
    Serial.println(" :) ISM-330 Good!");
  }

  ism.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);

  pinMode(led, OUTPUT);
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

float sensor_readings(unsigned long time_since_start, String info, Adafruit_GPS GPS, bool logging_gps){
  sensors_event_t accel, gyro, temp;
  ism.getEvent(&accel, &gyro, &temp);
  
  if(info == "time_since_start"){
    return time_since_start * 1000;
  } else if (info == "bme_temp") {
    return bme.temperature;
  } else if (info == "bme_alt") {
    return bme.readAltitude(SEALEVELPRESSURE_HPA);
  } else if (info == "uv") {
    return ltr.readUVS();
  } else if (info == "ism_temp") {
    return temp.temperature;
  } else if (info == "acc_x") {
    return accel.acceleration.x;
  } else if (info == "acc_y") {
    return accel.acceleration.y;
  } else if (info == "acc_z") {
    return accel.acceleration.x;
  } else if (info == "gps_time" && logging_gps) {
    return get_time(GPS); 
  } else if (info == "latitude" && logging_gps) {
    return GPS.latitudeDegrees; 
  } else if (info == "longitude" && logging_gps) {
    return GPS.longitudeDegrees; 
  } else if (info == "gps_alt" && logging_gps) {
    return GPS.altitude; 
  } else if (info == "speed" && logging_gps) {
    return GPS.speed * 1.151; 
  } else if (info == "sats" && logging_gps) {
    return GPS.satellites; 
  } else {
    return 0; 
  }
}

float get_time(Adafruit_GPS GPS){
  float hour = GPS.hour + diff;

  if (hour < 0) {
    hour = 24 + hour;
  }
  if (hour > 23) {
    hour = 24 - hour;
  }

  // Handle when hour are past 12 by subtracting 12 hour (1200 value).
  if (hour > 12) {
    hour -= 12;
  }
  // Handle hour 0 (midnight) being shown as 12.
  else if (hour == 0) {
    hour += 12;
  }

  hour = hour*100 + GPS.minute;
  hour = hour*100 + GPS.seconds;

  float time = hour;

  return time;
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