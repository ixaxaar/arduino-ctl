#ifndef ANALOG_H
#define ANALOG_H

#include <Arduino.h>

class AnalogCtl
{
public:
    AnalogCtl(int pin, int resolution = 12, int frequency = 5000);
    void begin();
    void writeAnalog(int value);
    int readAnalog();
    void setResolution(int resolution);
    void setFrequency(int frequency);

private:
    int _pin;
    int _resolution;
    int _frequency;
};

#endif // ANALOG_H
