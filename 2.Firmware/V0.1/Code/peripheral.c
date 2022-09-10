#include "peripheral.h"
#include "stc8gxx.h"
#include <intrins.h>
#include <stdio.h>
#include "state.h"

/*
    // 不断电下载
    // 1.使用 P3.0/INT4 口的下降沿中断，检测串口起始信号 
    INTCLKO |= 0x40;    //使能 INT4 中断
    void Int4Isr() interrupt 16     //INT4 中断服务程序
    { 
        IAP_CONTR = 0x60;   //串口起始信号触发 INT4 中断
                            //软件复位到系统区
    }
    // 2.使用 P3.0/RxD 串口接收，检测 ISP 下载软件发送的用户下载命令 P858
    SCON = 0x50; //设置用户串口模式为 8 位数据位
    TMOD = 0x00; 
    AUXR = 0x40; 
    TH1 = BR115200 >> 8; //设置串口波特率为 115200
    TL1 = BR115200; 
    TR1 = 1; 
    ES = 1; 
    stage = 0;
    void UartIsr() interrupt 4 {...} //串口中断服务程序
        
    //中断唤醒
    IT0 = 0;                                    //使能INT0上升沿和下降沿中断
//  IT0 = 1;                                    //使能INT0下降沿中断
    EX0 = 1;                                    //使能INT0中断
    IT1 = 0;                                    //使能INT1上升沿和下降沿中断
//  IT1 = 1;                                    //使能INT1下降沿中断
    EX1 = 1;                                    //使能INT1中断
    INTCLKO = EX2;                              //使能INT2下降沿中断
    INTCLKO |= EX3;                             //使能INT3下降沿中断
    INTCLKO |= EX4;                             //使能INT4下降沿中断
    void INT0_Isr() interrupt 0 
    {
        P10 = !P10;                                 //测试端口
    }
    
    // 掉电模式    
    #define IDL             0x01                    //PCON.0
    #define PD              0x02                    //PCON.1
    PCON = IDL;                                 //MCU进入IDLE模式
//  PCON = PD;                                  //MCU进入掉电模式
    _nop_();                                    //掉电模式被唤醒后,MCU首先会执行此语句
                                                //然后再进入中断服务程序
*/

enum POWER power;

// usart
void Uart_SendChar(unsigned char  dat)
{
    SBUF = dat; 
    while(!TI); 
    TI = 0; 
}

char putchar(char c)//重定向
{
    Uart_SendChar(c);
    return c;
}

void UartIsr() interrupt 4 //串口中断服务程序
{ 
//    char dat; 
    if (TI) 
    { 
//        TI = 0; 
    } 
    if (RI) 
    {
        RI = 0; 
//        dat = SBUF; 
    } 
} 

// INT
void INT0_Isr() interrupt 0 
{
    if (power == WAKE)
        time_reset_shutdown();
    else
        power = SLEEP;
}

void INT1_Isr() interrupt 2 
{
    time_reset_shutdown();
}

// 不断电下载
void Int4Isr() interrupt 16     //INT4 中断服务程序
{ 
    IAP_CONTR = 0x60;   //串口起始信号触发 INT4 中断
                        //软件复位到系统区
}


void per_usart_init(void)
{
    // 使用定时器1
    #define FOSC 12000000UL
    #define BD115200 (65536 - FOSC / 4 / 115200)
    SCON = 0x50; //设置用户串口模式为 8 位数据位
    TMOD &= 0x0F;     
    AUXR &= 0x9E;
    AUXR |= 0x40;
    TH1 = BD115200 >> 8; //设置串口波特率为 115200  使用定时器1
    TL1 = BD115200;
    TI = 1;   //用系统自带的printf函数，TI必须等于1，否则无法发送。
    TR1 = 1; 
    ES = 1;   
    printf("\r\n");     // 不加这句跑不了，只能发换行
}

void per_io_init(void)
{
    /*
    // IO配置
    P0M0 = 0x00;                                //设置P0.0~P0.7为双向口模式
    P0M1 = 0x00;
    P1M0 = 0xff;                                //设置P1.0~P1.7为推挽输出模式
    P1M1 = 0x00;
    P2M0 = 0x00;                                //设置P2.0~P2.7为高阻输入模式
    P2M1 = 0xff;
    P3M0 = 0xff;                                //设置P3.0~P3.7为开漏模式
    P3M1 = 0xff;
	*/	
    P1M0 = 0x00;	// OAAx OOOO  
    P1M1 = 0x60;
    P3M0 = 0x00;	// IIOI II??
    P3M1 = 0x00;
    P5M0 = 0x00;	//xxxX xxxx
    P5M1 = 0x00;    
}

void per_int_init(void)
{
    // 外部中断唤醒
    //#define KEY_LEFT    P32 //INT0  KEY_1
    IT0 = 1;        //使能INT0下降沿中断
    EX0 = 1;        //使能INT0中断
    IT1 = 1;        //使能INT1下降沿中断
    EX1 = 1;        //使能INT1中断
    
    //不断电下载 1
    INTCLKO |= 0x40;    //使能 INT4 中断
}

void per_off_all(void)
{    
    light_off();
    LED_G = 1;
    LED_B = 1;
}

void reset_state(void)
{
    power = WAKE;
    time_reset_shutdown();
    time_break_zero();
}

void per_power(enum POWER _power)
{
    per_off_all();
    // 掉电模式    
    if (_power > SLEEP)
        return;
    PCON = _power;       //MCU进入IDLE模式 / MCU进入掉电模式
    _nop_();            //掉电模式被唤醒后,MCU首先会执行此语句
                        //然后再进入中断服务程序
    reset_state();
}



