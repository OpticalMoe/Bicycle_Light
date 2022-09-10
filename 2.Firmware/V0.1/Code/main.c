#include "stc8gxx.h"
#include <stdio.h>
#include <intrins.h>
#include "light.h"
#include "adc.h"
#include "peripheral.h"
#include "state.h"

void Delay600us()		//@12.000MHz
{
	unsigned char i, j;

	i = 10;
	j = 87;
	do
	{
		while (--j);
	} while (--i);
}



void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 16;
	j = 147;
	do
	{
		while (--j);
	} while (--i);
}

void delay_ms(int number) 
{
    while(number--)
    {
       Delay1ms(); 
    }
}

void main()
{
    int i, j;
    unsigned short temp = 0;
    unsigned short battery_light = 0x0000;

    per_io_init();
    per_int_init();
    adc_init();
    light_init();
    timer_init();
    per_usart_init();
    reset_state();
    EA = 1; 
    
    while (1)
    {        
//        if (time_get_boot_time() % 500 == 0)
//        {
//            LED_B = !LED_B;
////            printf("power: 0x%02x\t", power);
//            
//            printf("cur: %d\tshut: %d\tbreak: %d \r\n", 
//                    (int)(time_get_boot_time() / 1000), (int)(time_get_shutdown() / 1000), 
//                        time_get_break());
//        }
                    
        if (power == WAKE)
        {
            printf("Start\r\n");
            power = RUN;
            light_running(TURN_ON);
        }
        else if (power == SLEEP)
        {
            printf("Sleep\r\n");
            light_running(TURN_OFF);
            per_power(SLEEP);
            continue;
        }
        else        //RUN
        {
            if (time_get_break() > 0) //刹车
            {
                printf("Break\r\n");
                if (time_get_break() % 500 < 250)
                    light(0xFFFF);
                else                    
                    light(0x0000);
            }
            else if(time_get_battery() > 0)     // 电量显示
            {
                if(battery == BAT)  // 获取电量
                {
                    battery = NO;
                    light(0x0000);
                    light_reset_last_number();
                    LED_G = 0;
                    
                    temp = adc_get_battery_electricity();
//                    temp = 10;
                    printf("Battery: %d %% \tV: %d\r\n", temp, adc_get_voltage(ADC_1_19));
                    temp = (unsigned short)(temp / 10.0 + 0.5);
                    battery_light = 0;
                    while(temp > 0)
                    {
                        battery_light <<= 1;
                        ++battery_light;
                        --temp;
                    }
                    temp = 0;
                }
                else if (time_get_battery() % 500 == 0)    // 显示
                {
                    ++temp;
                    light(light_get_last_number() | (battery_light & (0x0001 << temp)));
                }
            }
            else                    //RUN
            {
                LED_G = 1;
                if (time_get_boot_time() % 3000 < 1000)
                    light(0x0000);
                else if (time_get_boot_time() % 3000 < 2000)
                    light(0x5555);
                else
                    light(0xAAAA);
            }
        }
        
        Delay600us();
    }
}

