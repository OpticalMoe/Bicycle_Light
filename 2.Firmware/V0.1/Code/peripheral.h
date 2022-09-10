#ifndef __PERIPHERAL_H__
#define __PERIPHERAL_H__

enum POWER
{
    IDLE = 0x01,
    SLEEP ,
    RUN ,
    WAKE 
};
extern enum POWER power;

//enum BIKE
//{
//    BREAK = 0,      // É²³µ
//    DRIVING    // ÐÐ³µ
//};

//extern enum BIKE bike;

#define KEY_LEFT    P32 //INT0  KEY_1
#define KEY_RIGHT   P55 //NULL

#define CONCUSSION_START    P33     //INT1
#define CONCUSSION_BREAK    P34     //T0
#define HELL_1  P37     //INT3
#define HELL_2  P36     //INT2

#define LED_G   P17
#define LED_B   P35

void per_usart_init(void);
void per_io_init(void);
void per_int_init(void);
void per_power(enum POWER _power);
void reset_state(void);

#endif
