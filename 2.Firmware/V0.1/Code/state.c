#include "state.h"
#include "stc8gxx.h"

long boot_time = 0;             // max 24day

long shutdown_countdown = 0;            // 关机倒计时
const int shutdown_time_out = 20 * 1000;    // 关机时间

int break_countdown = 0;                //刹车灯亮灯倒计时
const int break_time_out = 3 * 1000;

int battery_countdown = 0;              //电池电量显示 倒计时
const int battery_time_out = 8 * 1000;  //电池电量显示 时长

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


// 定时器1被串口0占用


void timer_init(void)
{    
    reset_state();
    boot_time = 0;             // max 24day
    time_reset_shutdown();
    time_reset_break();
    
    // 1ms
//    AUXR |= 0x04;		//定时器时钟1T模式
//	T2L = 0x20;		    //设置定时初始值
//	T2H = 0xD1;		    //设置定时初始值
//	AUXR |= 0x10;		//定时器2开始计时 
//    IE2 = 0x04;         //使能定时器中断  
    
    AUXR |= 0x80;   //定时器时钟1T模式
	TMOD &= 0xF0;   //设置定时器模式
	TMOD |= 0x03;   //设置定时器模式
	TL0 = 0x20;		//设置定时初始值
	TH0 = 0xD1;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;        //使能定时器中断
}
