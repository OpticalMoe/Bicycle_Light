/*!
 * @file        state.c
 * @brief       
 * @copyright   Copyright (c) 2022 ChenYuanliang
 * @licence     CC-BY-NC-SA 3.0，知识共享许可协议-署名-非商业使用-相同方式共享。
 * @author      ChenYuanliang
 * @version     V1.0
 * @date        2022-09-01
 * @url         https://github.com/OpticalMoe
 */
 
#include "state.h"
#include "stc8gxx.h"

long boot_time = 0;             // max 24day

long shutdown_countdown = 0;            // �ػ�����ʱ
const int shutdown_time_out = 20 * 1000;    // �ػ�ʱ��

int break_countdown = 0;                //ɲ�������Ƶ���ʱ
const int break_time_out = 3 * 1000;

int battery_countdown = 0;              //��ص�����ʾ ����ʱ
const int battery_time_out = 8 * 1000;  //��ص�����ʾ ʱ��

char flag = 0;

void TIM0_Isr() interrupt 1
{
    ++boot_time;
    if (shutdown_countdown > 0)
        --shutdown_countdown;
    else 
    {
        power = SLEEP;   
//        LED_G = 0;       
    }
    
    flag = (flag << 1) | (KEY_RIGHT);
    if ((flag & 0x03) == 0x02)
    {
        battery = BAT;
        time_reset_battery();
    }
    if (battery_countdown > 0)
        --battery_countdown;
    
    if (CONCUSSION_BREAK == 0)
        time_reset_break();
    if (break_countdown > 0)
        --break_countdown;    
}

long time_get_boot_time()
{
    return boot_time;
}

long time_get_shutdown()
{
    return shutdown_countdown;
}

int time_get_break()
{
    return break_countdown;
}

int time_get_battery()
{
    return battery_countdown;
}

//
void time_reset_shutdown()
{
    shutdown_countdown = shutdown_time_out;
}

void time_reset_break()
{
    break_countdown = break_time_out;
}

void time_break_zero()
{
    break_countdown = 0;
}

void time_reset_battery()
{
    battery_countdown = battery_time_out;
}


// ��ʱ��1������0ռ��


void timer_init(void)
{    
    reset_state();
    boot_time = 0;             // max 24day
    time_reset_shutdown();
    time_reset_break();
    
    // 1ms
//    AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
//	T2L = 0x20;		    //���ö�ʱ��ʼֵ
//	T2H = 0xD1;		    //���ö�ʱ��ʼֵ
//	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ 
//    IE2 = 0x04;         //ʹ�ܶ�ʱ���ж�  
    
    AUXR |= 0x80;   //��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;   //���ö�ʱ��ģʽ
	TMOD |= 0x03;   //���ö�ʱ��ģʽ
	TL0 = 0x20;		//���ö�ʱ��ʼֵ
	TH0 = 0xD1;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
    ET0 = 1;        //ʹ�ܶ�ʱ���ж�
}
