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

/**
 * @brief Default I2S configuration.
 */
const i2s_config_t I2SCtl::defconf = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false};

/**
 * @brief Constructor for the I2SCtl class.
 *
 * Initializes the I2S port, configuration, and buffer size to default values.
 */
I2SCtl::I2SCtl() : _i2sPort(I2S_NUM_0), _i2sConfig(defconf), _bufferSize(defconf.dma_buf_len) {}

/**
 * @brief Initializes I2S communication with the provided parameters.
 *
 * @param params A vector of pairs containing parameter names and values.
 */
void I2SCtl::init(const std::vector<std::pair<std::string, std::string>> &params)
{
    for (const auto &param : params)
    {
        if (param.first == "i2sPort")
        {
            _i2sPort = (i2s_port_t)std::stoi(param.second);
        }
        else if (param.first == "mode")
        {
            _i2sConfig.mode = (i2s_mode_t)std::stoi(param.second);
        }
        else if (param.first == "sampleRate")
        {
            _i2sConfig.sample_rate = std::stoi(param.second);
        }
        else if (param.first == "bitsPerSample")
        {
            _i2sConfig.bits_per_sample = (i2s_bits_per_sample_t)std::stoi(param.second);
        }
        else if (param.first == "channelFormat")
        {
            _i2sConfig.channel_format = (i2s_channel_fmt_t)std::stoi(param.second);
        }
        else if (param.first == "communicationFormat")
        {
            _i2sConfig.communication_format = (i2s_comm_format_t)std::stoi(param.second);
        }
        else if (param.first == "dmaBufferCount")
        {
            _i2sConfig.dma_buf_count = std::stoi(param.second);
        }
        else if (param.first == "dmaBufferLength")
        {
            _i2sConfig.dma_buf_len = std::stoi(param.second);
            _bufferSize = _i2sConfig.dma_buf_len;
        }
    }

    i2s_driver_install(_i2sPort, &_i2sConfig, 0, NULL);
}

/**
 * @brief Deinitializes I2S communication.
 */
void I2SCtl::deinit()
{
    i2s_driver_uninstall(_i2sPort);
}

/**
 * @brief Executes a command with the provided parameters.
 *
 * @param command The command to execute.
 * @param params A vector of pairs containing parameter names and values.
 * @return A pair containing the return type and a pointer to the result.
 */
std::pair<std::string, void *> I2SCtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "write")
    {
        size_t size = 0;
        uint8_t *data = nullptr;
        for (const auto &param : params)
        {
            if (param.first == "data")
            {
                data = reinterpret_cast<uint8_t *>(std::stoul(param.second, nullptr, 16));
            }
            else if (param.first == "size")
            {
                size = std::stoul(param.second);
            }
        }
        if (data != nullptr && size > 0)
        {
            write(data, size);
            delete[] data;
        }
        return {"", nullptr};
    }
    else if (command == "read")
    {
        size_t size = 0;
        for (const auto &param : params)
        {
            if (param.first == "size")
            {
                size = std::stoul(param.second);
                break;
            }
        }
        uint8_t *data = read(size);
        return {"uint8_t[]", static_cast<void *>(data)};
    }
    else if (command == "setBufferSize")
    {
        size_t bufferSize = 0;
        for (const auto &param : params)
        {
            if (param.first == "bufferSize")
            {
                bufferSize = std::stoul(param.second);
                break;
            }
        }
        setBufferSize(bufferSize);
        return {"", nullptr};
    }
    return {"", nullptr};
}

/**
 * @brief Returns a vector of supported functions and their parameter information.
 *
 * @return A vector of FunctionInfo structs.
 */
std::vector<FunctionInfo> I2SCtl::getSupportedFunctions()
{
    return {
        {"write", {{"data", "uint8_t*"}, {"size", "size_t"}}},
        {"read", {{"size", "size_t"}}},
        {"setBufferSize", {{"bufferSize", "size_t"}}}};
}

/**
 * @brief Writes data to the I2S bus.
 *
 * @param data A pointer to the data to write.
 * @param size The size of the data in bytes.
 */
void I2SCtl::write(const uint8_t *data, size_t size)
{
    size_t bytesWritten;
    i2s_write(_i2sPort, data, size, &bytesWritten, portMAX_DELAY);
}

/**
 * @brief Reads data from the I2S bus.
 *
 * @param size The size of the data to read in bytes.
 * @return A pointer to the read data.
 */
uint8_t *I2SCtl::read(size_t size)
{
    uint8_t *data = new uint8_t[size];
    size_t bytesRead;
    i2s_read(_i2sPort, data, size, &bytesRead, portMAX_DELAY);
    return data;
}

/**
 * @brief Sets the buffer size for I2S communication.
 *
 * @param bufferSize The buffer size to set.
 */
void I2SCtl::setBufferSize(size_t bufferSize)
{
    _bufferSize = bufferSize;
}
