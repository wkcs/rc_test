#include "test_com.h"
#include "board.h"

enum {
    TX_MODE = 0,
    RX_MODE
}TEST_COM_MODE;

static struct{
    uint8_t mode;
}test_com_data;

void send_data(uint8_t *buf, uint8_t len)
{
    uint8_t tx_len = len, i;
    uint8_t tx_buf[64];
    test_com_data.mode = TX_MODE;
    while (tx_len > 0)
    {
        if (tx_len > 63)
        {
            tx_buf[0] = 63;
            for (i = 1; i < 64; i++)
            {
                tx_buf[i] = buf[i - 1];
            }
            buf += 63;
            tx_len -= 63;
            xm9816_tx_packet(tx_buf, 16);
        }
        else
        {
            tx_buf[0] = tx_len;
            for (i = 0; i < tx_len; i++)
            {
                tx_buf[i + 1] = buf[i];
            }
            tx_len = 0;
            xm9816_tx_packet(tx_buf, 16);
        }
    }
}

void get_data(void)
{
    uint8_t rx_buf[64];
    if(test_com_data.mode != RX_MODE)
    {
        xm9816_clear_rx_fifo();
        xm9816_set_rx_mode(16);
        test_com_data.mode = RX_MODE;
    }

    if (XM9816_PKT)
    {
        if ((xm9816_read_reg16(XM9816_MAIN_STATE) & 0x8040) == 0x0040)
        {
            xm9816_read_buf(XM9816_RXFIFO, rx_buf, 1);
            xm9816_read_buf(XM9816_RXFIFO, (rx_buf + 1), rx_buf[0]);
        }
    }
    
}