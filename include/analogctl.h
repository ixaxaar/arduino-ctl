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

#ifndef ANALOG_H
#define ANALOG_H

#include <Arduino.h>
#include <vector>
#include <string>
#include "module.h"

/**
 * @brief Analog control module for Arduino-CTL
 *
 * This class implements the ModuleInterface for analog operations,
 * including reading from analog inputs and writing to analog outputs (PWM).
 */
class AnalogCtl : public ModuleInterface
{
public:
    /**
     * @brief Constructor for AnalogCtl
     *
     * Initializes the Analog control module with default settings.
     */
    AnalogCtl();

    /**
     * @brief Initialize the Analog module
     *
     * @param params A vector of parameter name-value pairs for initialization
     *               Expected parameters:
     *               - "pin": The analog pin number to use
     *               - "resolution": The ADC resolution in bits (e.g., 10 for 10-bit resolution)
     */
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief De-initialize the Analog module
     *
     * This method is called when the module is no longer needed.
     * It can be used to reset the analog pin to a safe state if necessary.
     */
    void deinit() override;

    /**
     * @brief Execute a command on the Analog module
     *
     * @param command The command to execute ("readAnalog" or "writeAnalog")
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
    int _pin;        ///< The analog pin number being used
    int _resolution; ///< The ADC resolution in bits

    /**
     * @brief Read analog values from the specified pin
     *
     * @param numSamples The number of samples to read
     * @return A vector of int values read from the analog pin
     */
    std::vector<int> readAnalog(int numSamples);

    /**
     * @brief Write analog values (PWM) to the specified pin
     *
     * @param values A vector of int values to write to the analog pin
     */
    void writeAnalog(const std::vector<int> &values);
};

#endif // ANALOG_H
