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

#ifndef GPIO_H
#define GPIO_H

#include <Arduino.h>
#include <vector>
#include <string>
#include "module.h"

/**
 * @class GPIOCtl
 * @brief Provides functionality for GPIO control on an Arduino board.
 *
 * The GPIOCtl class allows you to initialize and deinitialize GPIO pins,
 * set pin modes, write and read digital values, attach and detach interrupts.
 */
class GPIOCtl : public ModuleInterface
{
public:
    /**
     * @brief Constructor for the GPIOCtl class.
     */
    GPIOCtl();

    /**
     * @brief Initializes the GPIO pin with the provided parameters.
     *
     * @param params A vector of pairs containing parameter names and values.
     */
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief Deinitializes the GPIO pin.
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
    int _pin;  ///< The GPIO pin number.
    int _mode; ///< The GPIO pin mode.

    /**
     * @brief Sets the mode of the GPIO pin.
     *
     * @param mode The pin mode to set.
     */
    void pinMode(int mode);

    /**
     * @brief Writes a digital value to the GPIO pin.
     *
     * @param value The digital value to write.
     */
    void digitalWrite(int value);

    /**
     * @brief Reads a digital value from the GPIO pin.
     *
     * @return A pointer to the read digital value.
     */
    int *digitalRead();

    /**
     * @brief Attaches an interrupt to the GPIO pin.
     *
     * @param callback The interrupt callback function.
     * @param mode The interrupt mode.
     */
    void attachInterrupt(void (*callback)(void), int mode);

    /**
     * @brief Detaches the interrupt from the GPIO pin.
     */
    void detachInterrupt();
};

#endif // GPIO_H
