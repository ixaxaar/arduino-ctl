// 🧠 Geniusrise
// Copyright (C) 2023  geniusrise.ai
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

/**
 * @brief Constructor for the I2CCtl class.
 *
 * Initializes the SDA pin, SCL pin, and frequency to default values.
 */
I2CCtl::I2CCtl() : _sdaPin(0), _sclPin(0), _frequency(100000) {}

/**
 * @brief Initializes I2C communication with the provided parameters.
 *
 * @param params A vector of pairs containing parameter names and values.
 */
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

/**
 * @brief Deinitializes I2C communication.
 */
void I2CCtl::deinit()
{
    Wire.end();
}

/**
 * @brief Executes a command with the provided parameters.
 *
 * @param command The command to execute.
 * @param params A vector of pairs containing parameter names and values.
 * @return A pair containing the return type and a pointer to the result.
 */
std::pair<std::string, void *> I2CCtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "setClock")
    {
        uint32_t frequency = 0;
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
    else if (command == "beginTransmission")
    {
        uint8_t address = 0;
        for (const auto &param : params)
        {
            if (param.first == "address")
            {
                address = std::stoul(param.second);
                break;
            }
        }
        beginTransmission(address);
        return {"", nullptr};
    }
    else if (command == "endTransmission")
    {
        bool stopBit = true;
        for (const auto &param : params)
        {
            if (param.first == "stopBit")
            {
                stopBit = (param.second == "true");
                break;
            }
        }
        uint8_t *result = endTransmission(stopBit);
        return {"uint8_t", static_cast<void *>(result)};
    }
    else if (command == "requestFrom")
    {
        uint8_t address = 0;
        uint8_t quantity = 0;
        bool stopBit = true;
        for (const auto &param : params)
        {
            if (param.first == "address")
            {
                address = std::stoul(param.second);
            }
            else if (param.first == "quantity")
            {
                quantity = std::stoul(param.second);
            }
            else if (param.first == "stopBit")
            {
                stopBit = (param.second == "true");
            }
        }
        uint8_t *result = requestFrom(address, quantity, stopBit);
        return {"uint8_t", static_cast<void *>(result)};
    }
    else if (command == "write")
    {
        if (params.size() == 1 && params[0].first == "data")
        {
            uint8_t data = std::stoul(params[0].second);
            write(data);
        }
        else if (params.size() == 2 && params[0].first == "data" && params[1].first == "quantity")
        {
            size_t quantity = std::stoul(params[1].second);
            uint8_t *data = new uint8_t[quantity];
            for (size_t i = 0; i < quantity; ++i)
            {
                data[i] = std::stoul(params[0].second);
            }
            write(data, quantity);
            delete[] data;
        }
        return {"", nullptr};
    }
    else if (command == "read")
    {
        size_t quantity = 0;
        for (const auto &param : params)
        {
            if (param.first == "quantity")
            {
                quantity = std::stoul(param.second);
                break;
            }
        }
        uint8_t *result = read(quantity);
        return {"uint8_t[]", static_cast<void *>(result)};
    }
    return {"", nullptr};
}

/**
 * @brief Returns a vector of supported functions and their parameter information.
 *
 * @return A vector of FunctionInfo structs.
 */
std::vector<FunctionInfo> I2CCtl::getSupportedFunctions()
{
    return {
        {"setClock", {{"frequency", "uint32_t"}}},
        {"beginTransmission", {{"address", "uint8_t"}}},
        {"endTransmission", {{"stopBit", "bool"}}},
        {"requestFrom", {{"address", "uint8_t"}, {"quantity", "uint8_t"}, {"stopBit", "bool"}}},
        {"write", {{"data", "uint8_t"}}},
        {"write", {{"data", "uint8_t"}, {"quantity", "size_t"}}},
        {"read", {{"quantity", "size_t"}}}};
}

/**
 * @brief Sets the I2C clock frequency.
 *
 * @param frequency The clock frequency to set.
 */
void I2CCtl::setClock(uint32_t frequency)
{
    _frequency = frequency;
    Wire.setClock(_frequency);
}

/**
 * @brief Begins an I2C transmission to the specified address.
 *
 * @param address The I2C slave address.
 */
void I2CCtl::beginTransmission(uint8_t address)
{
    Wire.beginTransmission(address);
}

/**
 * @brief Ends the current I2C transmission.
 *
 * @param stopBit Whether to send a stop bit (true) or not (false).
 * @return A pointer to the transmission status.
 */
uint8_t *I2CCtl::endTransmission(bool stopBit)
{
    uint8_t *result = new uint8_t(Wire.endTransmission(stopBit));
    return result;
}

/**
 * @brief Requests data from the specified I2C slave address.
 *
 * @param address The I2C slave address.
 * @param quantity The number of bytes to request.
 * @param stopBit Whether to send a stop bit (true) or not (false).
 * @return A pointer to the number of bytes received.
 */
uint8_t *I2CCtl::requestFrom(uint8_t address, uint8_t quantity, bool stopBit)
{
    uint8_t *result = new uint8_t(Wire.requestFrom(address, quantity, stopBit));
    return result;
}

/**
 * @brief Writes a byte of data to the I2C bus.
 *
 * @param data The byte to write.
 */
void I2CCtl::write(uint8_t data)
{
    Wire.write(data);
}

/**
 * @brief Writes multiple bytes of data to the I2C bus.
 *
 * @param data A pointer to the data to write.
 * @param quantity The number of bytes to write.
 */
void I2CCtl::write(const uint8_t *data, size_t quantity)
{
    Wire.write(data, quantity);
}

/**
 * @brief Reads multiple bytes of data from the I2C bus.
 *
 * @param quantity The number of bytes to read.
 * @return A pointer to the read data.
 */
uint8_t *I2CCtl::read(size_t quantity)
{
    uint8_t *data = new uint8_t[quantity];
    for (size_t i = 0; i < quantity; ++i)
    {
        data[i] = Wire.read();
    }
    return data;
}
