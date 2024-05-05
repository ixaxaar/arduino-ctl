#include "analog.h"

AnalogCtl::AnalogCtl(int pin, int resolution, int frequency)
{
    _pin = pin;
    _resolution = resolution;
    _frequency = frequency;
}

void AnalogCtl::begin()
{
    analogReadResolution(_resolution);
    ledcSetup(0, _frequency, _resolution);
    ledcAttachPin(_pin, 0);
}

void AnalogCtl::writeAnalog(int value)
{
    ledcWrite(0, value);
}

int AnalogCtl::readAnalog()
{
    return analogRead(_pin);
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
