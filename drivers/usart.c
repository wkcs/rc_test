#include "board.h"
#include "usart.h"

uart_data_t uart1_data, uart3_data;


int _write(int fd, char *ptr, int len);
int _read(int fd, char *ptr, int len);
void get_buffered_line(void);

/*
 * This is a pretty classic ring buffer for characters
 */
#define BUFLEN 127

static uint16_t start_ndx;
static uint16_t end_ndx;
static char buf[BUFLEN + 1];
#define buf_len ((end_ndx - start_ndx) % BUFLEN)
static inline int inc_ndx(int n) { return ((n + 1) % BUFLEN); }
static inline int dec_ndx(int n) { return (((n + BUFLEN) - 1) % BUFLEN); }

static inline void usart_send_blocking(USART_TypeDef * usart, uint8_t data)
{
	while (USART_GetFlagStatus(usart, USART_FLAG_TC) != SET);	
	USART_SendData(usart, data);
}

static inline char usart_recv_blocking(uart_data_t *uart_data)
{
	uint32_t i;
	char buf;
	while(uart_data -> rx_status == 0);
	buf = uart_data->rx_buf[0];
	uart_data->rx_len--;
	if (uart_data->rx_len > 0)
	{
		for (i = 0; i < uart_data->rx_len; i++)
			uart_data->rx_buf[i] = uart_data->rx_buf[i + 1];
	}
	else
		uart_data->rx_status = 0;
			
	return buf;
}

/* back up the cursor one space */
static inline void back_up(void)
{
    end_ndx = dec_ndx(end_ndx);
    usart_send_blocking(USART3, '\010');
    usart_send_blocking(USART3, ' ');
    usart_send_blocking(USART3, '\010');
}

/*
 * A buffered line editing function.
 */
void get_buffered_line(void)
{
    char c;

    if (start_ndx != end_ndx)
    {
        return;
    }

    while (1)
    {
        c = usart_recv_blocking(&uart3_data);

        if (c == '\r')
        {
            buf[end_ndx] = '\n';
            end_ndx = inc_ndx(end_ndx);
            buf[end_ndx] = '\0';
            usart_send_blocking(USART3, '\r');
            usart_send_blocking(USART3, '\n');
            return;
        }

        /* or DEL erase a character */
        if ((c == '\010') || (c == '\177'))
        {
            if (buf_len == 0)
            {
                usart_send_blocking(USART3, '\a');
            }

            else
            {
                back_up();
            }

            /* erases a word */
        }

        else if (c == 0x17)
        {
            while ((buf_len > 0) &&
                    (!(isspace((int) buf[end_ndx]))))
            {
                back_up();
            }

            /* erases the line */
        }

        else if (c == 0x15)
        {
            while (buf_len > 0)
            {
                back_up();
            }

            /* Non-editing character so insert it */
        }

        else
        {
            if (buf_len == (BUFLEN - 1))
            {
                usart_send_blocking(USART3, '\a');
            }

            else
            {
                buf[end_ndx] = c;
                end_ndx = inc_ndx(end_ndx);
                usart_send_blocking(USART3, c);
            }
        }
    }
}

/*
 * Called by libc stdio fwrite functions
 */
int _write(int fd, char *ptr, int len)
{
    int i = 0;

    /*
     * write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
    * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2)
    {
        return -1;
    }

    while (*ptr && (i < len))
    {
        usart_send_blocking(USART3, *ptr);

        if (*ptr == '\n')
        {
            usart_send_blocking(USART3, '\r');
        }

        i++;
        ptr++;
    }

    return i;
}

/*
 * Called by the libc stdio fread fucntions
 *
 * Implements a buffered read with line editing.
 */
int _read(int fd, char *ptr, int len)
{
    int my_len;

    if (fd > 2)
    {
        return -1;
    }

    get_buffered_line();
    my_len = 0;

    while ((buf_len > 0) && (len > 0))
    {
        *ptr++ = buf[start_ndx];
        start_ndx = inc_ndx(start_ndx);
        my_len++;
        len--;
    }

    return my_len; /* return the length we got */
}

void uart1_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10复用为USART1

	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART1, &USART_InitStructure);										//初始化串口1

	USART_Cmd(USART1, ENABLE); //使能串口1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化VIC寄存器、
}

void uart3_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART3时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //GPIOD8与GPIOD9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   //上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);				   //初始化PA9，PA10

	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART3, &USART_InitStructure);										//初始化串口3

	USART_Cmd(USART3, ENABLE); //使能串口1

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		  //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化VIC寄存器、
}

void USART1_IRQHandler(void) //串口1中断服务程序
{
	u8 Res;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART1);
		if (uart1_data.rx_status == 0)
		{
			if (uart1_data.rx_len < 2) //检测是否接收完包头
			{
				uart1_data.rx_buf[uart1_data.rx_len] = Res;
				uart1_data.rx_len++;
				if ((uart1_data.rx_len == 2) && (uart1_data.rx_buf[1] == 0))
				{ //数据包长度为0时判断是否接收完成
					uart1_data.rx_status = 1;
				}
			}
			else if (uart1_data.rx_len < (uint32_t)(uart1_data.rx_buf[1] + 2)) //判断是否接收完一帧数据
			{
				uart1_data.rx_buf[uart1_data.rx_len] = Res;
				uart1_data.rx_len++;
				if (uart1_data.rx_len > 256)
					uart1_data.rx_status = 0; //接收数据错误,重新开始接收
				if (uart1_data.rx_len == (uint32_t)(uart1_data.rx_buf[1] + 2))
				{
					uart1_data.rx_status = 1;
				}
			}
		}
	}
}

void USART3_IRQHandler(void) //串口3中断服务程序
{
	u8 Res;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART3); //读取接收到的数据
		uart3_data.rx_buf[uart3_data.rx_len] = Res;
		uart3_data.rx_len++;
		uart3_data.rx_status = 1;
			/*if (uart3_data.rx_status == 1) //接收到了0x0d
			{
				if (Res != 0x0a)
				{
					uart3_data.rx_status = 0; //接收错误,重新开始
					uart3_data.rx_len = 0;
				}
				else
				{
					uart3_data.rx_status = 2; //接收完成了
					uart3_data.rx_len++;
				}
			}
			else //还没收到0X0D
			{
				if (Res == 0x0d)
					uart3_data.rx_status = 1;
				else
				{
					uart3_data.rx_buf[uart3_data.rx_len] = Res;
					uart3_data.rx_len++;
					if (uart3_data.rx_len > 256)
						uart3_data.rx_status = 0; //接收数据错误,重新开始接收
				}
			}*/
		
		/* if(Res == '\r')
		{
			while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);	
			USART_SendData(USART3, '\r');
			while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);	
			USART_SendData(USART3, '\n');
		} 
		else
		{
			while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);	
			USART_SendData(USART3, Res);
		} */
	}
}

void send_data_to_uart1(uint8_t *buf, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);	
		USART_SendData(USART1, buf[i]);
	}
}
