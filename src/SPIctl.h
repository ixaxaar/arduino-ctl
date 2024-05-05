#ifndef SPI_H
#define SPI_H

#include <Arduino.h>
#include <SPI.h>

class SPICtl
{
public:
    SPICtl(int8_t sckPin = SCK, int8_t misoPin = MISO, int8_t mosiPin = MOSI, int8_t ssPin = SS);
    void begin();
    void end();
    void beginTransaction(SPISettings settings);
    void endTransaction();
    uint8_t transfer(uint8_t data);
    void transfer(void *buf, size_t count);
    void setDataMode(uint8_t mode);
    void setBitOrder(uint8_t order);
    void setClockDivider(uint8_t divider);

private:
    int8_t _sckPin;
    int8_t _misoPin;
    int8_t _mosiPin;
    int8_t _ssPin;
};

#endif // SPI_H
