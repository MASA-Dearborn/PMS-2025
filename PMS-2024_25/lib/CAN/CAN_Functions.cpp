#include "CAN_Functions.h"
#include <PinDefines.h>
#include <SerialConfig.h>

CANL4::CANL4(STM32_CAN* can)
  : _can(can), Counter(0)
{
}

void CANL4::begin() {
    _can->begin();
    _can->setBaudRate(500000);
    _can->setMBFilterProcessing(MB0, 0x153, 0x1FFFFFFF);
    _can->setMBFilterProcessing(MB1, 0x613, 0x1FFFFFFF);
    _can->setMBFilterProcessing(MB2, 0x615, 0x1FFFFFFF, STD);
    _can->setMBFilterProcessing(MB3, 0x1F0, 0x1FFFFFFF, EXT);

    // We set the data that is static for the three different message structs once here.
    CAN_outMsg_1.id = (0x1A5);
    CAN_outMsg_1.len = 8;
    CAN_outMsg_1.buf[0] =  0x03;
    CAN_outMsg_1.buf[1] =  0x41;
    CAN_outMsg_1.buf[2] =  0x11;
    CAN_outMsg_1.buf[3] =  0x00;
    CAN_outMsg_1.buf[4] =  0x00;
    CAN_outMsg_1.buf[5] =  0x00;
    CAN_outMsg_1.buf[6] =  0x00;
    CAN_outMsg_1.buf[7] =  0x00;
  
    CAN_outMsg_2.id = (0x7E8);
    CAN_outMsg_2.len = 8;
    CAN_outMsg_2.buf[0] =  0x03;
    CAN_outMsg_2.buf[1] =  0x41;
    CAN_outMsg_2.buf[2] =  0x11;
    CAN_outMsg_2.buf[3] =  0x21;
    CAN_outMsg_2.buf[4] =  0x00;
    CAN_outMsg_2.buf[5] =  0x00;
    CAN_outMsg_2.buf[6] =  0x00;
    CAN_outMsg_2.buf[7] =  0xFF;
  
    CAN_outMsg_3.id = (0xA63);
    CAN_outMsg_3.len = 8;
    CAN_outMsg_3.buf[0] =  0x63;
    CAN_outMsg_3.buf[1] =  0x49;
    CAN_outMsg_3.buf[2] =  0x11;
    CAN_outMsg_3.buf[3] =  0x22;
    CAN_outMsg_3.buf[4] =  0x00;
    CAN_outMsg_3.buf[5] =  0x00;
    CAN_outMsg_3.buf[6] =  0x00;
    CAN_outMsg_3.buf[7] =  0x00;
}

void CANL4::sendData() {
    if (Counter >= 255){ Counter = 0;}
  
  // Only the counter value is updated to the 3 messages sent out.
  CAN_outMsg_1.buf[3] =  Counter; 
  _can->write(CAN_outMsg_1);

  CAN_outMsg_2.buf[5] =  Counter;
  _can->write(CAN_outMsg_2);

  CAN_outMsg_3.buf[6] =  Counter;
  _can->write(CAN_outMsg_3);

  MySerial.print("Sent: ");
  MySerial.println(Counter, HEX);
  Counter++;
  delay(100);
}

void CANL4::readCanMessage() {
    
    MySerial.print("Channel:");
    MySerial.print(CAN_inMsg.bus);
    if (CAN_inMsg.flags.extended == false) {
        MySerial.print(" Standard ID:");
      }
      else {
        MySerial.print(" Extended ID:");
      }
      MySerial.print(CAN_inMsg.id, HEX);
    
    
    MySerial.print(" DLC: ");
    MySerial.print(CAN_inMsg.len);
    if (!CAN_inMsg.flags.remote) {
        MySerial.print(" buf: ");
        for (int i = 0; i < CAN_inMsg.len; i++) {
            MySerial.print("0x");
            MySerial.print(CAN_inMsg.buf[i], HEX);
            if (i != (CAN_inMsg.len - 1)) MySerial.print(" ");
        }
        MySerial.println();
    } else {
        MySerial.println(" Data: REMOTE REQUEST FRAME");
    }
    
}
