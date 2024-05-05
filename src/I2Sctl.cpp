#include "I2Sctl.h"

const i2s_config_t I2SCtl::defconf = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false};

I2SCtl::I2SCtl(i2s_port_t i2sPort, const i2s_config_t *i2sConfig)
{
    _i2sPort = i2sPort;
    _i2sConfig = i2sConfig ? *i2sConfig : defconf;
    _bufferSize = _i2sConfig.dma_buf_len;
}

void I2SCtl::begin()
{
    i2s_driver_install(_i2sPort, &_i2sConfig, 0, NULL);
}

void I2SCtl::end()
{
    i2s_driver_uninstall(_i2sPort);
}

void I2SCtl::write(const uint8_t *data, size_t size)
{
    size_t bytesWritten;
    i2s_write(_i2sPort, data, size, &bytesWritten, portMAX_DELAY);
}

void I2SCtl::read(uint8_t *data, size_t size)
{
    size_t bytesRead;
    i2s_read(_i2sPort, data, size, &bytesRead, portMAX_DELAY);
}

void I2SCtl::setBufferSize(size_t bufferSize)
{
    _bufferSize = bufferSize;
}
