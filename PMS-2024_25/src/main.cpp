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
#include <bq769x0.h>
#include <Wire.h>
#include <ACS37002.h>
//#include "variant_generic.cpp"

HardwareSerial MySerial(3); 
SPIClass SPI_3(Temp_SPI_MOSI, Temp_SPI_MISO, Temp_SPI_SCLK); 


// This will use CAN1 of our MCU PB12/13 pins and set RX-buffer size to 64-messages. TX-buffer size is kept at default 16.
STM32_CAN Can( CAN1, ALT, RX_SIZE_64, TX_SIZE_16 );
CANL4 myCAN(&Can);

// MAX31855 example code
int32_t rawData = 0;
MAX31855 myMAX31855(Temp_Chip_Select, &SPI_3);

bq769x0 myBQ76920(bq76920, 0x08);

ACS37002 myACS37002(Current_Sense_Vout, Current_Sense_Vref, 3.3, 0.040);


// CAN updates 

void updateCANMessages(float temp, float cell1, float cell2, float cell3, float cell4)
{
  // myCAN.payCurrent = current;
  // myCAN.rpiCurrent = rpicurrent;
  myCAN.tempThermo = temp;
  myCAN.cell1Voltage = cell1;
  myCAN.cell2Voltage = cell2;
  myCAN.cell3Voltage = cell3;
  myCAN.cell4Voltage = cell4;
}

void printUpdatedCAN()
{
  // MySerial.println("Payload Current:");
  // MySerial.println(myCAN.payCurrent);
  // MySerial.println("RPI Current:");
  // MySerial.println(myCAN.rpiCurrent);
  MySerial.println("Temp:");
  MySerial.println(myCAN.tempThermo);
  MySerial.println("Cell 1:");
  MySerial.println(myCAN.cell1Voltage);
  MySerial.println("Cell 2:");
  MySerial.println(myCAN.cell2Voltage);
  MySerial.println("Cell 3:");
  MySerial.println(myCAN.cell3Voltage);
  MySerial.println("Cell 4:");
  MySerial.println(myCAN.cell4Voltage);

}




void setup() {
  MySerial.setTx(DEBUG_MCU_TX_PIN);
  MySerial.begin(115200);

  MySerial.println("Setup has begun");

  SPI_3.begin();  

  //setting up SDA and SCL so that data can be sent through I2C
  Wire.setSDA(SDA);
  Wire.setSCL(SCL);
  Wire.begin();

  /* start MAX31855 */
  myMAX31855.begin();
  myCAN.begin();

  /* start ACS37002 */
  myACS37002.begin();

  while (myMAX31855.getChipID() != MAX31855_ID)
  {

    MySerial.println(F("MAX6675 error")); 
    delay(5000);
  }
  MySerial.println(F("MAX6675 OK"));

  

  /* start BQ76920 */
  int result = myBQ76920.begin(-1, Battery_Monitor_Enable);

  if(result == 0)
  {
    MySerial.println("Battery Monitor Initialization Successful");
  }
  else
  {
    MySerial.println("Battery Monitor Initialization Failed");
  }

  /* start CAN */

  myCAN.begin();

  // setup hardware timer to send data in 50Hz pace
  //#if defined(TIM1)
  //  TIM_TypeDef *Instance = TIM1;
  //#else
  //  TIM_TypeDef *Instance = TIM2;
  //#endif
  //  HardwareTimer *SendTimer = new HardwareTimer(Instance);
  //  SendTimer->setOverflow(50, HERTZ_FORMAT); // 50 Hz
  //#if ( STM32_CORE_VERSION_MAJOR < 2 )
  //  SendTimer->attachInterrupt(1, [](){ myCAN.sendData(); });
  //  SendTimer->setMode(1, TIMER_OUTPUT_COMPARE);
  //#else //2.0 forward
  //  SendTimer->attachInterrupt([](){ myCAN.sendData(); }); // using a lambda function
  //#endif
  //SendTimer->resume();
  
}


void loop() {
  MySerial.println("\nPMS Data:");
  

  myMAX31855.getError(); // i made the error checker into a function to clean up the main
  rawData = myMAX31855.readRawData();

  MySerial.print(F("Thermocouple: "));
  MySerial.println(myMAX31855.getTemperature(rawData));

  // MySerial.print(F("Current: "));
  // MySerial.println(myACS37002.readCurrent());


  // enabling battery monitor dont ask
  pinMode(Battery_Monitor_Enable, OUTPUT);
  digitalWrite(Battery_Monitor_Enable, LOW);
  delay(10);   // wait 5 ms for device to receive boot signal (datasheet: max. 2 ms)
  digitalWrite(Battery_Monitor_Enable,HIGH);
  delay(10);   // wait 5 ms for device to receive boot signal (datasheet: max. 2 ms)
  digitalWrite(Battery_Monitor_Enable,LOW);
  delay(10);  // wait for device to boot up completely (datasheet: max. 10 ms)



  //the class has its own array of every individual cells' voltage so this function 
  //updates the voltages for the array and battery pack voltage
  myBQ76920.updateVoltages();


  //just reads the value and prints it
  long batteryVoltage = myBQ76920.getBatteryVoltage();
  MySerial.print("Battery Voltage V: ");
  MySerial.println(myBQ76920.getBatteryVoltage() / 1000.0f);

  // prints cell voltages
  for(int i = 1; i <= 4; i++)
  {
    MySerial.print("Cell #");
    MySerial.print(i);
    MySerial.print(" Voltage mV: ");
    MySerial.println(myBQ76920.getCellVoltage(i) / 1000.0f);
  }

  

  // CAN sending
  updateCANMessages(myMAX31855.getTemperature(rawData),myBQ76920.getCellVoltage(1),myBQ76920.getCellVoltage(2),myBQ76920.getCellVoltage(3),myBQ76920.getCellVoltage(4));
  printUpdatedCAN();
  myCAN.sendData();

  


  delay(1000);

}