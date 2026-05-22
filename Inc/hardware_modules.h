#ifndef __HARDWARE_MODULES_H__
#define __HARDWARE_MODULES_H__
#include "stdbool.h"
#include <stdint.h>
#include "stm32f4xx.h"



void light_init(void);
void light_on(void);
void light_off(void);

void button_init(void);
bool get_btn_state(void);

void pa1_adc_init(void);
uint32_t adc_read(void);
void start_conversion(void);



#endif
