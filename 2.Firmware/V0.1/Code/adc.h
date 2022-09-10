#ifndef __ADC_H__
#define __ADC_H__

enum ADC_CHS
{
//    ADC_0 = 0x00,
//    ADC_1,
//    ADC_2,
//    ADC_3,
//    ADC_4,    
    ADC_5 = 0x05,
    ADC_6,
//    ADC_7,
//    ADC_8,
//    ADC_9,
//    ADC_10,
//    ADC_11,
//    ADC_12,
//    ADC_13,
//    ADC_14,
    ADC_1_19 = 0x0F
};

enum ENV
{
    DAYTIME ,   // °×Ìì ²»ÁÁµÆ
    EVENING ,   // °øÍí
    NIGHT       // Ò¹Àï
};
extern enum ENV env;

enum BATTERY
{
    NO = 0 ,
    BAT
};
extern enum BATTERY battery;

void adc_init(void);
int adc_get_voltage(enum ADC_CHS adc_chs);
int adc_get_battery_electricity();


#endif