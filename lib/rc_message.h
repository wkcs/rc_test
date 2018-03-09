#ifndef __RC_MESSAGE_H__
#define __RC_MESSAGE_H__

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TEST_PARA_MES = 0,       //测试参数
    TEST_SAVE_MES,           //测试数据
    NULL_MES = 253,          //空白消息
    RC_DEBUG_MES,            //debug消息
    FIRMWARE_MES             //版本消息
}RC_MESSAGE; 

void rc_send_message(uint8_t *buf, uint8_t len, uint8_t mes_type);

#ifdef __cplusplus
}
#endif

#endif