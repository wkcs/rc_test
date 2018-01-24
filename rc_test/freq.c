#include "freq.h"
#include "timer.h"
#include "test_type.h"

uint32_t get_frequency(uint16_t test_num)
{
	uint32_t temp = 0, freq;
    test_data.freq_data.timer_num = 0;
	freq_num = 0;
	freq_test_num = test_num;
	__HAL_TIM_ENABLE(&TIM2_Handler);              /*使能定时器2*/
	while((TIM2CH1_CAPTURE_STA&0X80)==0);         /*等待采集完成*/
	freq_test_num = 10000;                        /*采集完成恢复默认值*/
	if (TIM2CH1_CAPTURE_STA == 0xff && TIM2CH1_CAPTURE_VAL == 0xffff) {
		return 0;
	}
	else {
		temp = (TIM2CH1_CAPTURE_STA & 0X3F) * 65536;
		temp += (uint16_t)TIM2CH1_CAPTURE_VAL;
		freq = (uint32_t)((uint64_t)1000000 * freq_num / (uint64_t)temp);
		return freq;
	}
}





