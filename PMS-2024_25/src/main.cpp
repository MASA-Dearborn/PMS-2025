#include <PinDefines.h>
#include <Arduino.h>
#include <SPI.h>
#include <PinDefines.h>
#include "PeripheralPins.c"
#include <variant_generic.cpp>
#include <MAX31855.h>
#include <SerialConfig.h>
#include <STM32_CAN.h>
#include <CAN_Functions.h>
//#include "variant_generic.cpp"

HardwareSerial MySerial(3); 
SPIClass SPI_3(Temp_SPI_MOSI, Temp_SPI_MISO, Temp_SPI_SCLK); 

// This will use CAN1 of our MCU PB12/13 pins and set RX-buffer size to 64-messages. TX-buffer size is kept at default 16.
STM32_CAN Can( CAN1, ALT, RX_SIZE_64, TX_SIZE_16 );
CANL4 myCAN(&Can);

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
  myCAN.begin();


  while (myMAX31855.getChipID() != MAX31855_ID)
  {

    MySerial.println(F("MAX6675 error")); 
    delay(5000);
  }
  MySerial.println(F("MAX6675 OK"));



  // setup hardware timer to send data in 50Hz pace
  //#if defined(TIM1)
  //  TIM_TypeDef *Instance = TIM1;
  //#else
  //  TIM_TypeDef *Instance = TIM2;
  //#endif
  //  HardwareTimer *SendTimer = new HardwareTimer(Instance);
  //  SendTimer->setOverflow(50, HERTZ_FORMAT); // 50 Hz
  //#if ( STM32_CORE_VERSION_MAJOR < 2 )
  //  SendTimer->attachInterrupt(1, SendData);
  //  SendTimer->setMode(1, TIMER_OUTPUT_COMPARE);
  //#else //2.0 forward
  //  SendTimer->attachInterrupt(SendData);
  //#endif
  //SendTimer->resume();
  
}


void loop() {
  MySerial.println("Still running...");
  

  myMAX31855.getError(); // i made the error checker into a function to clean up the main
  rawData = myMAX31855.readRawData();

  MySerial.print(F("Thermocouple: "));
  MySerial.println(myMAX31855.getTemperature(rawData));

  // this is the stuff i got from chat to read an analog voltage
  int adcVout = analogRead(Current_Sense_Vout);
  int adcVref = analogRead(Current_Sense_Vref);

  float vout = adcVout * (3.3 / 4095.0);
  float vref = adcVref * (3.3 / 4095.0);   

  float current = (vout - vref) / 0.040;

  MySerial.print(F("Current: "));
  MySerial.println(current);


  // can main code has been commented out so i could test current sensor
  // CAN stuff is in the lib folder under CAN with my self created .h and .cpp file

  // CAN Stuff 
  //while (Can.read(myCAN.CAN_inMsg) ) 
  //{
  //  myCAN.readCanMessage();
  //}


  delay(5000);





}


