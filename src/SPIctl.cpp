#include "SPIctl.h"

SPICtl::SPICtl(int8_t sckPin, int8_t misoPin, int8_t mosiPin, int8_t ssPin)
{
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
    _ssPin = ssPin;
}

void SPICtl::begin()
{
    SPI.begin(_sckPin, _misoPin, _mosiPin, _ssPin);
}

void SPICtl::end()
{
    SPI.end();
}

void SPICtl::beginTransaction(SPISettings settings)
{
    SPI.beginTransaction(settings);
}

void SPICtl::endTransaction()
{
    SPI.endTransaction();
}

uint8_t SPICtl::transfer(uint8_t data)
{
    return SPI.transfer(data);
}

void SPICtl::transfer(void *buf, size_t count)
{
    SPI.transfer(buf, count);
}

void SPICtl::setDataMode(uint8_t mode)
{
    SPI.setDataMode(mode);
}

void SPICtl::setBitOrder(uint8_t order)
{
    SPI.setBitOrder(order);
}

void SPICtl::setClockDivider(uint8_t divider)
{
    SPI.setClockDivider(divider);
}
