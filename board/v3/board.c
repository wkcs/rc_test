#include "board.h"

void rc11x_test_v3_board_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart1_init(921600);	//串口初始化波特率为115200
    uart3_init(1382400);	//串口初始化波特率为115200
    TIM2_CH3_Cap_Init(65535,83);
    adc_init();
    W25QXX_Init();
    xm9816_init();
    power_init();
    test_mach_init();
    test_signal_init();
}

