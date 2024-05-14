
#ifndef I2C_H
#define I2C_H

#include <Arduino.h>
#include <Wire.h>
#include <vector>
#include <string>
#include "module.h"

class I2CCtl : public ModuleInterface
{
public:
    I2CCtl();
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;
    void deinit() override;
    std::pair<std::string, void *> execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params) override;
    std::vector<FunctionInfo> getSupportedFunctions() override;

private:
    int _sdaPin;
    int _sclPin;
    uint32_t _frequency;

    void setClock(uint32_t frequency);
    void beginTransmission(uint8_t address);
    uint8_t *endTransmission(bool stopBit);
    uint8_t *requestFrom(uint8_t address, uint8_t quantity, bool stopBit);
    void write(uint8_t data);
    void write(const uint8_t *data, size_t quantity);
    uint8_t *read(size_t quantity);
};

#endif // I2C_H
