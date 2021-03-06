#include "power.h"

char power_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   //2MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);             //初始化
    SOCKET_POWER = SOCKET_POWER_OFF;
    //GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    test_data.power_data.status = POWER_OFF; 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   //2MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);             //初始化
    GPIO_ResetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_3);

    return 0;
}



