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

    std::vector<uint8_t> readFromDevice(uint8_t address, size_t numBytes);
    void writeToDevice(uint8_t address, const std::vector<uint8_t> &data);
    void setClock(uint32_t frequency);
};

#endif // I2C_H
