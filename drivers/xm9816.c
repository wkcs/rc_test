#include "xm9816.h"
#include "spi.h"
#include "delay.h"

void xm9816_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOB时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //使能GPIOB时钟


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;   //25MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     //下拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);             //初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOD, &GPIO_InitStructure);             //初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      //普通输入模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     //下拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);             //初始化


    spi2_init(); //初始化SPI2
    spi2_set_speed(SPI_BaudRatePrescaler_2);

    XM9816_RSTB = 0;
    delay_ms(5);
    XM9816_RSTB = 1;
    delay_ms(5);
    XM9816_CS = 1;

    xm9816_write_reg16(0x02, 0x4060);
    xm9816_write_reg16(0x03, 0x5810);
    xm9816_write_reg16(0x05, 0x7fa6);
    xm9816_write_reg16(0x0a, 0x2053);
    xm9816_write_reg16(0x0d, 0x6003);
    xm9816_write_reg16(0x0f, 0x661d);
    xm9816_write_reg16(0x1a, 0x00f7);

    xm9816_set_channel(16);
    xm9816_set_auto_ack(XM9816_AUTO_ACK_EN);
}

uint16_t xm9816_read_reg16(uint8_t reg)
{
    uint16_t reg_val;
    XM9816_CS = 0;
    spi2_read_write_byte(reg | 0x80);
    reg_val = spi2_read_write_byte(0xff);
    reg_val = reg_val << 8;
    reg_val += spi2_read_write_byte(0xff);
    XM9816_CS = 1;
    delay_us(25);
    return reg_val;
}

uint8_t xm9816_write_reg16(uint8_t reg, uint16_t val)
{
    uint8_t status;
    XM9816_CS = 0;
    status = spi2_read_write_byte(reg);
    spi2_read_write_byte(val >> 8);
    spi2_read_write_byte(val);
    XM9816_CS = 1;
    delay_us(25);
    return status;
}

uint8_t xm9816_read_buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status, i;
    XM9816_CS = 0;                     //使能SPI传输
    status = spi2_read_write_byte(reg | 0x80); //发送寄存器值(位置),并读取状态值
    for (i = 0; i < len; i++)
        pBuf[i] = spi2_read_write_byte(0XFF); //读出数据
    XM9816_CS = 1;                            //关闭SPI传输
    delay_us(5);
    return status;                               //返回读到的状态值
}

uint8_t xm9816_write_buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status, i;
    XM9816_CS = 0;                 //使能SPI传输
    status = spi2_read_write_byte(reg); //发送寄存器值(位置),并读取状态值
    for (i = 0; i < len; i++)
        spi2_read_write_byte(*pBuf++); //写入数据
    XM9816_CS = 1;                //关闭SPI传输
    delay_us(5);
    return status;                   //返回读到的状态值
}

void xm9816_set_channel(uint8_t channel)
{
    uint16_t reg_val;
    reg_val = xm9816_read_reg16(XM9816_RXTX_CTRL);
    reg_val &= 0xff00;
    reg_val |= (uint16_t)channel;
    xm9816_write_reg16(XM9816_RXTX_CTRL, reg_val);
}

void xm9816_set_auto_ack(uint8_t auto_ack_flag)
{
    uint16_t reg_val;
    reg_val = xm9816_read_reg16(XM9816_ACK_REG);
    if (auto_ack_flag)
        reg_val |= 0x08;
    else
        reg_val &= 0xfff7;
    xm9816_write_reg16(XM9816_ACK_REG, reg_val);
}

void xm9816_set_tx_mode(uint8_t channel)
{
    uint16_t reg_val;
    reg_val = xm9816_read_reg16(XM9816_RXTX_CTRL);
    reg_val &= 0x3f00;
    reg_val |= (0x8000 | (uint16_t)channel);
    xm9816_write_reg16(XM9816_RXTX_CTRL, reg_val);
}

void xm9816_set_rx_mode(uint8_t channel)
{
    uint16_t reg_val;
    reg_val = xm9816_read_reg16(XM9816_RXTX_CTRL);
    reg_val &= 0x3f00;
    reg_val |= (0x4000 | (uint16_t)channel);
    xm9816_write_reg16(XM9816_RXTX_CTRL, reg_val);
}

void xm9816_clear_tx_fifo(void)
{
    uint16_t reg_val;
    reg_val = xm9816_read_reg16(XM9816_TXFIFO_PTR);
    reg_val |= 0x8080;
    xm9816_write_reg16(XM9816_TXFIFO_PTR, reg_val);
}

void xm9816_clear_rx_fifo(void)
{
    uint16_t reg_val;
    reg_val = xm9816_read_reg16(XM9816_RXFIFO_PTR);
    reg_val |= 0x8080;
    xm9816_write_reg16(XM9816_RXFIFO_PTR, reg_val);
}

int8_t xm9816_tx_packet(uint8_t *tx_buf, uint8_t channel) 
{
    xm9816_clear_tx_fifo();
    delay_us(50);
    xm9816_write_buf(XM9816_TXFIFO, tx_buf, tx_buf[0] + 1);
    delay_us(50);
    xm9816_set_tx_mode(channel);
    while(!XM9816_PKT);
    return 0;
}

int8_t xm9816_rx_packet(uint8_t *rx_buf, uint8_t channel) 
{
    xm9816_clear_rx_fifo();
    delay_us(50);
    xm9816_set_rx_mode(channel);
    delay_us(50);
    while(!XM9816_PKT);
    if ((xm9816_read_reg16(XM9816_MAIN_STATE) & 0x8040) == 0x0040)
	{
		xm9816_read_buf(XM9816_RXFIFO, rx_buf, 1);
        xm9816_read_buf(XM9816_RXFIFO, (rx_buf + 1), rx_buf[0]);
        return 0;
	} else 
        return -1;
}

