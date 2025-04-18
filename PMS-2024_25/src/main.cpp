#include <PinDefines.h>
#include "generic_clock.c"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

//#include "PeripheralPins.c"
//#include <variant_generic.cpp>
//#include "MyPeripheralInit.cpp" 
//#include "stm32l4xx_hal.h"
//#include <MyPeripherals.h>


HardwareSerial MySerial(3); 


// Explicitly use SPI3 with your defined pins
//SPIClass SPI_3(PC12, PC11, PC10); // MOSI, MISO, SCLK

// Forward declare function
uint32_t readMAX31855();


SPIClass SPI_3(Temp_SPI_MOSI, Temp_SPI_MISO, Temp_SPI_SCLK);   // Declared somewhere else
//SPIClass SPI_3(PA7, PA6, PA5);   // Declared somewhere else

// PA5 sck, PA6 MISO, PA7 MOSI


void setup() {
  MySerial.setTx(DEBUG_MCU_TX_PIN);
  MySerial.begin(115200);
  MySerial.println("Program has been set up.");

  pinMode(Temp_Chip_Select, OUTPUT);
  digitalWrite(Temp_Chip_Select, HIGH); // Deselect initially

  PinName sclk = digitalPinToPinName(Temp_SPI_SCLK);
  PinName miso = digitalPinToPinName(Temp_SPI_MISO);
  PinName mosi = digitalPinToPinName(Temp_SPI_MOSI);
  MySerial.printf("Resolved SPI3 pins: SCLK = %d, MISO = %d, MOSI = %d\n", sclk, miso, mosi);


  SPI_3.begin();
  SPI_3.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); // MAX 5MHz, Mode 0
  MySerial.println("SPI_3 Began transaction");

}


void loop() {
 
  uint32_t data = readMAX31855();

  int16_t tempRaw = (data >> 18) & 0x3FFF;
  if (tempRaw & 0x2000) tempRaw |= 0xC000; // Sign extend negative numbers
  float temperature = tempRaw * 0.25;

  MySerial.print("Thermocouple Temperature: ");
  MySerial.print(temperature);
  MySerial.println(" °C");

  delay(1000);
}



uint32_t readMAX31855() {
  digitalWrite(Temp_Chip_Select, LOW);
  delayMicroseconds(1); // small delay before starting

  uint32_t value = 0;
  for (int i = 0; i < 4; i++) {
    value <<= 8;
    MySerial.println("Transfer started");

    value |= SPI_3.transfer(0x00);
    MySerial.println("Transfer finished");

  }

  digitalWrite(Temp_Chip_Select, HIGH);
  return value;
}