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

#include "arduinoctl.h"
#include <algorithm>

AsyncWebServer server(80);
RemoteControlServer remoteServer;

// Helper function to convert Arduino String to std::string
std::string to_std_string(const String &arduino_string)
{
    return std::string(arduino_string.c_str(), arduino_string.length());
}

// Helper function to convert std::string to Arduino String
String to_arduino_string(const std::string &std_string)
{
    return String(std_string.c_str());
}

void handleExecute(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (len > 0)
    {
        std::string json(reinterpret_cast<char *>(data), len);
        std::string result = remoteServer.executeCommands(json);
        request->send(200, "application/json", to_arduino_string(result));
    }
    else
    {
        request->send(400, "text/plain", "No data received");
    }
}

RemoteControlServer::RemoteControlServer() {}

bool RemoteControlServer::begin()
{
    if (!configCtl.loadConfig())
    {
        Serial.println("Failed to load configuration. Using default values.");
        configCtl.setWifiSsid("DefaultSSID");
        configCtl.setWifiPassword("DefaultPassword");
        configCtl.setApiKey("DefaultAPIKey");
        configCtl.saveConfig();
    }

    // Connect to Wi-Fi
    WiFi.begin(configCtl.getWifiSsid().c_str(), configCtl.getWifiPassword().c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
        attempts++;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Failed to connect to WiFi");
        return false;
    }

    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Set up web server
    server.on("/execute", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleExecute);

    server.begin();
    Serial.println("HTTP server started");

    return true;
}

void RemoteControlServer::registerModule(const std::string &name, std::shared_ptr<ModuleInterface> module)
{
    modules.push_back(std::make_pair(name, module));
}

std::string RemoteControlServer::executeCommands(const std::string &jsonCommands)
{
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonCommands);

    if (error)
    {
        return "{\"error\": \"Failed to parse JSON\"}";
    }

    // Check API key
    if (doc["api_key"] != configCtl.getApiKey())
    {
        return "{\"error\": \"Invalid API key\"}";
    }

    JsonArray commands = doc["commands"];
    DynamicJsonDocument resultDoc(1024);
    JsonArray results = resultDoc.createNestedArray("results");

    for (JsonObject command : commands)
    {
        std::string moduleName = command["module"].as<std::string>();
        std::string commandName = command["command"].as<std::string>();
        JsonObject params = command["params"];

        std::vector<std::pair<std::string, std::string>> paramPairs;
        for (JsonPair p : params)
        {
            paramPairs.emplace_back(p.key().c_str(), p.value().as<std::string>());
        }

        std::string result = executeCommand(moduleName, commandName, paramPairs);
        results.add(serialized(result));
    }

    std::string response;
    serializeJson(resultDoc, response);
    return response;
}

std::string RemoteControlServer::executeCommand(const std::string &moduleName, const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    for (const auto &module : modules)
    {
        if (module.first == moduleName)
        {
            std::pair<std::string, void *> result = module.second->execute(command, params);

            if (result.first == "std::vector<int>" || result.first == "std::vector<uint8_t>")
            {
                std::vector<uint8_t> *data;
                if (result.first == "std::vector<int>")
                {
                    std::vector<int> *intData = static_cast<std::vector<int> *>(result.second);
                    data = new std::vector<uint8_t>(reinterpret_cast<uint8_t *>(intData->data()), reinterpret_cast<uint8_t *>(intData->data() + intData->size()));
                    delete intData;
                }
                else
                {
                    data = static_cast<std::vector<uint8_t> *>(result.second);
                }

                std::string base64Data = encodeBase64(*data);
                delete data;
                return "{\"data\":\"" + base64Data + "\"}";
            }
            else if (result.first == "int")
            {
                int *data = static_cast<int *>(result.second);
                std::string jsonResult = std::to_string(*data);
                delete data;
                return "{\"data\":" + jsonResult + "}";
            }
            else
            {
                return "{\"data\": null}";
            }
        }
    }
    return "{\"error\": \"Module not found\"}";
}

void setup()
{
    Serial.begin(115200);

    // Initialize modules
    auto analogModule = std::make_shared<AnalogCtl>();
    auto gpioModule = std::make_shared<GPIOCtl>();
    auto i2cModule = std::make_shared<I2CCtl>();
    auto i2sModule = std::make_shared<I2SCtl>();
    auto spiModule = std::make_shared<SPICtl>();

    // Register modules
    remoteServer.registerModule("analog", analogModule);
    remoteServer.registerModule("gpio", gpioModule);
    remoteServer.registerModule("i2c", i2cModule);
    remoteServer.registerModule("i2s", i2sModule);
    remoteServer.registerModule("spi", spiModule);

    if (!remoteServer.begin())
    {
        Serial.println("Failed to start RemoteControlServer");
        while (1)
        {
            delay(1000);
        } // Infinite loop if setup fails
    }
}

void loop()
{
    // The AsyncWebServer is non-blocking, so we don't need to call server.handleClient()
    // You can add any other continuous tasks here if needed
    delay(1000); // Small delay to prevent watchdog timer issues
}
