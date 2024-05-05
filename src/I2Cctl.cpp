#include "I2Cctl.h"

I2CCtl::I2CCtl(int sdaPin, int sclPin, uint32_t frequency)
{
    _sdaPin = sdaPin;
    _sclPin = sclPin;
    _frequency = frequency;
}

void I2CCtl::begin()
{
    Wire.begin(_sdaPin, _sclPin, _frequency);
}

void I2CCtl::setClock(uint32_t frequency)
{
    _frequency = frequency;
    Wire.setClock(_frequency);
}

void I2CCtl::beginTransmission(uint8_t address)
{
    Wire.beginTransmission(address);
}

uint8_t I2CCtl::endTransmission(bool stopBit)
{
    return Wire.endTransmission(stopBit);
}

uint8_t I2CCtl::requestFrom(uint8_t address, uint8_t quantity, bool stopBit)
{
    return Wire.requestFrom(address, quantity, stopBit);
}

void I2CCtl::write(uint8_t data)
{
    Wire.write(data);
}

void I2CCtl::write(const uint8_t *data, size_t quantity)
{
    Wire.write(data, quantity);
}

uint8_t I2CCtl::available()
{
    return Wire.available();
}

uint8_t I2CCtl::read()
{
    return Wire.read();
}
