#include "timer.h"
#include "test_type.h"

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM3_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); ///使能TIM3时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;					//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //初始化TIM3

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //允许定时器3更新中断
	//TIM_Cmd(TIM3, ENABLE);					   //使能定时器3

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			  //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
	{
		/*if (send_lock)
		{
			time_num++;
			if (time_num == clk_f_1527)
			{
				send_1527();
				time_num = 0;
			}
		}
		else
			time_num = 0;*/
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void TIM4_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM4_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //TIM4时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能PORTB时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	 //下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);			   //初始化PB7

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4); //PB7复用位定时器4

	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//初始化TIM4输入捕获参数
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2;				 
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 //上升沿捕获
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI2上
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 //配置输入分频,不分频
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;						 //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //允许更新中断 ,允许CC1IE捕获中断

	TIM_Cmd(TIM4, ENABLE); //使能定时器4

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	

	//TIM_DMACmd(TIM4,TIM_DMA_Update, ENABLE);              
    TIM_DMACmd(TIM4,TIM_DMA_CC2, ENABLE);	
	
	TIM_Cmd(TIM4, DISABLE);					  //根据指定的参数初始化VIC寄存器、
}

//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM11, TIM_IT_Update) != RESET)
		test_data.freq_data.timer_updata_num++;
	TIM_ClearITPendingBit(TIM11, TIM_IT_Update);
}

//通用定时器5中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器5!(定时器5挂在APB1上，时钟为HCLK/2)
void TIM5_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); ///使能TIM5时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;					//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure); //初始化TIM5

	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //允许定时器3更新中断
	//TIM_Cmd(TIM5, ENABLE);					   //使能定时器3

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;			  //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//定时器5中断服务函数
void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET) //溢出中断
	{
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}

