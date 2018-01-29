#ifndef __SPI_H__
#define __SPI_H__

#include "sys.h"

void spi3_init(void);
void spi3_set_speed(uint8_t SPI_BaudRatePrescaler);
uint8_t spi3_read_write_byte(uint8_t tx_data);
void spi2_init(void);
void spi2_set_speed(uint8_t SPI_BaudRatePrescaler);
uint8_t spi2_read_write_byte(uint8_t tx_data);

#endif
