#include "CAN_Functions.h"
#include "stm32l4xx.h"   // or your MCU specific header
#include <PinDefines.h>
#include <SerialConfig.h>

CAN_message_t CANL4::CAN_outMsg_9;
CAN_message_t CANL4::CAN_outMsg_10;

CANL4::CANL4(STM32_CAN* can)
  : _can(can), Counter(0)
{
}

//Raw value to CAN message
void CANL4::encodeToBuffer(CAN_message_t &msg, uint32_t startBit, uint8_t length, float factor, float offset, float value)
{
    int32_t raw = round((value - offset) / factor);
 
    for (uint8_t i = 0; i < length; i++) {
        uint8_t byteIndex = (startBit + i) / 8;
        uint8_t bitIndex = (startBit + i) % 8;
 
        if (raw & (1L << i)) {
            msg.buf[byteIndex] |= (1 << bitIndex);
        } else {
            msg.buf[byteIndex] &= ~(1 << bitIndex);
        }
    }
}


void CANL4::begin() {
    _can->begin();
    _can->setBaudRate(500000);

    // We set the data that is static for the three different message structs once here.
    // Current + Temp (0x109)
    CAN_outMsg_9.id = 0x109;
    CAN_outMsg_9.len = 8;
    memset(CAN_outMsg_9.buf, 0, 8);

    // Voltage + Humidity  (0x10A)
    CAN_outMsg_10.id = 0x10A;
    CAN_outMsg_10.len = 8;
    memset(CAN_outMsg_10.buf, 0, 8);
}

void CANL4::sendData() {
    if (Counter >= 255){ Counter = 0;}

    
  
  // === Current + Temperature (ID 0x301) === //Good
  memset(CAN_outMsg_9.buf, 0, 8);
  // encodeToBuffer(CAN_outMsg_9, 0, 12, 0.001, 0, payCurrent);
  // encodeToBuffer(CAN_outMsg_9, 12, 16, 0.001, 0, rpiCurrent);
  encodeToBuffer(CAN_outMsg_9, 28, 16, 1, -27, tempThermo);
  // CAN_outMsg_9.id = 0x301; wrong IDs most likely
  _can->write(CAN_outMsg_9);

  // === Voltage + Humidity (ID 0x302) ===
  memset(CAN_outMsg_10.buf, 0, 8);
  encodeToBuffer(CAN_outMsg_10, 0, 12, 0.01, 0, cell1Voltage);
  encodeToBuffer(CAN_outMsg_10, 12, 12, 0.01, 0, cell2Voltage);
  encodeToBuffer(CAN_outMsg_10, 24, 12, 0.01, 0, cell3Voltage);
  encodeToBuffer(CAN_outMsg_10, 36, 12, 0.01, 0, cell4Voltage);
  // encodeToBuffer(CAN_outMsg_10, 48, 16, 0.01, 0, Humidity);
  // CAN_outMsg_10.id = 0x302; wrong IDs most likely
  _can->write(CAN_outMsg_10);

  // === COUNTER TEST (optional debug) ===
  // MySerial.print("Sent Frame Set. Counter: ");
  // MySerial.println(Counter, HEX);

  Counter++;
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

