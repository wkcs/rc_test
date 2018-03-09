#include "board.h"

USB_OTG_CORE_HANDLE    USB_OTG_dev;
//extern vu8 bDeviceState;			//USB连接 情况

void rc11x_test_v3_board_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart1_init(1382400);	//串口初始化波特率为1382400
    uart3_init(1382400);	//串口初始化波特率为1382400
    TIM4_Init(65535,0);
    adc_init();
    dac1_init();
    W25QXX_Init();
    //xm9816_init();
    power_init();
    test_mach_init();
    test_signal_init();
    freq_init();
    /*USB_Port_Set(0); 
	delay_ms(7000);
	USB_Port_Set(1);*/
    USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);
}

