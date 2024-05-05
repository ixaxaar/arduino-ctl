#include "gpioctl.h"

GPIOCtl::GPIOCtl(int pin, int mode)
{
    _pin = pin;
    _mode = mode;
}

void GPIOCtl::begin()
{
    pinMode(_mode);
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

int GPIOCtl::digitalRead()
{
    return ::digitalRead(_pin);
}

void GPIOCtl::attachInterrupt(void (*callback)(void), int mode)
{
    ::attachInterrupt(_pin, callback, mode);
}

void GPIOCtl::detachInterrupt()
{
    ::detachInterrupt(_pin);
}
