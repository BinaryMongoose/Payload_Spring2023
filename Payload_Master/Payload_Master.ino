#include <Adafruit_GPS.h>
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

/*** Misc. ***/
int led = LED_BUILTIN;

void setup() {
  Serial.begin(115200);
  Serial.println(F(" ===== Welcome to GPS Log test. ===== \n"));

  if(!SD.begin(4)) {
    halt(F("Failed to setup SD card!"));
  } else {
    Serial.println("SD good!");
  }

  test = SD.open("TEST.CSV", FILE_WRITE);
  if(!test) {
    halt(F("Failed to open log file!"));
  }

  test.println("latitude,longitude,speed,time");
  test.flush();

  setup_gps();

  pinMode(led, OUTPUT);
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

  if (millis() - timer > 5000) {
    timer = millis(); // reset the timer
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 10); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 10); Serial.println(GPS.lon);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      Serial.print("Antenna status: "); Serial.println((int)GPS.antenna);

      test.print(GPS.latitudeDegrees, 10); test.print(GPS.lat);
      test.print(",");
      test.print(GPS.longitudeDegrees, 10); test.print(GPS.lon);
      test.print(",");
      test.print(GPS.speed * 1.151, 5);
      test.print(",");
      
      if (GPS.hour < 10) { test.print('0'); }
      test.print(GPS.hour, DEC); test.print(':');
      if (GPS.minute < 10) { test.print('0'); }
      test.print(GPS.minute, DEC); test.print(':');
      if (GPS.seconds < 10) { test.print('0'); }
      test.println(GPS.seconds, DEC);
      test.flush();

    } else {
      Serial.println("No fix.");
    }
  }
}

void halt(const __FlashStringHelper *error){
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
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate. DOn't use anything lower.

  delay(1000);  // Wait for everyone to catch up.
  
  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
}
