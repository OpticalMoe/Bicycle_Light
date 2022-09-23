/*!
 * @file        state.h
 * @brief       
 * @copyright   Copyright (c) 2022 ChenYuanliang
 * @licence     CC-BY-NC-SA 3.0，知识共享许可协议-署名-非商业使用-相同方式共享。
 * @author      ChenYuanliang
 * @version     V1.0
 * @date        2022-09-01
 * @url         https://github.com/OpticalMoe
 */
 
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

