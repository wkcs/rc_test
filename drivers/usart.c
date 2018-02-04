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
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ��USART1ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10����ΪUSART1

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);										//��ʼ������1

	USART_Cmd(USART1, ENABLE); //ʹ�ܴ���1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

void uart3_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ��USART3ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //GPIOD8��GPIOD9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   //����
	GPIO_Init(GPIOD, &GPIO_InitStructure);				   //��ʼ��PA9��PA10

	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);										//��ʼ������3

	USART_Cmd(USART3, ENABLE); //ʹ�ܴ���1

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		  //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

void USART1_IRQHandler(void) //����1�жϷ������
{
	u8 Res;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART1);
		if (uart1_data.rx_status == 0)
		{
			if (uart1_data.rx_len < 2) //����Ƿ�������ͷ
			{
				uart1_data.rx_buf[uart1_data.rx_len] = Res;
				uart1_data.rx_len++;
				if ((uart1_data.rx_len == 2) && (uart1_data.rx_buf[1] == 0))
				{ //���ݰ�����Ϊ0ʱ�ж��Ƿ�������
					uart1_data.rx_status = 1;
				}
			}
			else if (uart1_data.rx_len < (uint32_t)(uart1_data.rx_buf[1] + 2)) //�ж��Ƿ������һ֡����
			{
				uart1_data.rx_buf[uart1_data.rx_len] = Res;
				uart1_data.rx_len++;
				if (uart1_data.rx_len > 256)
					uart1_data.rx_status = 0; //�������ݴ���,���¿�ʼ����
				if (uart1_data.rx_len == (uint32_t)(uart1_data.rx_buf[1] + 2))
				{
					uart1_data.rx_status = 1;
				}
			}
		}
	}
}

void USART3_IRQHandler(void) //����3�жϷ������
{
	u8 Res;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART3); //��ȡ���յ�������
		uart3_data.rx_buf[uart3_data.rx_len] = Res;
		uart3_data.rx_len++;
		uart3_data.rx_status = 1;
			/*if (uart3_data.rx_status == 1) //���յ���0x0d
			{
				if (Res != 0x0a)
				{
					uart3_data.rx_status = 0; //���մ���,���¿�ʼ
					uart3_data.rx_len = 0;
				}
				else
				{
					uart3_data.rx_status = 2; //���������
					uart3_data.rx_len++;
				}
			}
			else //��û�յ�0X0D
			{
				if (Res == 0x0d)
					uart3_data.rx_status = 1;
				else
				{
					uart3_data.rx_buf[uart3_data.rx_len] = Res;
					uart3_data.rx_len++;
					if (uart3_data.rx_len > 256)
						uart3_data.rx_status = 0; //�������ݴ���,���¿�ʼ����
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
