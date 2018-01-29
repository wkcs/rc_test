/*
* filename  test_signal.h
*
* author    胡启航
*
* date      2017/4/17
*
* version   v1.0.1
*/

#ifndef __TEST_SIGNAL_H__
#define __TEST_SIGNAL_H__

#include "sys.h"

enum TEST_MOD{
    NORMAL_MODE = 0,
    START_MODE,
    CAL_MODE,
    EFUSE_MODE,
    CHECK_MODE
};


#define START 0X04
/*118 & 119*/
#define K7 PEout(10)
#define K8 PEout(9)
#define K9 PEout(8)
#define K1 PEout(7)
#define K2 PBout(1)
#define K3 PBout(0)
#define K0 PCout(5)
#define K4 PCout(4)
#define K5 PAout(7)
#define K6 PAout(6)


#define CLK PCout(5)
#define DATA PEout(7)

/*114*/
#define D0 PEout(10)
#define D1 PEout(9)
#define D2 PEout(8)
#define D3 PEout(7)
#define T0 PBout(1)
#define A3 PBout(0)
#define A4 PCout(5)
#define A5 PCout(4)
#define A6 PAout(7)
#define A7 PAout(6)
#define T1_1 PAout(1)
#define T1_15 PAout(2)

char test_signal_init(void);
char start_sig(void);
char cal_sig(unsigned short int data);
char efuse_sig(unsigned short int data);
char check_sig(void);
void exit_test(void);
char cal_sig_114s(unsigned short int data);
char efuse_sig_114s(unsigned short int data);

#endif



