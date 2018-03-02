#include "adc.h"
#include "delay.h"
#include "dma.h"

static uint16_t adc1_buf[36];   /*存放ADC1规则组转换结果，用于OS测试*/
static uint16_t adc2_buf[1000];   /*存放ADC2规则组转换结果，用于电流测试*/

void adc1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);           //初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不带上下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不带上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE); //复位结束

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;             //DMA模式一
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);                                    //初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                      //12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                //扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                          //连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 12;                                 //12个转换在规则序列中
	ADC_Init(ADC1, &ADC_InitStructure);                                         //ADC初始化

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 6, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 7, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 8, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 9, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 10, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 11, ADC_SampleTime_84Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 12, ADC_SampleTime_84Cycles );

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE); //开启AD转换器

	/*配置DMA*/
	DMA_config(DMA2_Stream0, DMA_Channel_0, &ADC1->DR, adc1_buf, 36);
}

void adc2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不带上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);           //初始化

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);  //ADC2复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE); //复位结束

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;             //DMA模式一
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);                                    //初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                      //12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                //扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                          //连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 12;                                 //12个转换在规则序列中
	ADC_Init(ADC2, &ADC_InitStructure);                                         //ADC初始化

	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
	ADC_DMACmd(ADC2, ENABLE);
	ADC_Cmd(ADC2, ENABLE); //开启AD转换器

	/*配置DMA*/
	DMA_config(DMA2_Stream2, DMA_Channel_1, &ADC2->DR, adc2_buf, 1000);
}


uint16_t *get_os_adc(void)
{
	uint32_t i = 0;
	ADC_Cmd(ADC1, DISABLE);         //先失能ADC，这样保证从第一个规则组开始转换
	DMA2_Stream0->NDTR = 36;
    DMA_Cmd(DMA2_Stream0, ENABLE);
	DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
	ADC_ClearFlag(ADC1, ADC_FLAG_OVR);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
	while(DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0) == RESET && i < 10000)//等待DMA2_Steam0传输完成
		i++;
	if (i >= 10000) {
		ADC_Cmd(ADC1, DISABLE);
		ADC_Cmd(ADC1, ENABLE);
		DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
		DMA_Cmd(DMA2_Stream0, DISABLE);
		return 0;
	}
	DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
	DMA_Cmd(DMA2_Stream0, DISABLE);
	return adc1_buf;
}

uint16_t *get_work_current_adc(uint16_t num)
{
	uint32_t i = 0;
	ADC_Cmd(ADC2, DISABLE);         //先失能ADC，这样保证从第一个规则组开始转换
	DMA2_Stream2->NDTR = num;
    DMA_Cmd(DMA2_Stream2, ENABLE);
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
	ADC_ClearFlag(ADC2, ADC_FLAG_OVR);
	ADC_DMACmd(ADC2, ENABLE);
	ADC_Cmd(ADC2, ENABLE);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_84Cycles );
	ADC_SoftwareStartConv(ADC2);
	while(DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TCIF2) == RESET && i < 10000)//等待DMA传输完成
		i++;
	if (i >= 10000) {
		ADC_Cmd(ADC2, DISABLE);
		ADC_Cmd(ADC2, ENABLE);
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_Cmd(DMA2_Stream2, DISABLE);
		return 0;
	}
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
	DMA_Cmd(DMA2_Stream2, DISABLE);
	return adc2_buf;
}

uint16_t *get_standby_current_adc(uint16_t num)
{
	uint32_t i = 0;
	ADC_Cmd(ADC2, DISABLE);         //先失能ADC，这样保证从第一个规则组开始转换
	DMA2_Stream2->NDTR = num;
    DMA_Cmd(DMA2_Stream2, ENABLE);
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
	ADC_ClearFlag(ADC2, ADC_FLAG_OVR);
	ADC_DMACmd(ADC2, ENABLE);
	ADC_Cmd(ADC2, ENABLE);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_11, 1, ADC_SampleTime_84Cycles );
	ADC_SoftwareStartConv(ADC2);
	while(DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TCIF2) == RESET && i < 10000)//等待DMA传输完成
		i++;
	if (i >= 10000) {
		ADC_Cmd(ADC2, DISABLE);
		ADC_Cmd(ADC2, ENABLE);
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_Cmd(DMA2_Stream2, DISABLE);
		return 0;
	}
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
	DMA_Cmd(DMA2_Stream2, DISABLE);
	return adc2_buf;
}

uint16_t *get_rc_power_adc(uint16_t num)
{
	uint32_t i = 0;
	ADC_Cmd(ADC2, DISABLE);         //先失能ADC，这样保证从第一个规则组开始转换
	DMA2_Stream2->NDTR = num;
    DMA_Cmd(DMA2_Stream2, ENABLE);
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
	ADC_ClearFlag(ADC2, ADC_FLAG_OVR);
	ADC_DMACmd(ADC2, ENABLE);
	ADC_Cmd(ADC2, ENABLE);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_11, 1, ADC_SampleTime_84Cycles );
	ADC_SoftwareStartConv(ADC2);
	while(DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TCIF2) == RESET && i < 10000)//等待DMA传输完成
		i++;
	if (i >= 10000) {
		ADC_Cmd(ADC2, DISABLE);
		ADC_Cmd(ADC2, ENABLE);
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_Cmd(DMA2_Stream2, DISABLE);
		return 0;
	}
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
	DMA_Cmd(DMA2_Stream2, DISABLE);
	return adc2_buf;
}