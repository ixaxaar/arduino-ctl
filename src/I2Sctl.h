#ifndef I2S_H
#define I2S_H

#include <Arduino.h>
#include <driver/i2s.h>

class I2SCtl
{
public:
    I2SCtl(i2s_port_t i2sPort = I2S_NUM_0, const i2s_config_t *i2sConfig = &defconf);
    void begin();
    void end();
    void write(const uint8_t *data, size_t size);
    void read(uint8_t *data, size_t size);
    void setBufferSize(size_t bufferSize);

private:
    i2s_port_t _i2sPort;
    i2s_config_t _i2sConfig;
    size_t _bufferSize;

    static const i2s_config_t defconf;
};

#endif // I2S_H
