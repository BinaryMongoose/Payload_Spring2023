#include <Adafruit_GPS.h>
#include <Adafruit_PMTK.h>
#include <NMEA_data.h>

#include <SD.h>
#include <SPI.h>

File test;

void setup() {
  Serial.begin(9600);

  Serial.println("Setting up SD...");
  if(!SD.begin(4)) {
    Serial.println("SD Setup failed.");
  }

  test = SD.open("tests.txt", FILE_WRITE);

  test.print("Hello World!");

  test.close();
}

void loop() {
  
}
