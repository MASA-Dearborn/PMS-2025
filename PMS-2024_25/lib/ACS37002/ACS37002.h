#ifndef ACS37002_H
#define ACS37002_H

#include <Arduino.h>

class ACS37002 
{
    public:
        ACS37002(uint8_t voutPin, uint8_t vrefPin, float voltage, float sensitivity);
        void begin(void);
        float readCurrent(void);
        void printPins(void);
        void printVoltage(void);

    private:
        uint8_t _voutPin;
        uint8_t _vrefPin;
        float _voltage;
        float _sensitivity;


};

#endif