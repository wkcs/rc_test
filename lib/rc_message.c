#include "rc_message.h"
#include "usart.h"

void rc_send_message(uint8_t *buf, uint8_t len, uint8_t mes_type)
{
    uint8_t tx_buf[6];
    tx_buf[0] = 2;
    tx_buf[1] = 0;
	tx_buf[2] = 3;
    tx_buf[3] = 0;
    tx_buf[4] = mes_type;
    tx_buf[5] = len;
    send_data_to_uart1(tx_buf, 6);
    //delay_us(10);
    if (len > 0 && buf != 0)
        send_data_to_uart1(buf, len);
    //delay_us(10);
}
