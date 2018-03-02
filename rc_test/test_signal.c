/*
* filename  test_signal.c
*
* author    胡启航
*
* date      2017/4/17
*
* version   v1.0.1
*/

#include "test_signal.h"
#include "test_type.h"
#include "delay.h"
#include "os_cpu.h"

static __INLINE void clk_delay(uint16_t num)
{
	uint16_t i = num;
	while(i--);
}


char test_signal_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //使能GPIOD时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //使能GPIOE时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       //下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);             //初始化
	GPIO_ResetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructure);             //初始化
	GPIO_ResetBits(GPIOB, GPIO_Pin_2 | GPIO_Pin_8);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);             //初始化
	GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_13);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);             //初始化
	GPIO_ResetBits(GPIOD, GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_14 | GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOE, &GPIO_InitStructure);             //初始化
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);

	test_data.test_mod = NORMAL_MODE;
	return 0;
}

char start_sig(void)
{
	uint32_t irq_save;
	char state = 0;
	irq_save = os_cpu_sr_save();
	test_data.test_mod = START_MODE;
	DATA = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 0;
	DATA = 0;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 0;
	DATA = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 0;
	DATA = 0;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 0;
	os_cpu_sr_restore(irq_save);
	return state;
}

char cal_sig(unsigned short int data)
{
	uint32_t irq_save;
	char state = 0, i;
	irq_save = os_cpu_sr_save();
	test_data.test_mod = CAL_MODE;
	for (i = 0; i < 10; i++)
	{
		DATA = (data >> i) & 0x0001;
		clk_delay(test_para.test_signal_para.clk_delay_num);
		CLK = 1;
		clk_delay(test_para.test_signal_para.clk_delay_num);
		CLK = 0;
	}
	DATA = 0;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 0;
	DATA = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 0;
	DATA = 0;
	os_cpu_sr_restore(irq_save);
	return state;
}

char efuse_sig(unsigned short int data)
{
	uint32_t irq_save;
	char state = 0, i;
	irq_save = os_cpu_sr_save();     /*关闭中断*/
	test_data.test_mod = EFUSE_MODE;
	for (i = 0; i < 10; i++)
	{
		DATA = (data >> i) & 0x0001;
		clk_delay(test_para.test_signal_para.clk_delay_num);
		CLK = 1;
		clk_delay(test_para.test_signal_para.clk_delay_num);
		CLK = 0;
	}
	DATA = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 0;
	DATA = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 0;
	DATA = 0;
	os_cpu_sr_restore(irq_save);
	return state;
}

char check_sig(void)
{
	uint32_t irq_save;
	char state = 0, i;
	irq_save = os_cpu_sr_save();
	test_data.test_mod = CHECK_MODE;
	for (i = 0; i < 10; i++)
	{
		DATA = 0;
		clk_delay(test_para.test_signal_para.clk_delay_num);
		CLK = 1;
		clk_delay(test_para.test_signal_para.clk_delay_num);
		CLK = 0;
	}
	DATA = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 0;
	DATA = 0;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 1;
	clk_delay(test_para.test_signal_para.clk_delay_num);
	CLK = 0;
	DATA = 0;
	os_cpu_sr_restore(irq_save);
	return state;
}

void exit_test(void)
{
	uint32_t irq_save;
	irq_save = os_cpu_sr_save();
	test_data.test_mod = NORMAL_MODE;
	K2 = 1;
	delay_us(100);
	K2 = 0;
	delay_us(100);
	os_cpu_sr_restore(irq_save);
}

char cal_sig_114s(unsigned short int data)
{
	char state = 0;
	
	T0 = 1;
	T1_1 = 1;
	D3 = 1;
	T1_15 = 0;

	D2 = (data >> 7) & 0x1;
	D1 = (data >> 6) & 0x1;
	D0 = (data >> 5) & 0x1;
	A7 = (data >> 4) & 0x1;
	A6 = (data >> 3) & 0x1;
	A5 = (data >> 2) & 0x1;
	A4 = (data >> 1) & 0x1;
	A3 = data & 0x1;

	return state;
}

char efuse_sig_114s(unsigned short int data)
{
	char state = 0;

	T0 = 0;
	T1_1 = 1;
	D3 = 1;
	T1_15 = 0;

	D2 = (data >> 7) & 0x1;
	D1 = (data >> 6) & 0x1;
	D0 = (data >> 5) & 0x1;
	A7 = (data >> 4) & 0x1;
	A6 = (data >> 3) & 0x1;
	A5 = (data >> 2) & 0x1;
	A4 = (data >> 1) & 0x1;
	A3 = data & 0x1;

	return state;
}





