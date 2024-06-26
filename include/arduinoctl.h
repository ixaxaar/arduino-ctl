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
#include <base64.hpp>
#include "analogctl.h"
#include "gpioctl.h"
#include "I2Cctl.h"
#include "I2Sctl.h"
#include "SPIctl.h"
#include "configctl.h"

/**
 * @brief Main class for remote control of Arduino modules
 *
 * The RemoteControlServer class manages the registration and execution of various
 * Arduino control modules. It handles WiFi connection, web server setup, and
 * command execution based on received JSON requests.
 */
class RemoteControlServer
{
public:
    /**
     * @brief Construct a new Remote Control Server object
     */
    RemoteControlServer();

    /**
     * @brief Register a new module with the server
     *
     * @param name The name of the module to be used in JSON requests
     * @param module A shared pointer to the module implementing the ModuleInterface
     */
    void registerModule(const std::string &name, std::shared_ptr<ModuleInterface> module);

    /**
     * @brief Execute a set of commands received as a JSON string
     *
     * @param jsonCommands A JSON string containing commands to be executed
     * @return std::string A JSON string containing the results of the executed commands
     */
    std::string executeCommands(const std::string &jsonCommands);

    /**
     * @brief Initialize the RemoteControlServer
     *
     * This method loads the configuration, connects to WiFi, and sets up the web server.
     *
     * @return true if initialization was successful, false otherwise
     */
    bool begin();

private:
    /**
     * @brief Vector of registered modules
     *
     * Each element is a pair containing the module name and a shared pointer to the module.
     */
    std::vector<std::pair<std::string, std::shared_ptr<ModuleInterface>>> modules;

    /**
     * @brief Execute a single command on a specific module
     *
     * @param moduleName The name of the module to execute the command on
     * @param command The name of the command to execute
     * @param params A vector of parameter name-value pairs for the command
     * @return std::string A JSON string containing the result of the executed command
     */
    std::string executeCommand(const std::string &moduleName, const std::string &command, const std::vector<std::pair<std::string, std::string>> &params);

    /**
     * @brief Configuration management object
     *
     * Handles loading, saving, and accessing configuration settings such as WiFi credentials and API key.
     */
    ConfigCtl configCtl;

    // Helper function to encode binary data to base64
    static std::string encodeBase64(const std::vector<uint8_t> &data)
    {
        size_t encodedLength = encode_base64_length(data.size());
        std::string encoded(encodedLength, 0);
        encode_base64(data.data(), data.size(), reinterpret_cast<unsigned char *>(&encoded[0]));
        return encoded;
    }

    // Helper function to decode base64 to binary data
    static std::vector<uint8_t> decodeBase64(const std::string &encoded)
    {
        std::vector<uint8_t> decoded(decode_base64_length(reinterpret_cast<const unsigned char *>(encoded.c_str())));
        decode_base64(reinterpret_cast<const unsigned char *>(encoded.c_str()), encoded.length(), decoded.data());
        return decoded;
    }
};

/**
 * @brief Global instance of the AsyncWebServer
 *
 * This server handles incoming HTTP requests on port 80.
 */
extern AsyncWebServer server;

/**
 * @brief Global instance of the RemoteControlServer
 *
 * This object manages the overall functionality of the Arduino-CTL system.
 */
extern RemoteControlServer remoteServer;

/**
 * @brief Handler function for the /execute endpoint
 *
 * This function is called when a POST request is received on the /execute endpoint.
 * It processes the received JSON data and executes the requested commands.
 *
 * @param request The AsyncWebServerRequest object containing the request details
 * @param data Pointer to the received data
 * @param len Length of the received data
 * @param index Starting index of the data chunk
 * @param total Total length of the data
 */
void handleExecute(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

/**
 * @brief Arduino setup function
 *
 * This function is called once when the Arduino boots. It initializes serial communication,
 * sets up the control modules, and starts the RemoteControlServer.
 */
void setup();

/**
 * @brief Arduino loop function
 *
 * This function is called repeatedly after setup. In this implementation, it simply
 * includes a small delay to prevent watchdog timer issues.
 */
void loop();

#endif // ARDUINOCTL_H
