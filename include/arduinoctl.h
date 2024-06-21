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

class RemoteControlServer
{
public:
    RemoteControlServer();
    void registerModule(const String &name, std::shared_ptr<ModuleInterface> module);
    String executeCommands(const String &jsonCommands);

private:
    std::vector<std::pair<String, std::shared_ptr<ModuleInterface>>> modules;
    String executeCommand(const String &moduleName, const String &command, const std::vector<std::pair<String, String>> &params);
};

#endif // ARDUINOCTL_H
