#include "gpioctl.h"
#include <sstream>

GPIOCtl::GPIOCtl() : _pin(0), _mode(INPUT) {}

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
    pinMode(_pin, _mode);
}

void GPIOCtl::deinit()
{
    // No specific deinitialization needed for GPIO pins
}

std::pair<std::string, void *> GPIOCtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "setPinMode")
    {
        int mode = INPUT;
        for (const auto &param : params)
        {
            if (param.first == "mode")
            {
                mode = std::stoi(param.second);
                break;
            }
        }
        setPinMode(mode);
        return {"", nullptr};
    }
    else if (command == "digitalRead")
    {
        int numSamples = 1;
        for (const auto &param : params)
        {
            if (param.first == "numSamples")
            {
                numSamples = std::stoi(param.second);
                break;
            }
        }
        std::vector<int> result = digitalRead(numSamples);
        return {"std::vector<int>", new std::vector<int>(result)};
    }
    else if (command == "digitalWrite")
    {
        std::vector<int> values;
        for (const auto &param : params)
        {
            if (param.first == "values")
            {
                std::istringstream iss(param.second);
                std::string token;
                while (std::getline(iss, token, ','))
                {
                    values.push_back(std::stoi(token));
                }
                break;
            }
        }
        digitalWrite(values);
        return {"", nullptr};
    }
    return {"", nullptr};
}

std::vector<FunctionInfo> GPIOCtl::getSupportedFunctions()
{
    return {
        {"setPinMode", {{"mode", "int"}}},
        {"digitalRead", {{"numSamples", "int"}}},
        {"digitalWrite", {{"values", "std::vector<int>"}}}};
}

void GPIOCtl::setPinMode(int mode)
{
    _mode = mode;
    pinMode(_pin, _mode);
}

std::vector<int> GPIOCtl::digitalRead(int numSamples)
{
    std::vector<int> samples;
    for (int i = 0; i < numSamples; ++i)
    {
        samples.push_back(::digitalRead(_pin));
        delay(1); // Short delay between readings
    }
    return samples;
}

void GPIOCtl::digitalWrite(const std::vector<int> &values)
{
    for (int value : values)
    {
        ::digitalWrite(_pin, value);
        delay(1); // Short delay between writes
    }
}
