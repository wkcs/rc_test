#ifndef __POWER_H__
#define __POWER_H__

#include "sys.h"
#include "delay.h"
#include "test_type.h"

#define SOCKET_POWER PAout(3)

typedef enum
{
  SOCKET_POWER_ON = 0,
  SOCKET_POWER_OFF
}socket_power_state;

typedef enum
{
  POWER_ON = 0,
  POWER_OFF
}socket_power_status;

inline void power_on(uint32_t time)
{
    SOCKET_POWER = SOCKET_POWER_ON;
    test_data.power_data.status = POWER_ON;
    delay_us(time);
}

inline void power_off(uint32_t time)
{
    SOCKET_POWER = SOCKET_POWER_OFF;
    test_data.power_data.status = POWER_OFF;
    delay_us(time);
}

inline void power_restart(uint32_t time)
{
    SOCKET_POWER = SOCKET_POWER_OFF;
    delay_us(time);
    SOCKET_POWER = SOCKET_POWER_ON;
    test_data.power_data.status = POWER_ON;
    delay_us(time);
}

char power_init(void);

#endif

