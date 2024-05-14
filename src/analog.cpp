#include "analog.h"

AnalogCtl::AnalogCtl() : _pin(0), _resolution(12), _frequency(5000) {}

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

void AnalogCtl::deinit()
{
    ledcDetachPin(_pin);
}

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

std::vector<FunctionInfo> AnalogCtl::getSupportedFunctions()
{
    return {
        {"writeAnalog", {{"value", "int"}}},
        {"readAnalog", {}},
        {"setResolution", {{"resolution", "int"}}},
        {"setFrequency", {{"frequency", "int"}}}};
}

void AnalogCtl::writeAnalog(int value)
{
    ledcWrite(0, value);
}

int *AnalogCtl::readAnalog()
{
    int *value = new int(analogRead(_pin));
    return value;
}

void AnalogCtl::setResolution(int resolution)
{
    _resolution = resolution;
    analogReadResolution(_resolution);
    ledcSetup(0, _frequency, _resolution);
}

void AnalogCtl::setFrequency(int frequency)
{
    _frequency = frequency;
    ledcSetup(0, _frequency, _resolution);
}
