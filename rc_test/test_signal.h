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
#define K7 PBout(2)
#define K8 PCout(6)
#define K9 PEout(7)
#define K1 PDout(15)
#define K2 PEout(6)
#define K3 PEout(0)
#define K0 PEout(11)
#define K4 PDout(6)
#define K5 PEout(10)
#define K6 PDout(4)


#define CLK PEout(11)
#define DATA PDout(15)

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

#define OS_TEST_1 PCout(13)
#define OS_TEST_3 PEout(1)
#define OS_TEST_4 PEout(13)
#define OS_TEST_5 PAout(8)
#define OS_TEST_6 PEout(12)
#define OS_TEST_7 PAout(15)
#define OS_TEST_8 PEout(9)
#define OS_TEST_9 PDout(14)
#define OS_TEST_10 PEout(8)
#define OS_TEST_11 PCout(7)
#define OS_TEST_12 PEout(5)
#define OS_TEST_13 PBout(8)

char test_signal_init(void);
char start_sig(void);
char cal_sig(unsigned short int data);
char efuse_sig(unsigned short int data);
char check_sig(void);
void exit_test(void);
char cal_sig_114s(unsigned short int data);
char efuse_sig_114s(unsigned short int data);

#endif



