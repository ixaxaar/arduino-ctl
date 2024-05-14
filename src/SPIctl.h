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
 * @class SPICtl
 * @brief Provides functionality for SPI communication on an Arduino board.
 *
 * The SPICtl class allows you to initialize and deinitialize SPI communication,
 * begin and end transactions, transfer data, and configure SPI settings.
 */
class SPICtl : public ModuleInterface
{
public:
    /**
     * @brief Constructor for the SPICtl class.
     */
    SPICtl();

    /**
     * @brief Initializes SPI communication with the provided parameters.
     *
     * @param params A vector of pairs containing parameter names and values.
     */
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief Deinitializes SPI communication.
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
    int8_t _sckPin;  ///< The SCK pin number.
    int8_t _misoPin; ///< The MISO pin number.
    int8_t _mosiPin; ///< The MOSI pin number.
    int8_t _ssPin;   ///< The SS pin number.

    /**
     * @brief Begins an SPI transaction with the specified settings.
     *
     * @param clock The SPI clock speed.
     * @param bitOrder The bit order (MSBFIRST or LSBFIRST).
     * @param dataMode The SPI data mode (SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3).
     */
    void beginTransaction(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);

    /**
     * @brief Ends the current SPI transaction.
     */
    void endTransaction();

    /**
     * @brief Transfers a byte of data over SPI.
     *
     * @param data The byte to transfer.
     * @return The received byte.
     */
    uint8_t transfer(uint8_t data);

    /**
     * @brief Transfers multiple bytes of data over SPI.
     *
     * @param data A pointer to the data to transfer.
     * @param size The size of the data in bytes.
     */
    void transferBytes(uint8_t *data, size_t size);

    /**
     * @brief Sets the SPI data mode.
     *
     * @param dataMode The data mode to set (SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3).
     */
    void setDataMode(uint8_t dataMode);

    /**
     * @brief Sets the SPI bit order.
     *
     * @param bitOrder The bit order to set (MSBFIRST or LSBFIRST).
     */
    void setBitOrder(uint8_t bitOrder);

    /**
     * @brief Sets the SPI clock divider.
     *
     * @param clockDiv The clock divider to set.
     */
    void setClockDivider(uint8_t clockDiv);
};

#endif // SPI_H
