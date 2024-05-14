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

#ifndef ANALOG_H
#define ANALOG_H

#include <Arduino.h>
#include <vector>
#include <string>
#include "module.h"

/**
 * @class AnalogCtl
 * @brief Provides functionality for analog control on an Arduino board.
 *
 * The AnalogCtl class allows you to initialize and deinitialize the analog control,
 * write analog values, read analog values, set the resolution, and set the frequency.
 */
class AnalogCtl : public ModuleInterface
{
public:
    /**
     * @brief Constructor for the AnalogCtl class.
     */
    AnalogCtl();

    /**
     * @brief Initializes the analog control with the provided parameters.
     *
     * @param params A vector of pairs containing parameter names and values.
     */
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief Deinitializes the analog control.
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
    int _pin;        ///< The analog pin number.
    int _resolution; ///< The analog resolution.
    int _frequency;  ///< The analog frequency.

    /**
     * @brief Writes an analog value to the specified pin.
     *
     * @param value The analog value to write.
     */
    void writeAnalog(int value);

    /**
     * @brief Reads an analog value from the specified pin.
     *
     * @return A pointer to the read analog value.
     */
    int *readAnalog();

    /**
     * @brief Sets the resolution for analog read and write operations.
     *
     * @param resolution The analog resolution to set.
     */
    void setResolution(int resolution);

    /**
     * @brief Sets the frequency for analog operations.
     *
     * @param frequency The analog frequency to set.
     */
    void setFrequency(int frequency);
};

std::vector<FunctionInfo> AnalogCtl::getSupportedFunctions()
{
    return {
        {"writeAnalog", {{"value", "int"}}},
        {"readAnalog", {}},
        {"setResolution", {{"resolution", "int"}}},
        {"setFrequency", {{"frequency", "int"}}}};
}

#endif // ANALOG_H
