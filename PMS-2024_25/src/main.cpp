#include <Arduino.h>

#include <SPI.h>
#include <PinDefines.h>
#include "PeripheralPins.c"
#include <variant_generic.cpp>
//#include "variant_generic.cpp"

HardwareSerial MySerial(3); 


void setup() {
  MySerial.setTx(DEBUG_MCU_TX_PIN);
  MySerial.begin(115200);
  
}

void loop() {
  MySerial.println("Still running...");
  


}


