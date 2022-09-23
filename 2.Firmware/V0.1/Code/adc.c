/*!
 * @file        adc.c
 * @brief       adc
 * @copyright   Copyright (c) 2022 ChenYuanliang
 * @licence     CC-BY-NC-SA 3.0，知识共享许可协议-署名-非商业使用-相同方式共享。
 * @author      ChenYuanliang
 * @version     V1.0
 * @date        2022-09-01
 * @url         https://github.com/OpticalMoe
 */
 
#include "adc.h"
#include "stc8gxx.h"
#include <intrins.h>
#include <stdio.h>

//#define ENV_LIGHT_1 P15 //ADC5
//#define ENV_LIGHT_2 P16 //ADC6

int *BGV;   //�ڲ�1.19V�ο��ź�Դֵ�����idata��
            //idata��EFH��ַ��Ÿ��ֽ�
            //idata��F0H��ַ��ŵ��ֽ�
            //��ѹ��λΪ����(mV)
            
enum ENV env;
enum BATTERY battery;

unsigned int battery_voltage = 0;
            
void adc_init(void)
{
    BGV = (int idata *)0xef;
    P1M0 &= 0x9F;	// P1.5 & P1.6
    P1M1 |= 0x60;
    P_SW2 |= 0x80;
    ADCTIM = 0x3f;                              //����ADC�ڲ�ʱ��
    P_SW2 &= 0x7f;
    ADCCFG = 0x2f;                              // �Ҷ��룬����ADCʱ��Ϊϵͳʱ��/2/16
    ADC_CONTR = 0x80;                           //ʹ��ADCģ��
}

int adc_get_voltage(enum ADC_CHS adc_chs)
{    
    ADC_CONTR &= 0xF0;    
    ADC_CONTR |= 0x0F;                      // ADC�ڲ���׼Դ1.19V
    ADC_CONTR |= 0x40;                      //����ADת��
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20));            //��ѯADC��ɱ�־
    ADC_CONTR &= ~0x20;                     //����ɱ�־
    battery_voltage = (int)(1024L * *BGV / ((ADC_RES << 8) | ADC_RESL));
    if (adc_chs == ADC_1_19)
        return battery_voltage;
    
    //ADC
    ADC_CONTR &= 0xF0;   
    ADC_CONTR |= adc_chs;
    ADC_CONTR |= 0x40;                      //����ADת��
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20));            //��ѯADC��ɱ�־
    ADC_CONTR &= ~0x20;                     //����ɱ�־
    return (int)(battery_voltage / 1024L * ((ADC_RES << 8) | ADC_RESL));
}

int adc_get_battery_electricity()
{
    adc_get_voltage(ADC_1_19);
    return (int)((battery_voltage - 3000) * 100.0 / 1200 + 0.5);     // 3V��ֹ    
}

