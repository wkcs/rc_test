#ifndef __XM9816_H__
#define __XM9816_H__

#include "sys.h"

#define XM9816_PKT PDin(10)
#define XM9816_RSTB PDout(11)
#define XM9816_CS PBout(12)


#define XM9816_RXTX_CTRL 0x00
#define XM9816_TXFIFO 0x27
#define XM9816_RXFIFO 0x28
#define XM9816_TXFIFO_PTR 0x26
#define XM9816_RXFIFO_PTR 0x25
#define XM9816_ACK_REG 0x01
#define XM9816_ACK_TIMES 0x0b
#define XM9816_ACK_BIT 5
#define XM9816_MAIN_STATE 0x16

#define XM9816_AUTO_ACK_EN 1
#define XM9816_AUTO_ACK_DIS 0

void xm9816_init(void);
uint16_t xm9816_read_reg16(uint8_t reg);
uint8_t xm9816_write_reg16(uint8_t reg, uint16_t val);
uint8_t xm9816_read_buf(uint8_t reg, uint8_t *pBuf, uint8_t len);
uint8_t xm9816_write_buf(uint8_t reg, uint8_t *pBuf, uint8_t len);
void xm9816_set_channel(uint8_t channel);
void xm9816_set_auto_ack(uint8_t auto_ack_flag);
void xm9816_set_tx_mode(uint8_t channel);
void xm9816_set_rx_mode(uint8_t channel);
void xm9816_clear_tx_fifo(void);
void xm9816_clear_rx_fifo(void);
int8_t xm9816_tx_packet(uint8_t *tx_buf, uint8_t channel);
int8_t xm9816_rx_packet(uint8_t *rx_buf, uint8_t channel);

#endif

