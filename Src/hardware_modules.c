#include "stdbool.h"
#include "hardware_modules.h"
#include "stm32f4xx.h"
#include "stdint.h"


#define GPIOCEN		(0x01 << 2 )
#define GPIOAEN		(0x01 << 0 )
#define BTN_PIN		(0x01 << 13)
#define ADC1EN 		(0x01 << 8)

#define ADC_CH1				(0x01 << 0)
#define ADC_SEQ_LEN_1 		 0x00
#define CR2_ADC_ON			(0x01 << 0)
#define CR2_CONT			(0x01 << 1)
#define CR2_SWSTART			(0x01 << 30)
#define SR_EOC				(0x01 << 1)

void light_init(void)
{
	//Enable clock access to GPIOA

	RCC->AHB1ENR |= GPIOAEN;
	//Set PA5 as output
	GPIOA->MODER |= (0x01 << 10);
	GPIOA->MODER &= ~(0x01 << 11);
}



void light_on(void)
{
	//Set PA5 output to high
	GPIOA->ODR |= (0x01 << 5);
}


void light_off(void)
{
	//Set PA5 output to low
	GPIOA->ODR &= ~(0x01 << 5);

}



void button_init(void)
{
	//Enable clock access to GPIOC
	//Set PC13 to input
	RCC ->AHB1ENR |= GPIOCEN;

	//Set PC13 to input
	GPIOC -> MODER &= ~ (0x01 << 26);
	GPIOC -> MODER &= ~ (0x01 << 27);

}

bool get_btn_state(void)
{
	//Push button is in active low, by default is high
	//Check if BTN is pressed
	if(GPIOC->IDR & BTN_PIN)
	{
		return false;
	}
	else
	{
		return true;
	}


}

void pa1_adc_init(void)
{
	//Configure the ADC GPIO pin//
	//Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;
	//Set the mode of PA1 to analog
	GPIOA -> MODER |= (0x01 << 2);
	GPIOA -> MODER |= (0x01 << 3);


	//Configure the ADC module//
	//Enable clock access to ADC module
	RCC->APB2ENR |= ADC1EN;
	//Conversion sequence start
	ADC1->SQR3 |= ADC_CH1;
	//Conversion sequence length
	ADC1->SQR1 |= ADC_SEQ_LEN_1;
	//Enable ADC module
	ADC1 -> CR2 |= CR2_ADC_ON;
}

void start_conversion(void)
{
	//Enable continouse conversion
	ADC1->CR2 |= CR2_CONT;
	//Start ADC conversion
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void)
{
	//Wait for conversion to be complete
	while(!(ADC1->SR & SR_EOC)){};

	//Read converted reasults
	return (ADC1->DR);

}




