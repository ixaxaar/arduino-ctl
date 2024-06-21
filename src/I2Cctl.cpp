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

#include "I2Cctl.h"
#include <sstream>

I2CCtl::I2CCtl() : _sdaPin(SDA), _sclPin(SCL), _frequency(100000) {}

void I2CCtl::init(const std::vector<std::pair<std::string, std::string>> &params)
{
    for (const auto &param : params)
    {
        if (param.first == "sdaPin")
        {
            _sdaPin = std::stoi(param.second);
        }
        else if (param.first == "sclPin")
        {
            _sclPin = std::stoi(param.second);
        }
        else if (param.first == "frequency")
        {
            _frequency = std::stoul(param.second);
        }
    }
    Wire.begin(_sdaPin, _sclPin);
    setClock(_frequency);
}

void I2CCtl::deinit()
{
    Wire.end();
}

std::pair<std::string, void *> I2CCtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "readFromDevice")
    {
        uint8_t address = 0;
        size_t numBytes = 0;
        for (const auto &param : params)
        {
            if (param.first == "address")
            {
                address = std::stoul(param.second);
            }
            else if (param.first == "numBytes")
            {
                numBytes = std::stoul(param.second);
            }
        }
        std::vector<uint8_t> result = readFromDevice(address, numBytes);
        return {"std::vector<uint8_t>", new std::vector<uint8_t>(result)};
    }
    else if (command == "writeToDevice")
    {
        uint8_t address = 0;
        std::vector<uint8_t> data;
        for (const auto &param : params)
        {
            if (param.first == "address")
            {
                address = std::stoul(param.second);
            }
            else if (param.first == "data")
            {
                std::istringstream iss(param.second);
                std::string token;
                while (std::getline(iss, token, ','))
                {
                    data.push_back(std::stoul(token));
                }
            }
        }
        writeToDevice(address, data);
        return {"", nullptr};
    }
    else if (command == "setClock")
    {
        uint32_t frequency = 100000;
        for (const auto &param : params)
        {
            if (param.first == "frequency")
            {
                frequency = std::stoul(param.second);
                break;
            }
        }
        setClock(frequency);
        return {"", nullptr};
    }
    return {"", nullptr};
}

std::vector<FunctionInfo> I2CCtl::getSupportedFunctions()
{
    return {
        {"readFromDevice", {{"address", "uint8_t"}, {"numBytes", "size_t"}}},
        {"writeToDevice", {{"address", "uint8_t"}, {"data", "std::vector<uint8_t>"}}},
        {"setClock", {{"frequency", "uint32_t"}}}};
}

std::vector<uint8_t> I2CCtl::readFromDevice(uint8_t address, size_t numBytes)
{
    Wire.beginTransmission(address);
    Wire.requestFrom(address, numBytes);

    std::vector<uint8_t> data;
    while (Wire.available())
    {
        data.push_back(Wire.read());
    }

    Wire.endTransmission();
    return data;
}

void I2CCtl::writeToDevice(uint8_t address, const std::vector<uint8_t> &data)
{
    Wire.beginTransmission(address);
    for (uint8_t byte : data)
    {
        Wire.write(byte);
    }
    Wire.endTransmission();
}

void I2CCtl::setClock(uint32_t frequency)
{
    _frequency = frequency;
    Wire.setClock(_frequency);
}
