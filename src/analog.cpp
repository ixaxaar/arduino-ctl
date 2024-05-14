#include "analog.h"

/**
 * @brief Constructor for the AnalogCtl class.
 *
 * Initializes the analog pin, resolution, and frequency to default values.
 */
AnalogCtl::AnalogCtl() : _pin(0), _resolution(12), _frequency(5000) {}

/**
 * @brief Initializes the analog control with the provided parameters.
 *
 * @param params A vector of pairs containing parameter names and values.
 */
void AnalogCtl::init(const std::vector<std::pair<std::string, std::string>> &params)
{
    for (const auto &param : params)
    {
        if (param.first == "pin")
        {
            _pin = std::stoi(param.second);
        }
        else if (param.first == "resolution")
        {
            _resolution = std::stoi(param.second);
        }
        else if (param.first == "frequency")
        {
            _frequency = std::stoi(param.second);
        }
    }

    analogReadResolution(_resolution);
    ledcSetup(0, _frequency, _resolution);
    ledcAttachPin(_pin, 0);
}

/**
 * @brief Deinitializes the analog control.
 */
void AnalogCtl::deinit()
{
    ledcDetachPin(_pin);
}

/**
 * @brief Executes a command with the provided parameters.
 *
 * @param command The command to execute.
 * @param params A vector of pairs containing parameter names and values.
 * @return A pair containing the return type and a pointer to the result.
 */
std::pair<std::string, void *> AnalogCtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "writeAnalog")
    {
        int value = 0;
        for (const auto &param : params)
        {
            if (param.first == "value")
            {
                value = std::stoi(param.second);
                break;
            }
        }
        writeAnalog(value);
        return {"", nullptr};
    }
    else if (command == "readAnalog")
    {
        int *value = readAnalog();
        return {"int", static_cast<void *>(value)};
    }
    else if (command == "setResolution")
    {
        int resolution = 0;
        for (const auto &param : params)
        {
            if (param.first == "resolution")
            {
                resolution = std::stoi(param.second);
                break;
            }
        }
        setResolution(resolution);
        return {"", nullptr};
    }
    else if (command == "setFrequency")
    {
        int frequency = 0;
        for (const auto &param : params)
        {
            if (param.first == "frequency")
            {
                frequency = std::stoi(param.second);
                break;
            }
        }
        setFrequency(frequency);
        return {"", nullptr};
    }
    return {"", nullptr};
}

/**
 * @brief Returns a vector of supported functions and their parameter information.
 *
 * @return A vector of FunctionInfo structs.
 */
std::vector<FunctionInfo> AnalogCtl::getSupportedFunctions()
{
    return {
        {"writeAnalog", {{"value", "int"}}},
        {"readAnalog", {}},
        {"setResolution", {{"resolution", "int"}}},
        {"setFrequency", {{"frequency", "int"}}}};
}

/**
 * @brief Writes an analog value to the specified pin.
 *
 * @param value The analog value to write.
 */
void AnalogCtl::writeAnalog(int value)
{
    ledcWrite(0, value);
}

/**
 * @brief Reads an analog value from the specified pin.
 *
 * @return A pointer to the read analog value.
 */
int *AnalogCtl::readAnalog()
{
    int *value = new int(analogRead(_pin));
    return value;
}

/**
 * @brief Sets the resolution for analog read and write operations.
 *
 * @param resolution The analog resolution to set.
 */
void AnalogCtl::setResolution(int resolution)
{
    _resolution = resolution;
    analogReadResolution(_resolution);
    ledcSetup(0, _frequency, _resolution);
}

/**
 * @brief Sets the frequency for analog operations.
 *
 * @param frequency The analog frequency to set.
 */
void AnalogCtl::setFrequency(int frequency)
{
    _frequency = frequency;
    ledcSetup(0, _frequency, _resolution);
}
