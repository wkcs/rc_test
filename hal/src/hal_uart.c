#include "stm32f4xx.h"
#include "hal_uart.h"


/* UART GPIO define. */
#define UART1_GPIO_TX       GPIO_Pin_9
#define UART1_TX_PIN_SOURCE GPIO_PinSource9
#define UART1_GPIO_RX       GPIO_Pin_10
#define UART1_RX_PIN_SOURCE GPIO_PinSource10
#define UART1_GPIO          GPIOA
#define UART1_GPIO_RCC      RCC_AHB1Periph_GPIOA
#define RCC_APBPeriph_UART1 RCC_APB2Periph_USART1

#define UART3_GPIO_TX       GPIO_Pin_8
#define UART3_TX_PIN_SOURCE GPIO_PinSource8
#define UART3_GPIO_RX       GPIO_Pin_9
#define UART3_RX_PIN_SOURCE GPIO_PinSource9
#define UART3_GPIO          GPIOD
#define UART3_GPIO_RCC      RCC_AHB1Periph_GPIOD
#define RCC_APBPeriph_UART3 RCC_APB1Periph_USART3


typedef __uart_type {
	USART_TypeDef *uart_device;
	IRQn_Type irq;
	struct stm32_uart_dma
	{
		/* dma stream */
		DMA_Stream_TypeDef *rx_stream;
		/* dma channel */
		uint32_t rx_ch;
		/* dma flag */
		uint32_t rx_flag;
		/* dma irq channel */
		uint8_t rx_irq_ch;
		/* setting receive len */
		uint32_t setting_recv_len;
		/* last receive index */
		uint32_t last_recv_index;
	} dma;
} uart_type_t;

int32_t hal_uart_init(uart_type_t *uart)
{
	switch (uart -> uart_device) {
	case USART1:
		RCC_AHB1PeriphClockCmd(UART1_GPIO_RCC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APBPeriph_UART1,ENABLE);
		GPIO_PinAFConfig(UART1_GPIO,UART1_TX_PIN_SOURCE,GPIO_AF_USART1);
		GPIO_PinAFConfig(UART1_GPIO,UART1_RX_PIN_SOURCE,GPIO_AF_USART1);
		break;
	case USART3:
		RCC_AHB1PeriphClockCmd(UART3_GPIO_RCC,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APBPeriph_UART3,ENABLE);
		GPIO_PinAFConfig(UART3_GPIO,UART3_TX_PIN_SOURCE,GPIO_AF_USART3);
		GPIO_PinAFConfig(UART3_GPIO,UART3_RX_PIN_SOURCE,GPIO_AF_USART3);
		break;
	default:
		return -1;
	}
}