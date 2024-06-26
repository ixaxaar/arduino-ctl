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

#include "I2Sctl.h"
#include <sstream>
#include "base64.hpp"

I2SCtl::I2SCtl() : _i2sPort(I2S_NUM_0)
{
    _i2sConfig = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0};
    _i2sPins = {
        .bck_io_num = 26,
        .ws_io_num = 25,
        .data_out_num = 22,
        .data_in_num = 21};
}

void I2SCtl::init(const std::vector<std::pair<std::string, std::string>> &params)
{
    for (const auto &param : params)
    {
        if (param.first == "i2sPort")
        {
            _i2sPort = (i2s_port_t)std::stoi(param.second);
        }
    }
    i2s_driver_install(_i2sPort, &_i2sConfig, 0, NULL);
    i2s_set_pin(_i2sPort, &_i2sPins);
}

void I2SCtl::deinit()
{
    i2s_driver_uninstall(_i2sPort);
}

std::pair<std::string, void *> I2SCtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "readData")
    {
        size_t numBytes = 0;
        for (const auto &param : params)
        {
            if (param.first == "numBytes")
            {
                numBytes = std::stoul(param.second);
                break;
            }
        }
        std::vector<uint8_t> result = readData(numBytes);
        return {"std::vector<uint8_t>", new std::vector<uint8_t>(result)};
    }
    else if (command == "writeData")
    {
        std::vector<uint8_t> data;
        for (const auto &param : params)
        {
            if (param.first == "data")
            {
                data.resize(decode_base64_length(reinterpret_cast<const unsigned char *>(param.second.c_str())));
                decode_base64(reinterpret_cast<const unsigned char *>(param.second.c_str()), param.second.length(), data.data());
                break;
            }
        }
        writeData(data);
        return {"", nullptr};
    }
    return {"", nullptr};
}

std::vector<FunctionInfo> I2SCtl::getSupportedFunctions()
{
    return {
        {"readData", {{"numBytes", "size_t"}}},
        {"writeData", {{"data", "std::vector<uint8_t>"}}}};
}

std::vector<uint8_t> I2SCtl::readData(size_t numBytes)
{
    std::vector<uint8_t> data(numBytes);
    size_t bytesRead;
    i2s_read(_i2sPort, data.data(), numBytes, &bytesRead, portMAX_DELAY);
    data.resize(bytesRead);
    return data;
}

void I2SCtl::writeData(const std::vector<uint8_t> &data)
{
    size_t bytesWritten;
    i2s_write(_i2sPort, data.data(), data.size(), &bytesWritten, portMAX_DELAY);
}

void I2SCtl::setConfig(const i2s_config_t &config)
{
    _i2sConfig = config;
    i2s_driver_uninstall(_i2sPort);
    i2s_driver_install(_i2sPort, &_i2sConfig, 0, NULL);
}

void I2SCtl::setPins(const i2s_pin_config_t &pins)
{
    _i2sPins = pins;
    i2s_set_pin(_i2sPort, &_i2sPins);
}
