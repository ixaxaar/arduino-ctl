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

#ifndef I2C_H
#define I2C_H

#include <Arduino.h>
#include <Wire.h>
#include <vector>
#include <string>
#include "module.h"

/**
 * @class I2CCtl
 * @brief Provides functionality for I2C communication on an Arduino board.
 *
 * The I2CCtl class allows you to initialize and deinitialize I2C communication,
 * set the clock frequency, perform I2C transmissions and receive data.
 */
class I2CCtl : public ModuleInterface
{
public:
    /**
     * @brief Constructor for the I2CCtl class.
     */
    I2CCtl();

    /**
     * @brief Initializes I2C communication with the provided parameters.
     *
     * @param params A vector of pairs containing parameter names and values.
     */
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief Deinitializes I2C communication.
     */
    void deinit() override;

    /**
     * @brief Executes a command with the provided parameters.
     *
     * @param command The command to execute.
     * @param params A vector of pairs containing parameter names and values.
     * @return A pair containing the return type and a pointer to the result.
     */
    std::pair<std::string, void *> execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief Returns a vector of supported functions and their parameter information.
     *
     * @return A vector of FunctionInfo structs.
     */
    std::vector<FunctionInfo> getSupportedFunctions() override;

private:
    int _sdaPin;         ///< The SDA pin number.
    int _sclPin;         ///< The SCL pin number.
    uint32_t _frequency; ///< The I2C clock frequency.

    /**
     * @brief Sets the I2C clock frequency.
     *
     * @param frequency The clock frequency to set.
     */
    void setClock(uint32_t frequency);

    /**
     * @brief Begins an I2C transmission to the specified address.
     *
     * @param address The I2C slave address.
     */
    void beginTransmission(uint8_t address);

    /**
     * @brief Ends the current I2C transmission.
     *
     * @param stopBit Whether to send a stop bit (true) or not (false).
     * @return A pointer to the transmission status.
     */
    uint8_t *endTransmission(bool stopBit);

    /**
     * @brief Requests data from the specified I2C slave address.
     *
     * @param address The I2C slave address.
     * @param quantity The number of bytes to request.
     * @param stopBit Whether to send a stop bit (true) or not (false).
     * @return A pointer to the number of bytes received.
     */
    uint8_t *requestFrom(uint8_t address, uint8_t quantity, bool stopBit);

    /**
     * @brief Writes a byte of data to the I2C bus.
     *
     * @param data The byte to write.
     */
    void write(uint8_t data);

    /**
     * @brief Writes multiple bytes of data to the I2C bus.
     *
     * @param data A pointer to the data to write.
     * @param quantity The number of bytes to write.
     */
    void write(const uint8_t *data, size_t quantity);

    /**
     * @brief Reads multiple bytes of data from the I2C bus.
     *
     * @param quantity The number of bytes to read.
     * @return A pointer to the read data.
     */
    uint8_t *read(size_t quantity);
};

#endif // I2C_H
