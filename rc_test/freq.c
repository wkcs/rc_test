#include "freq.h"
#include "test_type.h"
#include "rc_err.h"

int32_t get_freq(uint16_t test_num)
{
	uint32_t temp = 0, freq;
	uint8_t i = 0;
try_again:
	test_data.freq_data.status = 1;
	test_data.freq_data.need_num = test_num;
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE); //使能定时器2
	while (test_data.freq_data.status < 3)
	{
		//printf("wait\t\n");
	} //等待采集完成
	if (test_data.freq_data.status != 3)
	{
		test_data.freq_data.status = 0;
		return -NO_FREQ;
	}
	else
	{
		if(test_data.freq_data.timer_updata_num == 0 && test_data.freq_data.timer_data < 10 && i < 5)
		{
			i++;
			goto try_again;
		}
		temp = test_data.freq_data.timer_updata_num * 65536;
		temp += test_data.freq_data.timer_data;
		freq = (uint32_t)((uint64_t)1000000 * (test_data.freq_data.get_num) / (uint64_t)temp);
		test_data.freq_data.status = 0;
		return freq;
	}
}
