#include "dma.h"

void DMA_config(DMA_Stream_TypeDef *DMA_Streamx, uint32_t chx, uint32_t par, uint32_t mar, uint16_t ndtr)
{

    DMA_InitTypeDef DMA_InitStructure;

    if ((uint32_t)DMA_Streamx > (uint32_t)DMA2) //得到当前stream是属于DMA2还是DMA1
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2时钟使能
    }
    else
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); //DMA1时钟使能
    }
    DMA_DeInit(DMA_Streamx);

    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
    {
    } //等待DMA可配置

    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = chx;                                    //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = par;                         //DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = mar;                            //DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 //外设到存储器模式
    DMA_InitStructure.DMA_BufferSize = ndtr;                               //数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据长度:16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //存储器数据长度:16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // 使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                   //中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输
    DMA_Init(DMA_Streamx, &DMA_InitStructure);                          //初始化DMA Stream
}