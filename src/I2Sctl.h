
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
    size_t _bufferSize;

    static const i2s_config_t defconf;

    void write(const uint8_t *data, size_t size);
    uint8_t *read(size_t size);
    void setBufferSize(size_t bufferSize);
};

#endif // I2S_H
