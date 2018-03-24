#include "rc_message.h"
#include "usart.h"
#include "board.h"

void rc_send_message(uint8_t *buf, uint8_t len, uint8_t mes_type)
{
    uint8_t tx_buf[6];
    tx_buf[0] = 2;
    tx_buf[1] = 0;
	tx_buf[2] = 3;
    tx_buf[3] = 0;
    tx_buf[4] = mes_type;
    tx_buf[5] = len;
    #if (MES_DEVICE == UART1)
        send_data_to_uart1(tx_buf, 6);
    #elif (MES_DEVICE == USB)
        send_data_to_uart_usb(tx_buf, 6);
    #endif
    //delay_us(10);
    if (len > 0 && buf != 0) {
    #if (MES_DEVICE == UART1)
        send_data_to_uart1(buf, len);
    #elif (MES_DEVICE == USB)
        send_data_to_uart_usb(buf, len);
    #endif
    }
    //delay_us(10);
}

uint8_t rc_get_message(uint8_t **buf, uint8_t *len, uint8_t *mes_type)
{
#if (MES_DEVICE == UART1)
    if (uart1_data.rx_status == 1) {
        *buf = uart1_data.rx_buf + 2;
        *len = uart1_data.rx_buf[1];
        *mes_type = uart1_data.rx_buf[0];
        return 1;
    } else
        return 0;
#elif (MES_DEVICE == USB)
	if (uart_usb_data.rx_status == 1) {
        *buf = uart_usb_data.rx_buf + 2;
        *len = uart_usb_data.rx_buf[1];
        *mes_type = uart_usb_data.rx_buf[0];
        return 1;
    } else
        return 0;
#endif
}

void rc_clear_message(void)
{
#if (MES_DEVICE == UART1)
    uart1_data.rx_status = 0;
	uart1_data.rx_len = 0;
#elif (MES_DEVICE == USB)
	uart_usb_data.rx_status = 0;
	uart_usb_data.rx_len = 0;
#endif
    rc_send_message(0, 0, CLEAN_MES);
}

void rc_debug_mes_save_enable(void)
{
    uint8_t mes = 1;
    rc_send_message(&mes, 1, DEBUG_MES_SAVE_MES);
}

void rc_debug_mes_save_disable(void)
{
    uint8_t mes = 0;
    rc_send_message(&mes, 1, DEBUG_MES_SAVE_MES);
}
