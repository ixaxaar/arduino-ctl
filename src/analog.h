#ifndef ANALOG_H
#define ANALOG_H

#include <Arduino.h>
#include <vector>
#include <string>
#include "module.h"

class AnalogCtl : public ModuleInterface
{
public:
    AnalogCtl();
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;
    void deinit() override;
    std::pair<std::string, void *> execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params) override;
    std::vector<FunctionInfo> getSupportedFunctions() override;

private:
    int _pin;
    int _resolution;
    int _frequency;

    void writeAnalog(int value);
    int *readAnalog();
    void setResolution(int resolution);
    void setFrequency(int frequency);
};

std::vector<FunctionInfo> AnalogCtl::getSupportedFunctions()
{
    return {
        {"writeAnalog", {{"value", "int"}}},
        {"readAnalog", {}},
        {"setResolution", {{"resolution", "int"}}},
        {"setFrequency", {{"frequency", "int"}}}};
}

#endif // ANALOG_H
