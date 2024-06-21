// spi.cpp

#include "SPIctl.h"
#include <sstream>

SPICtl::SPICtl() : _sckPin(SCK), _misoPin(MISO), _mosiPin(MOSI), _ssPin(SS), _spiSettings(4000000, MSBFIRST, SPI_MODE0) {}

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
    if (command == "transfer")
    {
        std::vector<uint8_t> data;
        for (const auto &param : params)
        {
            if (param.first == "data")
            {
                std::istringstream iss(param.second);
                std::string token;
                while (std::getline(iss, token, ','))
                {
                    data.push_back(std::stoul(token));
                }
                break;
            }
        }
        std::vector<uint8_t> result = transfer(data);
        return {"std::vector<uint8_t>", new std::vector<uint8_t>(result)};
    }
    else if (command == "setSettings")
    {
        uint32_t clock = 4000000;
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
                bitOrder = std::stoul(param.second);
            }
            else if (param.first == "dataMode")
            {
                dataMode = std::stoul(param.second);
            }
        }
        setSettings(clock, bitOrder, dataMode);
        return {"", nullptr};
    }
    return {"", nullptr};
}

std::vector<FunctionInfo> SPICtl::getSupportedFunctions()
{
    return {
        {"transfer", {{"data", "std::vector<uint8_t>"}}},
        {"setSettings", {{"clock", "uint32_t"}, {"bitOrder", "uint8_t"}, {"dataMode", "uint8_t"}}}};
}

std::vector<uint8_t> SPICtl::transfer(const std::vector<uint8_t> &data)
{
    std::vector<uint8_t> received(data.size());
    beginTransaction();
    for (size_t i = 0; i < data.size(); ++i)
    {
        received[i] = SPI.transfer(data[i]);
    }
    endTransaction();
    return received;
}

void SPICtl::beginTransaction()
{
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_ssPin, LOW);
}

void SPICtl::endTransaction()
{
    digitalWrite(_ssPin, HIGH);
    SPI.endTransaction();
}

void SPICtl::setSettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
{
    _spiSettings = SPISettings(clock, bitOrder, dataMode);
}