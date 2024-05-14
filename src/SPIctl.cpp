
#include "SPIctl.h"

SPICtl::SPICtl() : _sckPin(SCK), _misoPin(MISO), _mosiPin(MOSI), _ssPin(SS) {}

void SPICtl::init(const std::vector<std::pair<std::string, std::string>> &params)
{
    for (const auto &param : params)
    {
        if (param.first == "sckPin")
        {
            _sckPin = std::stoi(param.second);
        }
        else if (param.first == "misoPin")
        {
            _misoPin = std::stoi(param.second);
        }
        else if (param.first == "mosiPin")
        {
            _mosiPin = std::stoi(param.second);
        }
        else if (param.first == "ssPin")
        {
            _ssPin = std::stoi(param.second);
        }
    }

    SPI.begin(_sckPin, _misoPin, _mosiPin, _ssPin);
}

void SPICtl::deinit()
{
    SPI.end();
}

std::pair<std::string, void *> SPICtl::execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params)
{
    if (command == "beginTransaction")
    {
        uint32_t clock = SPI_CLOCK_DIV4;
        uint8_t bitOrder = MSBFIRST;
        uint8_t dataMode = SPI_MODE0;
        for (const auto &param : params)
        {
            if (param.first == "clock")
            {
                clock = std::stoul(param.second);
            }
            else if (param.first == "bitOrder")
            {
                bitOrder = std::stoi(param.second);
            }
            else if (param.first == "dataMode")
            {
                dataMode = std::stoi(param.second);
            }
        }
        beginTransaction(clock, bitOrder, dataMode);
        return {"", nullptr};
    }
    else if (command == "endTransaction")
    {
        endTransaction();
        return {"", nullptr};
    }
    else if (command == "transfer")
    {
        uint8_t data = 0;
        for (const auto &param : params)
        {
            if (param.first == "data")
            {
                data = std::stoi(param.second);
                break;
            }
        }
        uint8_t *result = new uint8_t(transfer(data));
        return {"uint8_t", static_cast<void *>(result)};
    }
    else if (command == "transferBytes")
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
            transferBytes(data, size);
            delete[] data;
        }
        return {"", nullptr};
    }
    else if (command == "setDataMode")
    {
        uint8_t dataMode = SPI_MODE0;
        for (const auto &param : params)
        {
            if (param.first == "dataMode")
            {
                dataMode = std::stoi(param.second);
                break;
            }
        }
        setDataMode(dataMode);
        return {"", nullptr};
    }
    else if (command == "setBitOrder")
    {
        uint8_t bitOrder = MSBFIRST;
        for (const auto &param : params)
        {
            if (param.first == "bitOrder")
            {
                bitOrder = std::stoi(param.second);
                break;
            }
        }
        setBitOrder(bitOrder);
        return {"", nullptr};
    }
    else if (command == "setClockDivider")
    {
        uint8_t clockDiv = SPI_CLOCK_DIV4;
        for (const auto &param : params)
        {
            if (param.first == "clockDiv")
            {
                clockDiv = std::stoi(param.second);
                break;
            }
        }
        setClockDivider(clockDiv);
        return {"", nullptr};
    }
    return {"", nullptr};
}

std::vector<FunctionInfo> SPICtl::getSupportedFunctions()
{
    return {
        {"beginTransaction", {{"clock", "uint32_t"}, {"bitOrder", "uint8_t"}, {"dataMode", "uint8_t"}}},
        {"endTransaction", {}},
        {"transfer", {{"data", "uint8_t"}}},
        {"transferBytes", {{"data", "uint8_t*"}, {"size", "size_t"}}},
        {"setDataMode", {{"dataMode", "uint8_t"}}},
        {"setBitOrder", {{"bitOrder", "uint8_t"}}},
        {"setClockDivider", {{"clockDiv", "uint8_t"}}}};
}

void SPICtl::beginTransaction(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
{
    SPI.beginTransaction(SPISettings(clock, bitOrder, dataMode));
}

void SPICtl::endTransaction()
{
    SPI.endTransaction();
}

uint8_t SPICtl::transfer(uint8_t data)
{
    return SPI.transfer(data);
}

void SPICtl::transferBytes(uint8_t *data, size_t size)
{
    SPI.transferBytes(data, data, size);
}

void SPICtl::setDataMode(uint8_t dataMode)
{
    SPI.setDataMode(dataMode);
}

void SPICtl::setBitOrder(uint8_t bitOrder)
{
    SPI.setBitOrder(bitOrder);
}

void SPICtl::setClockDivider(uint8_t clockDiv)
{
    SPI.setClockDivider(clockDiv);
}
