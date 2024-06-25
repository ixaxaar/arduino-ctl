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

/**
 * @brief SPI control module for Arduino-CTL
 *
 * This class implements the ModuleInterface for SPI communication.
 */
class SPICtl : public ModuleInterface
{
public:
    /**
     * @brief Constructor for SPICtl
     *
     * Initializes the SPI control module with default settings.
     */
    SPICtl();

    /**
     * @brief Initialize the SPI module
     *
     * @param params A vector of parameter name-value pairs for initialization
     *               Expected parameters:
     *               - "sckPin": The SCK (clock) pin number
     *               - "misoPin": The MISO (Master In Slave Out) pin number
     *               - "mosiPin": The MOSI (Master Out Slave In) pin number
     *               - "ssPin": The SS (Slave Select) pin number
     */
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief De-initialize the SPI module
     *
     * This method is called when the module is no longer needed.
     * It ends the SPI communication.
     */
    void deinit() override;

    /**
     * @brief Execute a command on the SPI module
     *
     * @param command The command to execute ("transfer" or "setSettings")
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
    int8_t _sckPin;           ///< The SCK (clock) pin number
    int8_t _misoPin;          ///< The MISO (Master In Slave Out) pin number
    int8_t _mosiPin;          ///< The MOSI (Master Out Slave In) pin number
    int8_t _ssPin;            ///< The SS (Slave Select) pin number
    SPISettings _spiSettings; ///< The current SPI settings

    /**
     * @brief Transfer data over SPI
     *
     * @param data A vector of uint8_t containing the data to transfer
     * @return A vector of uint8_t containing the received data
     */
    std::vector<uint8_t> transfer(const std::vector<uint8_t> &data);

    /**
     * @brief Begin an SPI transaction
     *
     * This method starts an SPI transaction with the current settings.
     */
    void beginTransaction();

    /**
     * @brief End an SPI transaction
     *
     * This method ends the current SPI transaction.
     */
    void endTransaction();

    /**
     * @brief Set SPI communication settings
     *
     * @param clock The SPI clock speed in Hz
     * @param bitOrder The bit order (MSBFIRST or LSBFIRST)
     * @param dataMode The SPI mode (SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3)
     */
    void setSettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);
};

#endif // SPI_H
