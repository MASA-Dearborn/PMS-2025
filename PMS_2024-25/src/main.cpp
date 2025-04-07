#include <Arduino.h>

#include <SPI.h>

#define SPI_CS_PIN   PC13  // Chip Select (you defined this)

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial monitor to open

  Serial.println("Starting MISO-only SPI test...");

  pinMode(SPI_CS_PIN, OUTPUT);
  digitalWrite(SPI_CS_PIN, HIGH); // Deselect the slave

  SPI.begin();  // Uses your defined pins (make sure PC10 = SCK, PC11 = MISO)

  // Optional: set up SPI settings for your sensor
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  digitalWrite(SPI_CS_PIN, LOW); // Select the sensor

  // Send a dummy byte (just clocks the bus to read from MISO)
  byte incoming = SPI.transfer(0x00);

  digitalWrite(SPI_CS_PIN, HIGH); // Deselect the sensor

  Serial.print("Received: 0x");
  Serial.println(incoming, HEX);

  delay(1000); // Read every second
}
