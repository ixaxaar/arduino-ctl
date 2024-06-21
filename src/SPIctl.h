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
