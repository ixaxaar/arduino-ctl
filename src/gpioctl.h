#ifndef GPIO_H
#define GPIO_H

#include <Arduino.h>

class GPIOCtl
{
public:
    GPIOCtl(int pin, int mode);
    void begin();
    void pinMode(int mode);
    void digitalWrite(int value);
    int digitalRead();
    void attachInterrupt(void (*callback)(void), int mode);
    void detachInterrupt();

private:
    int _pin;
    int _mode;
};

#endif // GPIO_H
