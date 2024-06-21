

#include "analogctl.h"
#include <sstream>

AnalogCtl::AnalogCtl() : _pin(0), _resolution(10) {}

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
            analogReadResolution(_resolution);
        }
    }
    pinMode(_pin, INPUT);
}

void AnalogCtl::deinit()
{
    // No specific deinitialization needed for analog pins
}

std::pair<std::string, void *> AnalogCtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "readAnalog")
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
        std::vector<int> result = readAnalog(numSamples);
        return {"std::vector<int>", new std::vector<int>(result)};
    }
    else if (command == "writeAnalog")
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
        writeAnalog(values);
        return {"", nullptr};
    }
    return {"", nullptr};
}

std::vector<FunctionInfo> AnalogCtl::getSupportedFunctions()
{
    return {
        {"readAnalog", {{"numSamples", "int"}}},
        {"writeAnalog", {{"values", "std::vector<int>"}}}};
}

std::vector<int> AnalogCtl::readAnalog(int numSamples)
{
    std::vector<int> samples;
    for (int i = 0; i < numSamples; ++i)
    {
        samples.push_back(analogRead(_pin));
        delay(1); // Short delay between readings
    }
    return samples;
}

void AnalogCtl::writeAnalog(const std::vector<int> &values)
{
    for (int value : values)
    {
        analogWrite(_pin, value);
        delay(1); // Short delay between writes
    }
}
