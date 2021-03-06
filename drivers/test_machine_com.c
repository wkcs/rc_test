#include "test_machine_com.h"
#include "test_type.h"
#include "delay.h"


void test_mach_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD13


    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource13);//PD13 连接到中断线13
    
    /* 配置EXTI_Line13 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
    EXTI_Init(&EXTI_InitStructure);//配置

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断15-10
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
    GPIO_Init(GPIOD,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_ResetBits(GPIOD, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

/*
 * Wait for the test machine to start the signal.
 */
void test_mach_wait_start_sig(void)
{

}

/*
 * Send the test end signal, which includes the bin signal.
 */
void test_mach_send_end_sig(uint8_t bin)      
{
    switch(bin)
    {
        case 1:
            BIN1 = 1;
            delay_ms(5);
            EOT = 1;
            delay_ms(10);
            EOT = 0;
            delay_ms(5);
            BIN1 = 0;
            break;
        case 2:
            BIN2 = 1;
            delay_ms(5);
            EOT = 1;
            delay_ms(10);
            EOT = 0;
            delay_ms(5);
            BIN2 = 0;
            break;
        case 3:
            BIN3 = 1;
            delay_ms(5);
            EOT = 1;
            delay_ms(10);
            EOT = 0;
            delay_ms(5);
            BIN3 = 0;
            break;
        case 4:
            BIN4 = 1;
            delay_ms(5);
            EOT = 1;
            delay_ms(10);
            EOT = 0;
            delay_ms(5);
            BIN4 = 0;
            break;
        case 5:
            BIN5 = 1;
            delay_ms(5);
            EOT = 1;
            delay_ms(10);
            EOT = 0;
            delay_ms(5);
            BIN5 = 0;
            break;
        case 6:
            BIN6 = 1;
            delay_ms(5);
            EOT = 1;
            delay_ms(10);
            EOT = 0;
            delay_ms(5);
            BIN6 = 0;
            break;
        case 7:
            BIN7 = 1;
            delay_ms(5);
            EOT = 1;
            delay_ms(10);
            EOT = 0;
            delay_ms(5);
            BIN7 = 0;
            break;
        case 8:
            BIN8 = 1;
            delay_ms(5);
            EOT = 1;
            delay_ms(10);
            EOT = 0;
            delay_ms(5);
            BIN8 = 0;
            break;
        test_data.test_machine_data.test_start = DISABLE;
    }
}  

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
    {
	    test_data.test_machine_data.test_start = ENABLE;	 
	    EXTI_ClearITPendingBit(EXTI_Line13);//清除LINE13上的中断标志位 
    }
}

