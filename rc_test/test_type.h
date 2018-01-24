#ifndef __TEST_TYPE_H__
#define __TEST_TYPE_H__

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __test_machine_para{
    uint8_t  test_start;
}test_machine_para_t;

typedef struct __test_para{
    test_machine_para_t test_machine_para;
}test_para_t;

typedef struct __freq_data{   
    uint8_t timer_num;			    				
    uint16_t timer_data;	
    uint16_t ris_edge_num;
    uint16_t test_num;
}freq_data_t;

typedef struct __test_data{
    freq_data_t freq_data;
}test_data_t;

extern test_para_t test_para;
extern test_data_t test_data;

#ifdef __cplusplus
}
#endif

#endif