#ifndef I2C_H
#define I2C_H

#include <Arduino.h>
#include <Wire.h>

class I2CCtl
{
public:
    I2CCtl(int sdaPin, int sclPin, uint32_t frequency = 100000);
    void begin();
    void setClock(uint32_t frequency);
    void beginTransmission(uint8_t address);
    uint8_t endTransmission(bool stopBit = true);
    uint8_t requestFrom(uint8_t address, uint8_t quantity, bool stopBit = true);
    void write(uint8_t data);
    void write(const uint8_t *data, size_t quantity);
    uint8_t available();
    uint8_t read();

private:
    int _sdaPin;
    int _sclPin;
    uint32_t _frequency;
};

#endif // I2C_H
