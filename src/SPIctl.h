

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
    SPISettings _spiSettings;

    std::vector<uint8_t> transfer(const std::vector<uint8_t> &data);
    void beginTransaction();
    void endTransaction();
    void setSettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);
};

#endif // SPI_H
