#include "adc.h"
#include "stc8gxx.h"
#include <intrins.h>
#include <stdio.h>

//#define ENV_LIGHT_1 P15 //ADC5
//#define ENV_LIGHT_2 P16 //ADC6

int *BGV;   //内部1.19V参考信号源值存放在idata中
            //idata的EFH地址存放高字节
            //idata的F0H地址存放低字节
            //电压单位为毫伏(mV)
            
enum ENV env;
enum BATTERY battery;

unsigned int battery_voltage = 0;
            
void adc_init(void)
{
    BGV = (int idata *)0xef;
    P1M0 &= 0x9F;	// P1.5 & P1.6
    P1M1 |= 0x60;
    P_SW2 |= 0x80;
    ADCTIM = 0x3f;                              //设置ADC内部时序
    P_SW2 &= 0x7f;
    ADCCFG = 0x2f;                              // 右对齐，设置ADC时钟为系统时钟/2/16
    ADC_CONTR = 0x80;                           //使能ADC模块
}

int adc_get_voltage(enum ADC_CHS adc_chs)
{    
    ADC_CONTR &= 0xF0;    
    ADC_CONTR |= 0x0F;                      // ADC内部基准源1.19V
    ADC_CONTR |= 0x40;                      //启动AD转换
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20));            //查询ADC完成标志
    ADC_CONTR &= ~0x20;                     //清完成标志
    battery_voltage = (int)(1024L * *BGV / ((ADC_RES << 8) | ADC_RESL));
    if (adc_chs == ADC_1_19)
        return battery_voltage;
    
    //ADC
    ADC_CONTR &= 0xF0;   
    ADC_CONTR |= adc_chs;
    ADC_CONTR |= 0x40;                      //启动AD转换
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20));            //查询ADC完成标志
    ADC_CONTR &= ~0x20;                     //清完成标志
    return (int)(battery_voltage / 1024L * ((ADC_RES << 8) | ADC_RESL));
}

int adc_get_battery_electricity()
{
    adc_get_voltage(ADC_1_19);
    return (int)((battery_voltage - 3000) * 100.0 / 1200 + 0.5);     // 3V截止    
}

