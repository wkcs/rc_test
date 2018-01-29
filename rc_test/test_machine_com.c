#include "test_machine_com.h"
#include "test_type.h"
#include "delay.h"


void test_mach_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2


    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接到中断线2
    
    /* 配置EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
    EXTI_Init(&EXTI_InitStructure);//配置

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOE,&GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_SetBits(GPIOE, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
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
            BIN1 = 0;
            EOT = 0;
            delay_ms(2);
            EOT = 1;
            BIN1 = 1;
            break;
        case 2:
            BIN2 = 0;
            EOT = 0;
            delay_ms(2);
            EOT = 1;
            BIN2 = 1;
            break;
        case 3:
            BIN3 = 0;
            EOT = 0;
            delay_ms(2);
            EOT = 1;
            BIN3 = 1;
            break;
        case 4:
            BIN4 = 0;
            EOT = 0;
            delay_ms(2);
            EOT = 1;
            BIN4 = 1;
            break;
        case 5:
            BIN5 = 0;
            EOT = 0;
            delay_ms(2);
            EOT = 1;
            BIN5 = 1;
            break;
        case 6:
            BIN6 = 0;
            EOT = 0;
            delay_ms(2);
            EOT = 1;
            BIN6 = 1;
            break;
        case 7:
            BIN7 = 0;
            EOT = 0;
            delay_ms(2);
            EOT = 1;
            BIN7 = 1;
            break;
        case 8:
            BIN8 = 0;
            EOT = 0;
            delay_ms(2);
            EOT = 1;
            BIN8 = 1;
            break;
        test_data.test_machine_data.test_start = DISABLE;
    }
}  

void EXTI2_IRQHandler(void)
{
	test_data.test_machine_data.test_start = ENABLE;	 
	EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE2上的中断标志位 
}

