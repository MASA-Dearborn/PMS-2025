#include <Arduino.h>

#include <SPI.h>
#include <PinDefines.h>
#include "PeripheralPins.c"
#include <variant_generic.cpp>
#include <MAX31855.h>
#include <SerialConfig.h>
//#include "variant_generic.cpp"

HardwareSerial MySerial(3); 
SPIClass SPI_3(Temp_SPI_MOSI, Temp_SPI_MISO, Temp_SPI_SCLK); 

// MAX31855 example code
int32_t rawData = 0;
MAX31855 myMAX31855(Temp_Chip_Select, &SPI_3);



void setup() {
  MySerial.setTx(DEBUG_MCU_TX_PIN);
  MySerial.begin(115200);

  MySerial.println("Setup has begun");


  SPI_3.begin();  


  /* start MAX31855 */
  myMAX31855.begin();


  while (myMAX31855.getChipID() != MAX31855_ID)
  {

    MySerial.println(F("MAX6675 error")); 
    delay(5000);
  }
  MySerial.println(F("MAX6675 OK"));
  
}

void loop() {
  MySerial.println("Still running...");
  
  while (myMAX31855.detectThermocouple() != MAX31855_THERMOCOUPLE_OK)
  {
    switch (myMAX31855.detectThermocouple())
    {
      case MAX31855_THERMOCOUPLE_SHORT_TO_VCC:
        MySerial.println(F("Thermocouple short to VCC"));
        break;

      case MAX31855_THERMOCOUPLE_SHORT_TO_GND:
        MySerial.println(F("Thermocouple short to GND"));
        break;

      case MAX31855_THERMOCOUPLE_NOT_CONNECTED:
        MySerial.println(F("Thermocouple not connected"));
        break;

      case MAX31855_THERMOCOUPLE_UNKNOWN:
        MySerial.println(F("Thermocouple unknown error"));
        break;

      case MAX31855_THERMOCOUPLE_READ_FAIL:
        MySerial.println(F("Thermocouple read error, check chip & spi cable"));
        break;
    }
    delay(5000);
  }

  rawData = myMAX31855.readRawData();

  MySerial.print(F("Chip ID: "));
  MySerial.println(myMAX31855.getChipID(rawData));

  MySerial.print(F("Cold Junction: "));
  MySerial.println(myMAX31855.getColdJunctionTemperature(rawData));

  MySerial.print(F("Thermocouple: "));
  MySerial.println(myMAX31855.getTemperature(rawData));

  delay(5000);





}


