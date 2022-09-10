#ifndef __LIGHT_H__
#define __LIGHT_H__

enum LIGHT 
{
    TURN_OFF = 0,
    TURN_ON
};

void light_init(void);
void light_off(void);
void light_on(void);
void light(unsigned short number);
void light_running(enum LIGHT _switch);
unsigned short light_get_last_number(void);
void light_reset_last_number(void);

#endif
