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

/**
 * @brief I2C control module for Arduino-CTL
 *
 * This class implements the ModuleInterface for I2C communication.
 */
class I2CCtl : public ModuleInterface
{
public:
    /**
     * @brief Constructor for I2CCtl
     *
     * Initializes the I2C control module with default values.
     */
    I2CCtl();

    /**
     * @brief Initialize the I2C module
     *
     * @param params A vector of parameter name-value pairs for initialization
     *               Expected parameters:
     *               - "sdaPin": The SDA (data) pin number
     *               - "sclPin": The SCL (clock) pin number
     *               - "frequency": The I2C clock frequency in Hz
     */
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief De-initialize the I2C module
     *
     * This method is called when the module is no longer needed.
     * It releases the I2C bus.
     */
    void deinit() override;

    /**
     * @brief Execute a command on the I2C module
     *
     * @param command The command to execute ("readFromDevice", "writeToDevice", or "setClock")
     * @param params A vector of parameter name-value pairs for the command
     * @return A pair containing the return type as a string and a void pointer to the return value
     */
    std::pair<std::string, void *> execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief Get information about the functions supported by this module
     *
     * @return A vector of FunctionInfo structs describing the supported functions
     */
    std::vector<FunctionInfo> getSupportedFunctions() override;

private:
    int _sdaPin;         ///< The SDA (data) pin number
    int _sclPin;         ///< The SCL (clock) pin number
    uint32_t _frequency; ///< The I2C clock frequency in Hz

    /**
     * @brief Read data from an I2C device
     *
     * @param address The 7-bit I2C address of the device
     * @param numBytes The number of bytes to read
     * @return A vector of uint8_t containing the read data
     */
    std::vector<uint8_t> readFromDevice(uint8_t address, size_t numBytes);

    /**
     * @brief Write data to an I2C device
     *
     * @param address The 7-bit I2C address of the device
     * @param data A vector of uint8_t containing the data to write
     */
    void writeToDevice(uint8_t address, const std::vector<uint8_t> &data);

    /**
     * @brief Set the I2C clock frequency
     *
     * @param frequency The desired clock frequency in Hz
     */
    void setClock(uint32_t frequency);
};

#endif // I2C_H
