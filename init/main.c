#include "board.h"
#include "test_type.h"
#include "rc11x_test.h"
#include "print_para.h"
#include "test_com.h"
#include "power.h"

int main(void)
{
    uint8_t bin;
    uint8_t buf[8];
    rc11x_test_v3_board_init();
    test_para_restore();
    test_para_init();
    print_test_para();
    /*while(1)
    {
        buf[0] = (uint8_t)(test_save.err_save >> 24);
        buf[1] = (uint8_t)(test_save.err_save >> 16);
        buf[2] = (uint8_t)(test_save.err_save >> 8);
        buf[3] = (uint8_t)(test_save.err_save);
        buf[4] = (uint8_t)(test_save.bin_save);
        buf[5] = (uint8_t)(test_save.freq_save.code >> 8);
        buf[6] = (uint8_t)(test_save.freq_save.code);
        send_data(buf, 7);
        printf("tx ok!!\r\n");
        delay_ms(1000);
    }*/
    while(1)
    {
        if (test_data.test_machine_data.test_start)
        {
            bin = rc11x_test_start(); 
            buf[0] = (uint8_t)(test_save.err_save >> 24);
            buf[1] = (uint8_t)(test_save.err_save >> 16);
            buf[2] = (uint8_t)(test_save.err_save >> 8);
            buf[3] = (uint8_t)(test_save.err_save);
            buf[4] = (uint8_t)(test_save.bin_save);
            buf[5] = (uint8_t)(test_save.freq_save.code >> 8);
            buf[6] = (uint8_t)(test_save.freq_save.code);
            send_data(buf, 7);
            send_data(buf, 7);
            if (test_save.err_save < 0)
            {
                printf("error:%s\r\n", get_err_name(test_save.err_save));
            }
            else
            {
                printf("freq:%d\r\n", test_save.freq_save.end_freq);
                printf("freq_err:%d\r\n", test_save.freq_save.freq_err);
                printf("code:0x%03x\r\n", test_save.freq_save.code);
            }
            printf("bin:Bin%d\r\n", bin);
            test_mach_send_end_sig(bin);
            test_data.test_machine_data.test_start = DISABLE;
        }
        else
        {
            get_data();
        }
    }
}

