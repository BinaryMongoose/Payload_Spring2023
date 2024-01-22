#include <SD.h>

#ifndef Log
#define Log

#include <Arduino.h>

void setup_sd(String measuring[]);
void log_file(String measuring[], unsigned long currentMillis);

#endif
