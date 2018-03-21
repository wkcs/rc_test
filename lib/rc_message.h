#ifndef __RC_MESSAGE_H__
#define __RC_MESSAGE_H__

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TEST_PARA_MES = 0,       //测试参数
    TEST_SAVE_MES,           //测试数据
    FREQ_MES,
    KEY_TEST_MES,
    BOARD_MES,
    CODE_MES,
    TARGET_FREQ_ERR_MES = 251,
    NO_FREQ_MES,
    NULL_MES,          //空白消息
    RC_DEBUG_MES,            //debug消息
    FIRMWARE_MES             //版本消息
}RC_MESSAGE; 

typedef enum {
    USB = 1,
    UART1,
    UART2,
    UART3 
}MES_DEVICE_TYPE;

void rc_send_message(uint8_t *buf, uint8_t len, uint8_t mes_type);
uint8_t rc_get_message(uint8_t **buf, uint8_t *len, uint8_t *mes_type);
void rc_clear_message(void);

#ifdef __cplusplus
}
#endif

#endif