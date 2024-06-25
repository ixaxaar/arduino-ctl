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

#ifndef ARDUINOCTL_H
#define ARDUINOCTL_H

#include <Arduino.h>
#include <vector>
#include <memory>
#include "module.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "analogctl.h"
#include "gpioctl.h"
#include "I2Cctl.h"
#include "I2Sctl.h"
#include "SPIctl.h"

/**
 * @brief Main control server for Arduino-CTL
 *
 * This class manages the remote control server, handling module registration and command execution.
 */
class RemoteControlServer
{
public:
    /**
     * @brief Constructor for RemoteControlServer
     */
    RemoteControlServer();

    /**
     * @brief Register a module with the server
     * @param name Name of the module
     * @param module Shared pointer to the module
     */
    void registerModule(const String &name, std::shared_ptr<ModuleInterface> module);

    /**
     * @brief Execute a set of commands
     * @param jsonCommands JSON string containing the commands to execute
     * @return JSON string containing the results of the executed commands
     */
    String executeCommands(const String &jsonCommands);

private:
    std::vector<std::pair<String, std::shared_ptr<ModuleInterface>>> modules; ///< Vector of registered modules

    /**
     * @brief Execute a single command on a specific module
     * @param moduleName Name of the module to execute the command on
     * @param command The command to execute
     * @param params Vector of parameter name-value pairs for the command
     * @return JSON string containing the result of the executed command
     */
    String executeCommand(const String &moduleName, const String &command, const std::vector<std::pair<String, String>> &params);
};

#endif // ARDUINOCTL_H
