#ifndef I2S_H
#define I2S_H

#include <Arduino.h>
#include <driver/i2s.h>
#include <vector>
#include <string>
#include "module.h"

/**
 * @class I2SCtl
 * @brief Provides functionality for I2S communication on an Arduino board.
 *
 * The I2SCtl class allows you to initialize and deinitialize I2S communication,
 * write and read data, and set the buffer size.
 */
class I2SCtl : public ModuleInterface
{
public:
    /**
     * @brief Constructor for the I2SCtl class.
     */
    I2SCtl();

    /**
     * @brief Initializes I2S communication with the provided parameters.
     *
     * @param params A vector of pairs containing parameter names and values.
     */
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief Deinitializes I2S communication.
     */
    void deinit() override;

    /**
     * @brief Executes a command with the provided parameters.
     *
     * @param command The command to execute.
     * @param params A vector of pairs containing parameter names and values.
     * @return A pair containing the return type and a pointer to the result.
     */
    std::pair<std::string, void *> execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params) override;

    /**
     * @brief Returns a vector of supported functions and their parameter information.
     *
     * @return A vector of FunctionInfo structs.
     */
    std::vector<FunctionInfo> getSupportedFunctions() override;

private:
    i2s_port_t _i2sPort;     ///< The I2S port number.
    i2s_config_t _i2sConfig; ///< The I2S configuration.
    size_t _bufferSize;      ///< The buffer size for I2S communication.

    static const i2s_config_t defconf; ///< Default I2S configuration.

    /**
     * @brief Writes data to the I2S bus.
     *
     * @param data A pointer to the data to write.
     * @param size The size of the data in bytes.
     */
    void write(const uint8_t *data, size_t size);

    /**
     * @brief Reads data from the I2S bus.
     *
     * @param size The size of the data to read in bytes.
     * @return A pointer to the read data.
     */
    uint8_t *read(size_t size);

    /**
     * @brief Sets the buffer size for I2S communication.
     *
     * @param bufferSize The buffer size to set.
     */
    void setBufferSize(size_t bufferSize);
};

#endif // I2S_H
