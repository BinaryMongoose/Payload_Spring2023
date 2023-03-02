#include "Adafruit_BME680.h"
#include <Adafruit_SHT4x.h>
#include <Adafruit_LTR390.h>
#include <Adafruit_ISM330DHCX.h>

#include <Adafruit_GPS.h>


#ifndef Sensors
#define Sensors

#include <Arduino.h>

void setup_sensors();
void halt(const __FlashStringHelper *error);

float sensor_readings(String info, Adafruit_GPS GPS, bool logging_gps);
float get_time(Adafruit_GPS GPS); 
#endif