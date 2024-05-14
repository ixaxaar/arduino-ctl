#include "gpioctl.h"

/**
 * @brief Constructor for the GPIOCtl class.
 *
 * Initializes the GPIO pin and mode to default values.
 */
GPIOCtl::GPIOCtl() : _pin(0), _mode(0) {}

/**
 * @brief Initializes the GPIO pin with the provided parameters.
 *
 * @param params A vector of pairs containing parameter names and values.
 */
void GPIOCtl::init(const std::vector<std::pair<std::string, std::string>> &params)
{
    for (const auto &param : params)
    {
        if (param.first == "pin")
        {
            _pin = std::stoi(param.second);
        }
        else if (param.first == "mode")
        {
            _mode = std::stoi(param.second);
        }
    }

    pinMode(_mode);
}

/**
 * @brief Deinitializes the GPIO pin.
 *
 * No specific deinitialization required for GPIO.
 */
void GPIOCtl::deinit()
{
    // No specific deinitialization required for GPIO
}

/**
 * @brief Executes a command with the provided parameters.
 *
 * @param command The command to execute.
 * @param params A vector of pairs containing parameter names and values.
 * @return A pair containing the return type and a pointer to the result.
 */
std::pair<std::string, void *> GPIOCtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "pinMode")
    {
        int mode = 0;
        for (const auto &param : params)
        {
            if (param.first == "mode")
            {
                mode = std::stoi(param.second);
                break;
            }
        }
        pinMode(mode);
        return {"", nullptr};
    }
    else if (command == "digitalWrite")
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
        digitalWrite(value);
        return {"", nullptr};
    }
    else if (command == "digitalRead")
    {
        int *value = digitalRead();
        return {"int", static_cast<void *>(value)};
    }
    else if (command == "attachInterrupt")
    {
        void (*callback)(void) = nullptr;
        int mode = 0;
        for (const auto &param : params)
        {
            if (param.first == "callback")
            {
                // Assuming the callback is passed as a function pointer
                callback = reinterpret_cast<void (*)(void)>(std::stoul(param.second, nullptr, 16));
            }
            else if (param.first == "mode")
            {
                mode = std::stoi(param.second);
            }
        }
        attachInterrupt(callback, mode);
        return {"", nullptr};
    }
    else if (command == "detachInterrupt")
    {
        detachInterrupt();
        return {"", nullptr};
    }
    return {"", nullptr};
}

/**
 * @brief Returns a vector of supported functions and their parameter information.
 *
 * @return A vector of FunctionInfo structs.
 */
std::vector<FunctionInfo> GPIOCtl::getSupportedFunctions()
{
    return {
        {"pinMode", {{"mode", "int"}}},
        {"digitalWrite", {{"value", "int"}}},
        {"digitalRead", {}},
        {"attachInterrupt", {{"callback", "void (*)(void)"}, {"mode", "int"}}},
        {"detachInterrupt", {}}};
}

/**
 * @brief Sets the mode of the GPIO pin.
 *
 * @param mode The pin mode to set.
 */
void GPIOCtl::pinMode(int mode)
{
    _mode = mode;
    ::pinMode(_pin, _mode);
}

/**
 * @brief Writes a digital value to the GPIO pin.
 *
 * @param value The digital value to write.
 */
void GPIOCtl::digitalWrite(int value)
{
    ::digitalWrite(_pin, value);
}

/**
 * @brief Reads a digital value from the GPIO pin.
 *
 * @return A pointer to the read digital value.
 */
int *GPIOCtl::digitalRead()
{
    int *value = new int(::digitalRead(_pin));
    return value;
}

/**
 * @brief Attaches an interrupt to the GPIO pin.
 *
 * @param callback The interrupt callback function.
 * @param mode The interrupt mode.
 */
void GPIOCtl::attachInterrupt(void (*callback)(void), int mode)
{
    ::attachInterrupt(_pin, callback, mode);
}

/**
 * @brief Detaches the interrupt from the GPIO pin.
 */
void GPIOCtl::detachInterrupt()
{
    ::detachInterrupt(_pin);
}
