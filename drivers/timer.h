#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

void TIM3_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM5_Init(u16 arr,u16 psc);
void TIM2_CH3_Cap_Init(u32 arr,u16 psc);

#ifdef __cplusplus
}
#endif

#endif
