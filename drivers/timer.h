#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FREQ_TEST PBout(7)
#define FREQ_TEST2 PBout(9)

void TIM3_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM5_Init(u16 arr,u16 psc);

#ifdef __cplusplus
}
#endif

#endif
