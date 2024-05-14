#include "I2Cctl.h"

I2CCtl::I2CCtl() : _sdaPin(0), _sclPin(0), _frequency(100000) {}

void I2CCtl::init(const std::vector<std::pair<std::string, std::string>> &params)
{
    for (const auto &param : params)
    {
        if (param.first == "sdaPin")
        {
            _sdaPin = std::stoi(param.second);
        }
        else if (param.first == "sclPin")
        {
            _sclPin = std::stoi(param.second);
        }
        else if (param.first == "frequency")
        {
            _frequency = std::stoul(param.second);
        }
    }

    Wire.begin(_sdaPin, _sclPin);
    setClock(_frequency);
}

void I2CCtl::deinit()
{
    Wire.end();
}

std::pair<std::string, void *> I2CCtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "setClock")
    {
        uint32_t frequency = 0;
        for (const auto &param : params)
        {
            if (param.first == "frequency")
            {
                frequency = std::stoul(param.second);
                break;
            }
        }
        setClock(frequency);
        return {"", nullptr};
    }
    else if (command == "beginTransmission")
    {
        uint8_t address = 0;
        for (const auto &param : params)
        {
            if (param.first == "address")
            {
                address = std::stoul(param.second);
                break;
            }
        }
        beginTransmission(address);
        return {"", nullptr};
    }
    else if (command == "endTransmission")
    {
        bool stopBit = true;
        for (const auto &param : params)
        {
            if (param.first == "stopBit")
            {
                stopBit = (param.second == "true");
                break;
            }
        }
        uint8_t *result = endTransmission(stopBit);
        return {"uint8_t", static_cast<void *>(result)};
    }
    else if (command == "requestFrom")
    {
        uint8_t address = 0;
        uint8_t quantity = 0;
        bool stopBit = true;
        for (const auto &param : params)
        {
            if (param.first == "address")
            {
                address = std::stoul(param.second);
            }
            else if (param.first == "quantity")
            {
                quantity = std::stoul(param.second);
            }
            else if (param.first == "stopBit")
            {
                stopBit = (param.second == "true");
            }
        }
        uint8_t *result = requestFrom(address, quantity, stopBit);
        return {"uint8_t", static_cast<void *>(result)};
    }
    else if (command == "write")
    {
        if (params.size() == 1 && params[0].first == "data")
        {
            uint8_t data = std::stoul(params[0].second);
            write(data);
        }
        else if (params.size() == 2 && params[0].first == "data" && params[1].first == "quantity")
        {
            size_t quantity = std::stoul(params[1].second);
            uint8_t *data = new uint8_t[quantity];
            for (size_t i = 0; i < quantity; ++i)
            {
                data[i] = std::stoul(params[0].second);
            }
            write(data, quantity);
            delete[] data;
        }
        return {"", nullptr};
    }
    else if (command == "read")
    {
        size_t quantity = 0;
        for (const auto &param : params)
        {
            if (param.first == "quantity")
            {
                quantity = std::stoul(param.second);
                break;
            }
        }
        uint8_t *result = read(quantity);
        return {"uint8_t[]", static_cast<void *>(result)};
    }
    return {"", nullptr};
}

std::vector<FunctionInfo> I2CCtl::getSupportedFunctions()
{
    return {
        {"setClock", {{"frequency", "uint32_t"}}},
        {"beginTransmission", {{"address", "uint8_t"}}},
        {"endTransmission", {{"stopBit", "bool"}}},
        {"requestFrom", {{"address", "uint8_t"}, {"quantity", "uint8_t"}, {"stopBit", "bool"}}},
        {"write", {{"data", "uint8_t"}}},
        {"write", {{"data", "uint8_t"}, {"quantity", "size_t"}}},
        {"read", {{"quantity", "size_t"}}}};
}

void I2CCtl::setClock(uint32_t frequency)
{
    _frequency = frequency;
    Wire.setClock(_frequency);
}

void I2CCtl::beginTransmission(uint8_t address)
{
    Wire.beginTransmission(address);
}

uint8_t *I2CCtl::endTransmission(bool stopBit)
{
    uint8_t *result = new uint8_t(Wire.endTransmission(stopBit));
    return result;
}

uint8_t *I2CCtl::requestFrom(uint8_t address, uint8_t quantity, bool stopBit)
{
    uint8_t *result = new uint8_t(Wire.requestFrom(address, quantity, stopBit));
    return result;
}

void I2CCtl::write(uint8_t data)
{
    Wire.write(data);
}

void I2CCtl::write(const uint8_t *data, size_t quantity)
{
    Wire.write(data, quantity);
}

uint8_t *I2CCtl::read(size_t quantity)
{
    uint8_t *data = new uint8_t[quantity];
    for (size_t i = 0; i < quantity; ++i)
    {
        data[i] = Wire.read();
    }
    return data;
}
