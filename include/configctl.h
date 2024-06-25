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

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

/**
 * @brief Configuration management module for Arduino-CTL
 *
 * This class handles loading, saving, and accessing configuration settings.
 */
class ConfigCtl
{
public:
    /**
     * @brief Constructor for ConfigCtl
     *
     * Initializes the SPIFFS file system.
     */
    ConfigCtl();

    /**
     * @brief Load configuration from file
     * @return true if successful, false otherwise
     */
    bool loadConfig();

    /**
     * @brief Save current configuration to file
     * @return true if successful, false otherwise
     */
    bool saveConfig();

    /**
     * @brief Get WiFi SSID
     * @return WiFi SSID as std::string
     */
    std::string getWifiSsid() const { return _wifiSsid; }

    /**
     * @brief Get WiFi password
     * @return WiFi password as std::string
     */
    std::string getWifiPassword() const { return _wifiPassword; }

    /**
     * @brief Get API key for authentication
     * @return API key as std::string
     */
    std::string getApiKey() const { return _apiKey; }

    /**
     * @brief Set WiFi SSID
     * @param ssid WiFi SSID
     */
    void setWifiSsid(const std::string &ssid) { _wifiSsid = ssid; }

    /**
     * @brief Set WiFi password
     * @param password WiFi password
     */
    void setWifiPassword(const std::string &password) { _wifiPassword = password; }

    /**
     * @brief Set API key for authentication
     * @param apiKey API key
     */
    void setApiKey(const std::string &apiKey) { _apiKey = apiKey; }

private:
    std::string _wifiSsid;     ///< WiFi SSID
    std::string _wifiPassword; ///< WiFi password
    std::string _apiKey;       ///< API key for authentication

    const char *CONFIG_FILE = "/config.json"; ///< Path to the configuration file
};

#endif // CONFIG_H
