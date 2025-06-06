




#ifndef CAN_Functions_h
#define CAN_Functions_h

#if defined(ARDUINO) && ((ARDUINO) >= 100) //arduino core v1.0 or later
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <STM32_CAN.h>





// i called the class CANL4 since im using an L4 stm32 variant
class CANL4
{
  public:
  CANL4(STM32_CAN* can);

  void     begin(void);
  void     sendData();
  void     readCanMessage();
  void     encodeToBuffer(CAN_message_t &msg, uint32_t startBit, uint8_t length, float factor, float offset, float value);

  CAN_message_t CAN_inMsg;

  // in the actual code we can affect these member variables to change
  // what info we send when we run sendData()
  // float payCurrent = 0; // 0.512;
  // float rpiCurrent = 0; // 0.317;
  float tempThermo = 0; // 120;
 
  float cell1Voltage = 0.00; // 2.80
  float cell2Voltage = 0.00; // 2.79;
  float cell3Voltage = 0.00; // 3.78;
  float cell4Voltage = 0.00; //1.90;

  private:

  protected:
  STM32_CAN* _can;
  static CAN_message_t CAN_outMsg_9;
  static CAN_message_t CAN_outMsg_10;
  
  uint8_t Counter;
};

#endif