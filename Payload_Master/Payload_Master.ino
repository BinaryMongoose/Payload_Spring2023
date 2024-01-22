#include "Payload_master.h"



/*
Here we are choosing what we want to measure. 
We can measure:
  time_since_start,bme_temp,bme_alt,uv,ism_temp,acc_x,acc_y,acc_z,gps_time,latitude,longitude,gps_alt,speed,sats

To measure one of these put it into the array measuring. Make sure to put it in the order you
want to log the data. 
The name MUST BE THE SAME CASE! If it says ism_temp, it must be typed ism_temp. 
*/
String measuring[14] = {"time_since_start","latitude", "longitude", "bme_temp", "bme_alt", "bme_pressure", "bme_humidity", "sats"
                         };

uint32_t log_timer = millis();

void setup() {
  Serial.begin(1115200);
  delay(5000);
  Serial.println("===== Welcome to the Real Deal ===== ");
  Serial.println();

  setup_sd(measuring);

  setup_sensors();

  // setup_gps();

  delay(10000);  // Give the board some time to rest. 
  Serial.println("\n*** Let's Go! ***\n");
}

void loop() {
  unsigned long currentMillis = millis();

  if (millis() - log_timer > 5000) {
    log_timer = millis(); // reset the timer

    log_file(measuring, currentMillis); 
    
  }
}
