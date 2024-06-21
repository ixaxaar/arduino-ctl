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

const char *ssid = "YourWiFiSSID";
const char *password = "YourWiFiPassword";

AsyncWebServer server(80);
RemoteControlServer remoteServer;

void handleExecute(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (len > 0)
    {
        String json = String((char *)data);
        String result = remoteServer.executeCommands(json);
        request->send(200, "application/json", result);
    }
    else
    {
        request->send(400, "text/plain", "No data received");
    }
}

RemoteControlServer::RemoteControlServer() {}

void RemoteControlServer::registerModule(const String &name, std::shared_ptr<ModuleInterface> module)
{
    modules.push_back(std::make_pair(name, module));
}

String RemoteControlServer::executeCommands(const String &jsonCommands)
{
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonCommands);

    if (error)
    {
        return "{\"error\": \"Failed to parse JSON\"}";
    }

    JsonArray commands = doc["commands"];
    DynamicJsonDocument resultDoc(1024);
    JsonArray results = resultDoc.createNestedArray("results");

    for (JsonObject command : commands)
    {
        String moduleName = command["module"];
        String commandName = command["command"];
        JsonObject params = command["params"];

        std::vector<std::pair<String, String>> paramPairs;
        for (JsonPair p : params)
        {
            paramPairs.emplace_back(p.key().c_str(), p.value().as<String>());
        }

        String result = executeCommand(moduleName, commandName, paramPairs);
        results.add(serialized(result));
    }

    String response;
    serializeJson(resultDoc, response);
    return response;
}

String RemoteControlServer::executeCommand(const String &moduleName, const String &command, const std::vector<std::pair<String, String>> &params)
{
    for (const auto &module : modules)
    {
        if (module.first == moduleName)
        {
            std::pair<String, void *> result = module.second->execute(command, params);

            if (result.first == "std::vector<int>")
            {
                std::vector<int> *data = static_cast<std::vector<int> *>(result.second);
                String jsonResult = "[";
                for (size_t i = 0; i < data->size(); ++i)
                {
                    jsonResult += String((*data)[i]);
                    if (i < data->size() - 1)
                        jsonResult += ",";
                }
                jsonResult += "]";
                delete data;
                return "{\"data\":" + jsonResult + "}";
            }
            else if (result.first == "std::vector<uint8_t>")
            {
                std::vector<uint8_t> *data = static_cast<std::vector<uint8_t> *>(result.second);
                String jsonResult = "[";
                for (size_t i = 0; i < data->size(); ++i)
                {
                    jsonResult += String((*data)[i]);
                    if (i < data->size() - 1)
                        jsonResult += ",";
                }
                jsonResult += "]";
                delete data;
                return "{\"data\":" + jsonResult + "}";
            }
            else if (result.first == "int")
            {
                int *data = static_cast<int *>(result.second);
                String jsonResult = String(*data);
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

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

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

    // Set up web server
    server.on("/execute", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleExecute);

    server.begin();
    Serial.println("HTTP server started");
}

void loop()
{
    // The AsyncWebServer is non-blocking, so we don't need to call server.handleClient()
    // You can add any other continuous tasks here if needed
    delay(1000); // Small delay to prevent watchdog timer issues
}
