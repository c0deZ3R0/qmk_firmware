//POINTING_DEVICE_ENABLE = yes
//POINTING_DEVICE_DRIVER = custom

OPT_DEFS += -DSTM32_I2C -DHAL_USE_I2C=TRUE  -DSTM32_SPI -DHAL_USE_SPI=TRUE
QUANTUM_LIB_SRC += spi_master.c i2c_master.c