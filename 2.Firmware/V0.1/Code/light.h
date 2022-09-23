/*!
 * @file        light.h
 * @brief       light
 * @copyright   Copyright (c) 2022 ChenYuanliang
 * @licence     CC-BY-NC-SA 3.0，知识共享许可协议-署名-非商业使用-相同方式共享。
 * @author      ChenYuanliang
 * @version     V1.0
 * @date        2022-09-01
 * @url         https://github.com/OpticalMoe
 */
 
#ifndef __LIGHT_H__
#define __LIGHT_H__

enum LIGHT 
{
    TURN_OFF = 0,
    TURN_ON
};

void light_init(void);
void light_off(void);
void light_on(void);
void light(unsigned short number);
void light_running(enum LIGHT _switch);
unsigned short light_get_last_number(void);
void light_reset_last_number(void);

#endif
