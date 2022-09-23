/*!
 * @file        light.c
 * @brief       light
 * @copyright   Copyright (c) 2022 ChenYuanliang
 * @licence     CC-BY-NC-SA 3.0，知识共享许可协议-署名-非商业使用-相同方式共享。
 * @author      ChenYuanliang
 * @version     V1.0
 * @date        2022-09-01
 * @url         https://github.com/OpticalMoe
 */
 
#include "light.h"
#include "stc8gxx.h"

//LED
#define CLK P10
#define SDI P13
#define LE  P12
#define OE  P11

unsigned short led_last_number = 0x0000;

void _Delay10us()		//@12.000MHz
{
	unsigned char i;

	i = 38;
	while (--i);
}

void _Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 16;
	j = 147;
	do
	{
		while (--j);
	} while (--i);
}

void _delay_ms(int number) 
{
    while(number--)
    {
       _Delay1ms(); 
    }
}

void light_init(void)
{
    // P1.0 => CLK  ������
	// P1.1 => OE	�͵�ƽ������LED���
	// P1.2 => LE	�ߵ�ƽ���������ݴ�������������
	// P1.3 => SDI	��������
    //���� Ϊ˫���ģʽ
    P1M0 &= 0xF0;	// xxxx OOOO  
    P1M1 &= 0xF0;
}

void light_off(void)
{
    OE = 1;
}

void light_on(void)
{
    OE = 0;
}

void light(unsigned short number) 
{	
	// P1.0 => CLK  ������
	// P1.1 => OE	�͵�ƽ������LED���
	// P1.2 => LE	�ߵ�ƽ���������ݴ�������������
	// P1.3 => SDI	��������
    char i, j;
    
    if(number == 0x0000)
    {
        light_off();     //�ر����
        return;
    }
    
    led_last_number = number;
    CLK = 0;
	OE = 1;
	LE = 1;
    _Delay10us();
    for (i = 0; i < 16; ++i)
    {
        SDI = number & 0x8000;
        CLK = 1;
        _Delay10us();
        
        CLK = 0;
        number <<= 1;
        _Delay10us();
    }
	OE = 0;
    LE = 0;	
}

unsigned short light_get_last_number(void)
{
    return led_last_number; 
}

void light_reset_last_number(void)
{
    led_last_number = 0x0000;
}

void light_running(enum LIGHT _switch)
{
    char i;
    unsigned short number = 0;
    if (_switch == TURN_OFF)
        number = 0xFFFF;
    for (i = 0; i < 16; ++i)
    {
        if (_switch == TURN_ON)
            number |= 0x0001 << i;
        else
            number &= 0x7FFF << i;
        light(number);
        _delay_ms(100);
    }
}

