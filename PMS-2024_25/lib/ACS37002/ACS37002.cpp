#include "ACS37002.h"
#include "SerialConfig.h"

ACS37002::ACS37002(uint8_t voutPin, uint8_t vrefPin, float voltage, float sensitivity)
{
    _voutPin = voutPin;
    _vrefPin = vrefPin;
    _voltage = voltage;
    _sensitivity = sensitivity;
}

void ACS37002::begin()
{
    MySerial.println(_voutPin);
    pinMode(_voutPin, INPUT);

    MySerial.println(_vrefPin);
    pinMode(_vrefPin, INPUT);
}

float ACS37002::readCurrent()
{
    int voutRaw = analogRead(_voutPin);
    int vrefRaw = analogRead(_vrefPin);

    if(voutRaw == -1)
    {
        MySerial.println("vout input pin read invalid");
        return NAN;
    }
    else if(vrefRaw == -1)
    {
        MySerial.println("vref input pin read invalid");
        return NAN;
    }

    float vout = (voutRaw / 4095.0) * _voltage;
    float vref = (vrefRaw / 4095.0) * _voltage;

    float current = (vout - vref) / _sensitivity;

    return current;
    
}

void ACS37002::printPins(void)
{
    MySerial.print("V out pin: ");
    MySerial.println(_voutPin);
    MySerial.print("V ref pin: ");
    MySerial.println(_vrefPin);
}

void ACS37002::printVoltage(void)
{
    int voutRaw = analogRead(_voutPin);
    int vrefRaw = analogRead(_vrefPin);

    float vout = (voutRaw / 4095.0) * _voltage;
    float vref = (vrefRaw / 4095.0) * _voltage;

    MySerial.print("V out: ");
    MySerial.println(vout);
    MySerial.print("V ref: ");
    MySerial.println(vref);
}