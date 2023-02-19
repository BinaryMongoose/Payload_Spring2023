#include <Adafruit_GPS.h>
#include <Adafruit_PMTK.h>
#include <NMEA_data.h>

#include <SD.h>
#include <SPI.h>

#define GPSSerial Serial1

/*** GPS Stuff ***/
// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();


/*** File Stuff ***/
File test;

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 15000;  // interval at which to save (15 seconds)

void setup() {
  Serial.begin(115200);

  // wait for hardware serial to appear
  while (!Serial) delay(10);

  // 9600 baud is the default rate for the Ultimate GPS
  GPSSerial.begin(9600);

  Serial.println("Setting up SD...");
  if(!SD.begin(4)) {
    Serial.println("SD Setup failed.");
    delay(10);
  }

  test = SD.open("tests.txt", FILE_WRITE);

  test.println("===============");

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
}

void loop() {
  unsigned long currentMillis = millis();

  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      Serial.print("Antenna status: "); Serial.println((int)GPS.antenna);

      test.print("Location: ");
      test.print(GPS.latitude, 4); test.print(GPS.lat);
      test.print(", ");
      test.print(GPS.longitude, 4); test.println(GPS.lon);
      test.print("Speed (knots): "); test.println(GPS.speed * 1.151);
    } else {
      Serial.println("No fix.");
    }
  }
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    test.close();
    test = SD.open("tests.txt", FILE_WRITE);
    Serial.println("Test closed and reopned!");
  }
}

