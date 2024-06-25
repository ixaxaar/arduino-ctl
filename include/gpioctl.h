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

#ifndef GPIO_H
#define GPIO_H

#include <Arduino.h>
#include <vector>
#include <string>
#include "module.h"

/**
 * @brief GPIO control module for Arduino-CTL
 *
 * This class implements the ModuleInterface for GPIO operations.
 */
class GPIOCtl : public ModuleInterface
{
public:
    /**
     * @brief Constructor for GPIOCtl
     *
     * Initializes the GPIO control module with default values.
     */
    GPIOCtl();

    /**
     * @brief Initialize the GPIO module
     *
     * @param params A vector of parameter name-value pairs for initialization
     *               Expected parameters:
     *               - "pin": The GPIO pin number to control
     *               - "mode": The pin mode (INPUT, OUTPUT, INPUT_PULLUP)
     */
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief De-initialize the GPIO module
     *
     * This method is called when the module is no longer needed.
     * It can be used to reset the GPIO pin to a safe state.
     */
    void deinit() override;

    /**
     * @brief Execute a command on the GPIO module
     *
     * @param command The command to execute ("setPinMode", "digitalRead", or "digitalWrite")
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
    int _pin;  ///< The GPIO pin number being controlled
    int _mode; ///< The current mode of the GPIO pin

    /**
     * @brief Set the mode of the GPIO pin
     *
     * @param mode The mode to set (INPUT, OUTPUT, INPUT_PULLUP)
     */
    void setPinMode(int mode);

    /**
     * @brief Read the digital value of the GPIO pin
     *
     * @param numSamples The number of samples to read
     * @return A vector of int values read from the pin (0 or 1)
     */
    std::vector<int> digitalRead(int numSamples);

    /**
     * @brief Write digital values to the GPIO pin
     *
     * @param values A vector of int values to write to the pin (0 or 1)
     */
    void digitalWrite(const std::vector<int> &values);
};

#endif // GPIO_H
