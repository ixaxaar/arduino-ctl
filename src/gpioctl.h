

#ifndef GPIO_H
#define GPIO_H

#include <Arduino.h>
#include <vector>
#include <string>
#include "module.h"

class GPIOCtl : public ModuleInterface
{
public:
    GPIOCtl();
    void init(const std::vector<std::pair<std::string, std::string>> &params) override;
    void deinit() override;
    std::pair<std::string, void *> execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params) override;
    std::vector<FunctionInfo> getSupportedFunctions() override;

private:
    int _pin;
    int _mode;

    void setPinMode(int mode);
    std::vector<int> digitalRead(int numSamples);
    void digitalWrite(const std::vector<int> &values);
};

#endif // GPIO_H
