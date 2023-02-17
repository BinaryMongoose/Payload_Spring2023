/************************************************
 Temperature Logger program.

 Created to learn how to use SD card and Sensor.
 Will also be used to test heater in box.

Elijah Mountz
2/13/2023

TODO: Update tests.txt to tests.csv

*************************************************/
#include <SPI.h>
#include <SD.h>

#include <Adafruit_BME680.h>
#include <bme68x.h>
#include <bme68x_defs.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define HALF_HOUR (18000000)  // Half hour in millieconds
#define HALF_MIN  (30000)

File currentFile; 

Adafruit_BME680 bme;

unsigned long previousMillis;
int interval = 5000;  // 5 seconds.

void setup() {
  Serial.begin(9600);

  Serial.println(F("------ Temperature Logging ------"));
  Serial.println();

  Serial.print("Initializing SD Card...");
  if (!SD.begin(4)) {
    Serial.println("Sd Card setup failed! :(");
    while(1);
  }

  if (!bme.begin()) {
    Serial.println(("Could not find the BME sensor! Check wiring."));
    while(1);
  }
  Serial.println("Setup good!");

  /*******
  SD SETUP
  *******/

  // Open the file.
  currentFile = SD.open("tests.txt", FILE_WRITE);

  // If the file opned okay, write to it.
  if (currentFile) {
    Serial.println("Writing to file...");
    currentFile.println("Temperature Data");

  } else {
    Serial.println("Error opening test.txt");
  }

  /********
  BME SETUP
  ********/

  // Set up oversampling and filter initialization.
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  // Uncomment to setup Pressure and Gas.
  // bme.setPressureOversampling(BME680_OS_4X);
  // bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  // bme.setGasHeater(320, 150);  // 320*C for 150 ms

}

void loop() {
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the last time you wrote the file.
    previousMillis = currentMillis;

    // Write the file.
    currentFile.println(bme.temperature);

    Serial.println(bme.temperature);
    Serial.println(currentMillis);

    // TODO: Logic here isn't working.
    if (currentMillis >= HALF_MIN) {
        currentFile.close();
        Serial.println("File closed.");
    }
  }
}
