#ifndef __TEST_MACHINE_COM_H__
#define __TEST_MACHINE_COM_H__

#include "sys.h"

#define SOT PDin(13)
#define EOT PDout(12)
#define BIN1 PBout(13)
#define BIN2 PBout(12)
#define BIN3 PBout(15)
#define BIN4 PBout(14)
#define BIN5 PDout(9)
#define BIN6 PDout(8)
#define BIN7 PDout(11)
#define BIN8 PDout(10)

void test_mach_init(void);
void test_mach_wait_start_sig(void);              /*Wait for the test machine to start the signal.*/
void test_mach_send_end_sig(uint8_t bin);         /*Send the test end signal, which includes the bin signal.*/

#endif

