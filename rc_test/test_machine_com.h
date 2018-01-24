#ifndef __TEST_MACHINE_COM_H__
#define __TEST_MACHINE_COM_H__

#include "sys.h"

//#define SOT PEin(2)
#define EOT PEout(3)
#define BIN1 PEout(4)
#define BIN2 PEout(5)
#define BIN3 PEout(6)
#define BIN4 PCout(13)
#define BIN5 PCout(14)
#define BIN6 PCout(15)
#define BIN7 PCout(0)
#define BIN8 PCout(1)

void test_mach_init(void);
void test_mach_wait_start_sig(void);              /*Wait for the test machine to start the signal.*/
void test_mach_send_end_sig(uint8_t bin);         /*Send the test end signal, which includes the bin signal.*/

#endif

