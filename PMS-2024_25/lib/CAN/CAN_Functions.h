




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
  CAN_message_t CAN_inMsg;
 
  private:

  protected:
  STM32_CAN* _can;
  CAN_message_t CAN_outMsg_1;
  CAN_message_t CAN_outMsg_2;
  CAN_message_t CAN_outMsg_3;
  
  uint8_t Counter;
};

#endif