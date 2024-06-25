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

#include "configctl.h"

ConfigCtl::ConfigCtl()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An error has occurred while mounting SPIFFS");
    }
}

bool ConfigCtl::loadConfig()
{
    File configFile = SPIFFS.open(CONFIG_FILE, "r");
    if (!configFile)
    {
        Serial.println("Failed to open config file");
        return false;
    }

    size_t size = configFile.size();
    if (size > 1024)
    {
        Serial.println("Config file size is too large");
        return false;
    }

    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, buf.get());
    if (error)
    {
        Serial.println("Failed to parse config file");
        return false;
    }

    _wifiSsid = doc["wifi_ssid"] | "";
    _wifiPassword = doc["wifi_password"] | "";
    _apiKey = doc["api_key"] | "";

    return true;
}

bool ConfigCtl::saveConfig()
{
    DynamicJsonDocument doc(1024);
    doc["wifi_ssid"] = _wifiSsid;
    doc["wifi_password"] = _wifiPassword;
    doc["api_key"] = _apiKey;

    File configFile = SPIFFS.open(CONFIG_FILE, "w");
    if (!configFile)
    {
        Serial.println("Failed to open config file for writing");
        return false;
    }

    serializeJson(doc, configFile);
    return true;
}
