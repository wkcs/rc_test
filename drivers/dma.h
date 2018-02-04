#ifndef __DMA_H__
#define __DMA_H__

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

void DMA_config(DMA_Stream_TypeDef *DMA_Streamx, uint32_t chx, uint32_t par, uint32_t mar, uint16_t ndtr);

#ifdef __cplusplus
}
#endif

#endif