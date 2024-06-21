

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

    std::vector<int> readAnalog(int numSamples);
    void writeAnalog(const std::vector<int> &values);
};

#endif // ANALOG_H
