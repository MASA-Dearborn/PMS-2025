/***************************************************************************************************/
/*
   This is an Arduino library for 14-bit MAX31855 K-Thermocouple to Digital Converter
   with 12-bit Cold Junction Compensation conneted to hardware 5Mhz SPI with maximum sampling
   rate ~9..10Hz.

   - MAX31855 maximum power supply voltage is 3.6v
   - Maximum SPI bus speed 5Mhz
   - K-type thermocouples have an absolute accuracy of around ±2°C..±6°C.
   - Measurement tempereture range -200°C..+700°C ±2°C or -270°C..+1372°C ±6°C
     with 0.25°C resolution/increment.
   - Cold junction compensation range -40°C..+125° ±3°C with 0.062°C resolution/increment.
     Optimal performance of cold junction compensation happends when the thermocouple cold junction
     & the MAX31855 are at the same temperature. Avoid placing heat-generating devices or components
     near the converter because this may produce an errors.
   - It is strongly recommended to add a 10nF/0.01mF ceramic surface-mount capacitor, placed across
     the T+ and T- pins, to filter noise on the thermocouple lines.
     
   written by : enjoyneering79
   sourse code: https://github.com/enjoyneering/MAX31855

   This sensor uses SPI bus to communicate, specials pins are required to interface
   Board:                                    MOSI        MISO        SCLK         SS, don't use for CS   Level
   Uno, Mini, Pro, ATmega168, ATmega328..... 11          12          13           10                     5v
   Mega, Mega2560, ATmega1280, ATmega2560... 51          50          52           53                     5v
   Due, SAM3X8E............................. ICSP4       ICSP1       ICSP3        x                      3.3v
   Leonardo, ProMicro, ATmega32U4........... 16          14          15           x                      5v
   Blue Pill, STM32F103xxxx boards.......... PA17        PA6         PA5          PA4                    3v
   NodeMCU 1.0, WeMos D1 Mini............... GPIO13/D7   GPIO12/D6   GPIO14/D5    GPIO15/D8*             3v/5v
   ESP32.................................... GPIO23/D23  GPIO19/D19  GPIO18/D18   x                      3v

                                             *most boards has 10-12kOhm pullup-up resistor on GPIO2/D4 & GPIO0/D3
                                              for flash & boot

   Frameworks & Libraries:
   ATtiny  Core          - https://github.com/SpenceKonde/ATTinyCore
   ESP32   Core          - https://github.com/espressif/arduino-esp32
   ESP8266 Core          - https://github.com/esp8266/Arduino
   STM32   Core          - https://github.com/stm32duino/Arduino_Core_STM32
                         - https://github.com/rogerclarkmelbourne/Arduino_STM32

   GNU GPL license, all text above must be included in any redistribution,
   see link for details  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/

#ifndef MAX31855_h
#define MAX31855_h

#if defined(ARDUINO) && ((ARDUINO) >= 100) //arduino core v1.0 or later
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(__AVR__)
#include <avr/pgmspace.h>                                 //for Arduino AVR PROGMEM support
#elif defined(ESP8266)
#include <pgmspace.h>                                     //for Arduino ESP8266 PROGMEM support 
#elif defined(_VARIANT_ARDUINO_STM32_) || defined (STM32)
#include <avr/pgmspace.h>                                 //for Arduino STM32 PROGMEM support
#endif

#ifndef  MAX31855_SOFT_SPI                 //enable upload hw driver spi.h
#include <SPI.h>
#endif


#define MAX31855_CONVERSION_POWER_UP_TIME   200    //in milliseconds
#define MAX31855_CONVERSION_TIME            100    //in milliseconds, 9..10Hz sampling rate 
#define MAX31855_THERMOCOUPLE_RESOLUTION    0.25   //in °C per dac step
#define MAX31855_COLD_JUNCTION_RESOLUTION   0.0625 //in °C per dac step


#define MAX31855_ID                         31855
#define MAX31855_FORCE_READ_DATA            7      //force to read the data, 7 is unique because d2d1d0 can't be all high at the same time
#define MAX31855_ERROR                      2000   //returned value if any error happends

#define MAX31855_THERMOCOUPLE_OK            0
#define MAX31855_THERMOCOUPLE_SHORT_TO_VCC  1
#define MAX31855_THERMOCOUPLE_SHORT_TO_GND  2
#define MAX31855_THERMOCOUPLE_NOT_CONNECTED 3
#define MAX31855_THERMOCOUPLE_UNKNOWN       4
#define MAX31855_THERMOCOUPLE_READ_FAIL     5

class MAX31855
{
  public:
   MAX31855(uint8_t cs, SPIClass* spi = &SPI);

           void     begin(void);
           uint8_t  detectThermocouple(int32_t rawValue = MAX31855_FORCE_READ_DATA);
           uint16_t getChipID(int32_t rawValue = MAX31855_FORCE_READ_DATA);
           float    getTemperature(int32_t rawValue = MAX31855_FORCE_READ_DATA);
           float    getColdJunctionTemperature(int32_t rawValue = MAX31855_FORCE_READ_DATA);
           void     getError(void);

   virtual int32_t  readRawData(void);
 
  private:

  protected:
   uint8_t _cs;
   SPIClass* _spi;
};

#endif