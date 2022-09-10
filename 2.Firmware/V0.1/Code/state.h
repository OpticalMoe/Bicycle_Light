#ifndef __STATE_H__
#define __STATE_H__

#include "light.h"
#include "adc.h"
#include "peripheral.h"

void timer_init(void);
long time_get_boot_time();
long time_get_shutdown();
int time_get_break();
int time_get_battery();

void time_reset_shutdown();
void time_reset_break();
void time_break_zero();
void time_reset_battery();

#endif

