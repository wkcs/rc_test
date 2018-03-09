#ifndef __POWER_H__
#define __POWER_H__

#include "sys.h"
#include "delay.h"
#include "test_type.h"
#include "dac.h"

#define SOCKET_POWER PAout(0)
#define CURRENT_TETS_L PEout(3)
#define CURRENT_TETS_H PEout(2)

typedef enum {
	SOCKET_POWER_ON = 0,
	SOCKET_POWER_OFF
} socket_power_state;

typedef enum {
	POWER_ON = 0,
	POWER_OFF
} socket_power_status;

__INLINE void power_on(uint32_t time)
{
	SOCKET_POWER = SOCKET_POWER_ON;
	test_data.power_data.status = POWER_ON;
	delay_us(time);
}

__INLINE void power_off(uint32_t time)
{
	SOCKET_POWER = SOCKET_POWER_OFF;
	test_data.power_data.status = POWER_OFF;
	delay_us(time);
}

__INLINE void power_restart(uint32_t time)
{
	SOCKET_POWER = SOCKET_POWER_OFF;
	delay_us(time);
	SOCKET_POWER = SOCKET_POWER_ON;
	test_data.power_data.status = POWER_ON;
	delay_us(time);
}

__INLINE void set_power_vol(uint16_t vol)
{
	rc_power_con_dac(vol);
}

__INLINE uint16_t read_power_vol(void)
{
	uint16_t vol = 0;
	return vol;
}

char power_init(void);

#endif

