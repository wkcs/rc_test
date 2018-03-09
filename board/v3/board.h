#ifndef __BOARD_H__
#define __BOARD_H__

#include "sys.h"

#include "usart.h"
#include "adc.h"
#include "power.h"
#include "w25qxx.h"
#include "xm9816.h"
#include "test_machine_com.h"
#include "test_signal.h"
#include "timer.h"
#include "freq.h"
#include "dac.h"
#include "usbd_cdc_vcp.h"
#include "usbd_usr.h" 
#include "usbd_desc.h"

void rc11x_test_v3_board_init(void);

#endif