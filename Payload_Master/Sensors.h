#include "Adafruit_BME680.h"
#include <Adafruit_SHT4x.h>
#include <Adafruit_LTR390.h>
#include <Adafruit_ISM330DHCX.h>

#include <Adafruit_GPS.h>


#ifndef Sensors
#define Sensors

#include <Arduino.h>

void setup_sensors();
void setup_gps();
void halt(const __FlashStringHelper *error);

float sensor_readings(unsigned long time_since_start, String info);
float get_time(Adafruit_GPS GPS); 
#endif