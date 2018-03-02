#include "freq.h"
#include "test_type.h"
#include "rc_err.h"
#include "dma.h"

uint16_t tim_ccr_data[10002];

void freq_init(void)
{
	DMA_config(DMA1_Stream3, DMA_Channel_2, (uint32_t)&TIM4->CCR2, (uint32_t)tim_ccr_data, 1002);
}

int32_t get_freq(uint16_t test_num)
{
	uint32_t temp = 0, freq;
	test_data.freq_data.timer_updata_num = 0;
	test_data.freq_data.need_num = test_num;
	if (test_data.freq_data.need_num > 10000)
		return -NO_FREQ;
	TIM_SetCounter(TIM4, 0);
	DMA1_Stream3->NDTR = test_data.freq_data.need_num + 2;
	DMA_Cmd(DMA1_Stream3, ENABLE);
	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);  //防止因为没有频率时造成的DMA误以为已经传输完成，导致输出错误
	TIM_Cmd(TIM4, ENABLE);
	while((DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3) == RESET) && (test_data.freq_data.timer_updata_num < 255));//等待DMA1_Stream3传输完成
	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);//清除DMA1_Stream3传输完成标志
	TIM_Cmd(TIM4, DISABLE);
	DMA_Cmd(DMA1_Stream3, DISABLE);
	if (test_data.freq_data.timer_updata_num < 255) {         //检验是否溢出
		temp = test_data.freq_data.timer_updata_num * 65536;
		temp += tim_ccr_data[test_data.freq_data.need_num + 1] - tim_ccr_data[1];
		freq = (uint32_t)((uint64_t)84000000 * (test_data.freq_data.need_num) / (uint64_t)temp);
		return freq;
	} else 
		return -NO_FREQ;
}
