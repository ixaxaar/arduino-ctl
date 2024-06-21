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

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "main.h"
#include "analogctl.h"
#include "gpioctl.h"
#include "I2Cctl.h"
#include "I2Sctl.h"
#include "SPIctl.h"

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
