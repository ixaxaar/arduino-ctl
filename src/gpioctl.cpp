
#include "gpioctl.h"

GPIOCtl::GPIOCtl() : _pin(0), _mode(0) {}

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

void GPIOCtl::deinit()
{
    // No specific deinitialization required for GPIO
}

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

std::vector<FunctionInfo> GPIOCtl::getSupportedFunctions()
{
    return {
        {"pinMode", {{"mode", "int"}}},
        {"digitalWrite", {{"value", "int"}}},
        {"digitalRead", {}},
        {"attachInterrupt", {{"callback", "void (*)(void)"}, {"mode", "int"}}},
        {"detachInterrupt", {}}};
}

void GPIOCtl::pinMode(int mode)
{
    _mode = mode;
    ::pinMode(_pin, _mode);
}

void GPIOCtl::digitalWrite(int value)
{
    ::digitalWrite(_pin, value);
}

int *GPIOCtl::digitalRead()
{
    int *value = new int(::digitalRead(_pin));
    return value;
}

void GPIOCtl::attachInterrupt(void (*callback)(void), int mode)
{
    ::attachInterrupt(_pin, callback, mode);
}

void GPIOCtl::detachInterrupt()
{
    ::detachInterrupt(_pin);
}
