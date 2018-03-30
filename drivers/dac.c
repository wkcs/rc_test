#include "dac.h"
#include "test_type.h"
#include "delay.h"

void dac1_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
	DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC通道1
    /*此处先设置rc_power为默认电压*/
	rc_power_con_dac(3300);
}

/*
 * 设置rc_power_con的电压值，以控制rc_power电压
 *
 */
void rc_power_con_dac(uint16_t value)
{
	uint16_t dac_val;
	if (test_data.power_data.rc_power_voltage != value) {
		/*这里要注意，rc_power电压设置不能超过3.7V，如果此电压过高会烧坏顶板上的MOS管*/
		if (value > 3700)
			value = 3700;
		//dac_val = (uint16_t)((double)(6710 - value) / 3.57); //V(mV) = 6710 - 3.57 * dac_val
		//dac_val = (uint16_t)((double)(6940 - value) / 3.7); //V(mV) = 6940 - 3.7 * dac_val
		dac_val = (uint16_t)((double)(7060 - value) / 3.11); 
    	DAC_SetChannel1Data(DAC_Align_12b_R, dac_val);
		test_data.power_data.rc_power_voltage = value;
		delay_ms(1);
	}
}
