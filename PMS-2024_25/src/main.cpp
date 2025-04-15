#include <Arduino.h>

#include <SPI.h>


#define DEBUG_PIN PB5
HardwareSerial MySerial(PC4, PC_5);  // TX = PA2, RX = PA3


void setup() {
  Serial.begin(115200);
  delay(500); // Wait for serial connection to be stable
  Serial.println("STM32: Hello from TX!");
}

void loop() {
  Serial.println("Still running...");
  delay(1000);
}


// /Users/emanuelcamarena/Documents/PlatformIO/Projects/PMS-2024_25/src/generic_clock.c