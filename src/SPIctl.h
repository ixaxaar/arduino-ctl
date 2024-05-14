
#ifndef SPI_H
#define SPI_H

#include <Arduino.h>
#include <SPI.h>
#include <vector>
#include <string>
#include "module.h"

class SPICtl : public ModuleInterface
{
public:
    SPICtl();
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;
    void deinit() override;
    std::pair<std::string, void *> execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params) override;
    std::vector<FunctionInfo> getSupportedFunctions() override;

private:
    int8_t _sckPin;
    int8_t _misoPin;
    int8_t _mosiPin;
    int8_t _ssPin;

    void beginTransaction(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);
    void endTransaction();
    uint8_t transfer(uint8_t data);
    void transferBytes(uint8_t *data, size_t size);
    void setDataMode(uint8_t dataMode);
    void setBitOrder(uint8_t bitOrder);
    void setClockDivider(uint8_t clockDiv);
};

#endif // SPI_H
