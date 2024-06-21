

#ifndef I2S_H
#define I2S_H

#include <Arduino.h>
#include <driver/i2s.h>
#include <vector>
#include <string>
#include "module.h"

class I2SCtl : public ModuleInterface
{
public:
    I2SCtl();
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;
    void deinit() override;
    std::pair<std::string, void *> execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params) override;
    std::vector<FunctionInfo> getSupportedFunctions() override;

private:
    i2s_port_t _i2sPort;
    i2s_config_t _i2sConfig;
    i2s_pin_config_t _i2sPins;

    std::vector<uint8_t> readData(size_t numBytes);
    void writeData(const std::vector<uint8_t> &data);
    void setConfig(const i2s_config_t &config);
    void setPins(const i2s_pin_config_t &pins);
};

#endif // I2S_H
