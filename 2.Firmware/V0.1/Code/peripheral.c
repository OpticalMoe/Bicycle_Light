/*!
 * @file        peripheral.c
 * @brief       
 * @copyright   Copyright (c) 2022 ChenYuanliang
 * @licence     CC-BY-NC-SA 3.0，知识共享许可协议-署名-非商业使用-相同方式共享。
 * @author      ChenYuanliang
 * @version     V1.0
 * @date        2022-09-01
 * @url         https://github.com/OpticalMoe
 */
 
#include "peripheral.h"
#include "stc8gxx.h"
#include <intrins.h>
#include <stdio.h>
#include "state.h"

/*
    // ���ϵ�����
    // 1.ʹ�� P3.0/INT4 �ڵ��½����жϣ���⴮����ʼ�ź� 
    INTCLKO |= 0x40;    //ʹ�� INT4 �ж�
    void Int4Isr() interrupt 16     //INT4 �жϷ������
    { 
        IAP_CONTR = 0x60;   //������ʼ�źŴ��� INT4 �ж�
                            //������λ��ϵͳ��
    }
    // 2.ʹ�� P3.0/RxD ���ڽ��գ���� ISP �����������͵��û��������� P858
    SCON = 0x50; //�����û�����ģʽΪ 8 λ����λ
    TMOD = 0x00; 
    AUXR = 0x40; 
    TH1 = BR115200 >> 8; //���ô��ڲ�����Ϊ 115200
    TL1 = BR115200; 
    TR1 = 1; 
    ES = 1; 
    stage = 0;
    void UartIsr() interrupt 4 {...} //�����жϷ������
        
    //�жϻ���
    IT0 = 0;                                    //ʹ��INT0�����غ��½����ж�
//  IT0 = 1;                                    //ʹ��INT0�½����ж�
    EX0 = 1;                                    //ʹ��INT0�ж�
    IT1 = 0;                                    //ʹ��INT1�����غ��½����ж�
//  IT1 = 1;                                    //ʹ��INT1�½����ж�
    EX1 = 1;                                    //ʹ��INT1�ж�
    INTCLKO = EX2;                              //ʹ��INT2�½����ж�
    INTCLKO |= EX3;                             //ʹ��INT3�½����ж�
    INTCLKO |= EX4;                             //ʹ��INT4�½����ж�
    void INT0_Isr() interrupt 0 
    {
        P10 = !P10;                                 //���Զ˿�
    }
    
    // ����ģʽ    
    #define IDL             0x01                    //PCON.0
    #define PD              0x02                    //PCON.1
    PCON = IDL;                                 //MCU����IDLEģʽ
//  PCON = PD;                                  //MCU�������ģʽ
    _nop_();                                    //����ģʽ�����Ѻ�,MCU���Ȼ�ִ�д����
                                                //Ȼ���ٽ����жϷ������
*/

enum POWER power;

// usart
void Uart_SendChar(unsigned char  dat)
{
    SBUF = dat; 
    while(!TI); 
    TI = 0; 
}

char putchar(char c)//�ض���
{
    Uart_SendChar(c);
    return c;
}

void UartIsr() interrupt 4 //�����жϷ������
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

// ���ϵ�����
void Int4Isr() interrupt 16     //INT4 �жϷ������
{ 
    IAP_CONTR = 0x60;   //������ʼ�źŴ��� INT4 �ж�
                        //������λ��ϵͳ��
}


void per_usart_init(void)
{
    // ʹ�ö�ʱ��1
    #define FOSC 12000000UL
    #define BD115200 (65536 - FOSC / 4 / 115200)
    SCON = 0x50; //�����û�����ģʽΪ 8 λ����λ
    TMOD &= 0x0F;     
    AUXR &= 0x9E;
    AUXR |= 0x40;
    TH1 = BD115200 >> 8; //���ô��ڲ�����Ϊ 115200  ʹ�ö�ʱ��1
    TL1 = BD115200;
    TI = 1;   //��ϵͳ�Դ���printf������TI�������1�������޷����͡�
    TR1 = 1; 
    ES = 1;   
    printf("\r\n");     // ��������ܲ��ˣ�ֻ�ܷ�����
}

void per_io_init(void)
{
    /*
    // IO����
    P0M0 = 0x00;                                //����P0.0~P0.7Ϊ˫���ģʽ
    P0M1 = 0x00;
    P1M0 = 0xff;                                //����P1.0~P1.7Ϊ�������ģʽ
    P1M1 = 0x00;
    P2M0 = 0x00;                                //����P2.0~P2.7Ϊ��������ģʽ
    P2M1 = 0xff;
    P3M0 = 0xff;                                //����P3.0~P3.7Ϊ��©ģʽ
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
    // �ⲿ�жϻ���
    //#define KEY_LEFT    P32 //INT0  KEY_1
    IT0 = 1;        //ʹ��INT0�½����ж�
    EX0 = 1;        //ʹ��INT0�ж�
    IT1 = 1;        //ʹ��INT1�½����ж�
    EX1 = 1;        //ʹ��INT1�ж�
    
    //���ϵ����� 1
    INTCLKO |= 0x40;    //ʹ�� INT4 �ж�
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
    // ����ģʽ    
    if (_power > SLEEP)
        return;
    PCON = _power;       //MCU����IDLEģʽ / MCU�������ģʽ
    _nop_();            //����ģʽ�����Ѻ�,MCU���Ȼ�ִ�д����
                        //Ȼ���ٽ����жϷ������
    reset_state();
}



