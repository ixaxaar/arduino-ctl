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

#include "SPIctl.h"

/**
 * @brief Constructor for the SPICtl class.
 *
 * Initializes the SPI pins to default values.
 */
SPICtl::SPICtl() : _sckPin(SCK), _misoPin(MISO), _mosiPin(MOSI), _ssPin(SS) {}

/**
 * @brief Initializes SPI communication with the provided parameters.
 *
 * @param params A vector of pairs containing parameter names and values.
 */
void SPICtl::init(const std::vector<std::pair<std::string, std::string>> &params)
{
    for (const auto &param : params)
    {
        if (param.first == "sckPin")
        {
            _sckPin = std::stoi(param.second);
        }
        else if (param.first == "misoPin")
        {
            _misoPin = std::stoi(param.second);
        }
        else if (param.first == "mosiPin")
        {
            _mosiPin = std::stoi(param.second);
        }
        else if (param.first == "ssPin")
        {
            _ssPin = std::stoi(param.second);
        }
    }

    SPI.begin(_sckPin, _misoPin, _mosiPin, _ssPin);
}

/**
 * @brief Deinitializes SPI communication.
 */
void SPICtl::deinit()
{
    SPI.end();
}

/**
 * @brief Executes a command with the provided parameters.
 *
 * @param command The command to execute.
 * @param params A vector of pairs containing parameter names and values.
 * @return A pair containing the return type and a pointer to the result.
 */
std::pair<std::string, void *> SPICtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "beginTransaction")
    {
        uint32_t clock = SPI_CLOCK_DIV4;
        uint8_t bitOrder = MSBFIRST;
        uint8_t dataMode = SPI_MODE0;
        for (const auto &param : params)
        {
            if (param.first == "clock")
            {
                clock = std::stoul(param.second);
            }
            else if (param.first == "bitOrder")
            {
                bitOrder = std::stoi(param.second);
            }
            else if (param.first == "dataMode")
            {
                dataMode = std::stoi(param.second);
            }
        }
        beginTransaction(clock, bitOrder, dataMode);
        return {"", nullptr};
    }
    else if (command == "endTransaction")
    {
        endTransaction();
        return {"", nullptr};
    }
    else if (command == "transfer")
    {
        uint8_t data = 0;
        for (const auto &param : params)
        {
            if (param.first == "data")
            {
                data = std::stoi(param.second);
                break;
            }
        }
        uint8_t *result = new uint8_t(transfer(data));
        return {"uint8_t", static_cast<void *>(result)};
    }
    else if (command == "transferBytes")
    {
        size_t size = 0;
        uint8_t *data = nullptr;
        for (const auto &param : params)
        {
            if (param.first == "data")
            {
                data = reinterpret_cast<uint8_t *>(std::stoul(param.second, nullptr, 16));
            }
            else if (param.first == "size")
            {
                size = std::stoul(param.second);
            }
        }
        if (data != nullptr && size > 0)
        {
            transferBytes(data, size);
            delete[] data;
        }
        return {"", nullptr};
    }
    else if (command == "setDataMode")
    {
        uint8_t dataMode = SPI_MODE0;
        for (const auto &param : params)
        {
            if (param.first == "dataMode")
            {
                dataMode = std::stoi(param.second);
                break;
            }
        }
        setDataMode(dataMode);
        return {"", nullptr};
    }
    else if (command == "setBitOrder")
    {
        uint8_t bitOrder = MSBFIRST;
        for (const auto &param : params)
        {
            if (param.first == "bitOrder")
            {
                bitOrder = std::stoi(param.second);
                break;
            }
        }
        setBitOrder(bitOrder);
        return {"", nullptr};
    }
    else if (command == "setClockDivider")
    {
        uint8_t clockDiv = SPI_CLOCK_DIV4;
        for (const auto &param : params)
        {
            if (param.first == "clockDiv")
            {
                clockDiv = std::stoi(param.second);
                break;
            }
        }
        setClockDivider(clockDiv);
        return {"", nullptr};
    }
    return {"", nullptr};
}

/**
 * @brief Returns a vector of supported functions and their parameter information.
 *
 * @return A vector of FunctionInfo structs.
 */
std::vector<FunctionInfo> SPICtl::getSupportedFunctions()
{
    return {
        {"beginTransaction", {{"clock", "uint32_t"}, {"bitOrder", "uint8_t"}, {"dataMode", "uint8_t"}}},
        {"endTransaction", {}},
        {"transfer", {{"data", "uint8_t"}}},
        {"transferBytes", {{"data", "uint8_t*"}, {"size", "size_t"}}},
        {"setDataMode", {{"dataMode", "uint8_t"}}},
        {"setBitOrder", {{"bitOrder", "uint8_t"}}},
        {"setClockDivider", {{"clockDiv", "uint8_t"}}}};
}

/**
 * @brief Begins an SPI transaction with the specified settings.
 *
 * @param clock The SPI clock speed.
 * @param bitOrder The bit order (MSBFIRST or LSBFIRST).
 * @param dataMode The SPI data mode (SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3).
 */
void SPICtl::beginTransaction(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
{
    SPI.beginTransaction(SPISettings(clock, bitOrder, dataMode));
}

/**
 * @brief Ends the current SPI transaction.
 */
void SPICtl::endTransaction()
{
    SPI.endTransaction();
}

/**
 * @brief Transfers a byte of data over SPI.
 *
 * @param data The byte to transfer.
 * @return The received byte.
 */
uint8_t SPICtl::transfer(uint8_t data)
{
    return SPI.transfer(data);
}

/**
 * @brief Transfers multiple bytes of data over SPI.
 *
 * @param data A pointer to the data to transfer.
 * @param size The size of the data in bytes.
 */
void SPICtl::transferBytes(uint8_t *data, size_t size)
{
    SPI.transferBytes(data, data, size);
}

/**
 * @brief Sets the SPI data mode.
 *
 * @param dataMode The data mode to set (SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3).
 */
void SPICtl::setDataMode(uint8_t dataMode)
{
    SPI.setDataMode(dataMode);
}

/**
 * @brief Sets the SPI bit order.
 *
 * @param bitOrder The bit order to set (MSBFIRST or LSBFIRST).
 */
void SPICtl::setBitOrder(uint8_t bitOrder)
{
    SPI.setBitOrder(bitOrder);
}

/**
 * @brief Sets the SPI clock divider.
 *
 * @param clockDiv The clock divider to set.
 */
void SPICtl::setClockDivider(uint8_t clockDiv)
{
    SPI.setClockDivider(clockDiv);
}
