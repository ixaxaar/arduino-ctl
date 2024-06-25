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

/**
 * @brief I2S control module for Arduino-CTL
 *
 * This class implements the ModuleInterface for I2S communication.
 */
class I2SCtl : public ModuleInterface
{
public:
    /**
     * @brief Constructor for I2SCtl
     *
     * Initializes the I2S control module with default configuration.
     */
    I2SCtl();

    /**
     * @brief Initialize the I2S module
     *
     * @param params A vector of parameter name-value pairs for initialization
     *               Expected parameters:
     *               - "i2sPort": The I2S port number to use
     */
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief De-initialize the I2S module
     *
     * This method is called when the module is no longer needed.
     * It uninstalls the I2S driver.
     */
    void deinit() override;

    /**
     * @brief Execute a command on the I2S module
     *
     * @param command The command to execute ("readData" or "writeData")
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
    i2s_port_t _i2sPort;       ///< The I2S port being used
    i2s_config_t _i2sConfig;   ///< The I2S configuration
    i2s_pin_config_t _i2sPins; ///< The I2S pin configuration

    /**
     * @brief Read data from the I2S interface
     *
     * @param numBytes The number of bytes to read
     * @return A vector of uint8_t containing the read data
     */
    std::vector<uint8_t> readData(size_t numBytes);

    /**
     * @brief Write data to the I2S interface
     *
     * @param data A vector of uint8_t containing the data to write
     */
    void writeData(const std::vector<uint8_t> &data);

    /**
     * @brief Set the I2S configuration
     *
     * @param config The I2S configuration to set
     */
    void setConfig(const i2s_config_t &config);

    /**
     * @brief Set the I2S pin configuration
     *
     * @param pins The I2S pin configuration to set
     */
    void setPins(const i2s_pin_config_t &pins);
};

#endif // I2S_H
