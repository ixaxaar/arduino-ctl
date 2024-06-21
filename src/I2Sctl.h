// ⚡🔌 Arduino-CTL 🔌⚡
//
// Copyright (C) 2024 ixaxaar
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
