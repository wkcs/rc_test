#include "adc.h"
#include "delay.h"

//初始化ADC
//ch: ADC_channels
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
void adc_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  //使能ADC1时钟

    //先初始化ADC1通道5 IO口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;        //PA5 通道5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不带上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);           //初始化

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);  //ADC1复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE); //复位结束

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样阶段之间的延迟5个时钟
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;      //DMA失能
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
    ADC_CommonInit(&ADC_CommonInitStructure);                                    //初始化

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                      //12位模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                               //非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                         //关闭连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //1个转换在规则序列中 也就是只转换规则序列1
    ADC_Init(ADC1, &ADC_InitStructure);                                         //ADC初始化

    ADC_Cmd(ADC1, ENABLE); //开启AD转换器
}

u16 get_adc(u8 ch)
{
    ADC_Cmd(ADC1, DISABLE);
    ADC_Cmd(ADC1, ENABLE); 
    //设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

u16 get_adc_average(u8 ch, u8 times)
{
    u32 temp_val = 0;
    u8 t;
    for (t = 0; t < times; t++)
    {
        temp_val += get_adc(ch);
        delay_ms(1);
    }
    return temp_val / times;
}

uint16_t get_socket_voltage(uint8_t num)
{
    u16 adcx;
    uint16_t socket_voltage;
    float temp;
    adcx = get_adc_average(ADC_Channel_4, num);
    temp = (float)adcx * (3.332 / 4096);
    socket_voltage = temp * 2000;
    return socket_voltage;
}

uint16_t get_socket_voltage_one(void)
{
    u16 adcx;
    uint16_t socket_voltage;
    float temp;
    adcx = get_adc(ADC_Channel_4);
    temp = (float)adcx * (3.332 / 4096);
    socket_voltage = temp * 2000;
    return socket_voltage;
}

uint16_t get_socket_current(uint8_t num)
{
    u16 adcx;
    uint16_t socket_current;
    float temp;
    adcx = get_adc_average(ADC_Channel_5, num);
    temp = (float)adcx * (3.332 / 4096);
    socket_current = (temp * 2041 - 150) * 1000 / 500;
    return socket_current;
}

uint16_t get_socket_current_one(void)
{
    u16 adcx;
    uint16_t socket_current;
    float temp;
    adcx = get_adc(ADC_Channel_5);
    temp = (float)adcx * (3.332 / 4096);
    socket_current = (temp * 2041 - 150) * 1000 / 500;
    return socket_current;
}
