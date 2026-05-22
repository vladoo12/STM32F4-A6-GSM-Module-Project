#include "a6_driver.h"

#define GPIOAEN			(0x1 << 0)

#define UART2EN			(0x1 << 17)
#define UART1EN			(0x1 << 4)

#define CR1_UE			(0x1 << 13)
#define CR1_RE			(0x1 << 2)
#define CR1_TE			(0x1 << 3)
#define CR1_RXNEIE		(0x1 << 5)

#define SR_TXE 			(0x1 << 7)

#define UART_BAUDRATE 	115200
#define SYS_FREQ		16000000

//systick m4 cortec ug
#define CTRL_ENABLE 	 (0x1 << 0)
#define CTRL_CLKSRC		 (0x1 << 2)
#define CTRL_COUNTFLAG	 (0x1 << 16)
#define SYSTICK_LOAD_VAL 16000 		//because our HSI is 16MHz/16KHz



static uint16_t compute_urat_bd(uint32_t peripher_clk, uint32_t baudrate);
void debug_uart_write(int ch);
void a6_uart_write_char(char ch);
void systick_delay_ms(uint32_t delay);

int  __io_putchar(int ch)
{
	debug_uart_write(ch);
	return ch;

}

void debug_uart_init(void)
{

	/*1. Enable clock access to UART pins GPIO port(GPIOA)*/

	RCC -> AHB1ENR |= GPIOAEN;
	/*2.Set PA2 mode to alternate function mode */
	//MODER2 pin 5 and 4

	GPIOA -> MODER &= ~(0x1 << 4);
	GPIOA -> MODER |=  (0x1 << 5);
	/*3.Set PA3 mode to alternate function mode */

	//MODER3 pin 6 and 7
	GPIOA -> MODER &= ~(0x1 << 6);
	GPIOA -> MODER |=  (0x1 << 7);
	/*4.Set PA2 alternate function tzpe to UART2_TX (AF07)*/
	//we need to set pa2 to af7 (0111)
	GPIOA -> AFR[0] &= ~(0xF << 8);
	GPIOA -> AFR[0] |= (0x7 <<8);

	/*5.Set PA3 alterante function tzpe to UART2_RX (AF07)*/
	GPIOA -> AFR[0] &= ~(0xF << 12);
	GPIOA -> AFR[0] |= (0x7 <<12);

	//Configure UART module
	//6. Enable clock access to the UART module
	RCC -> APB1ENR|= UART2EN;

	//7. Disable UART mnodule
	USART2 -> CR1 &= ~(CR1_UE);
	//8. Set UART baudrate
	USART2 -> BRR = compute_urat_bd(SYS_FREQ, UART_BAUDRATE);
	//9. Set transfer direction
	USART2 -> CR1 = (CR1_TE | CR1_RE);
	//10. ENable UART module
	USART2 -> CR1 |= CR1_UE;


}



/*
 * UART Module: USART1
 * UART Pins  : PA9 = TX, PA10 = RX
 * A6 RX Pin  ==> PA9(TX)
 * A6 TX Pin  ==> PA10(RX)
 */


void a6uart_init(void)
{

	/*1. Enable clock access to UART pins GPIO port(GPIOA)*/

	RCC -> AHB1ENR |= GPIOAEN;

	/*2.Set PA9 mode to alternate function mode */

	GPIOA -> MODER &= ~(0x1 << 18);
	GPIOA -> MODER |=  (0x1 << 19);
	/*3.Set PA10 mode to alternate function mode */
	GPIOA -> MODER &= ~(0x1 << 20);
	GPIOA -> MODER |=  (0x1 << 21);

	/*4.Set PA9 alternate function tzpe to UART1_TX (AF07)*/
	GPIOA -> AFR[1] &= ~(0xF << 4);
	GPIOA -> AFR[1] |= (0x7 <<4);

	/*5.Set PA10 alterante function tzpe to UART1_RX (AF07)*/
	GPIOA -> AFR[1] &= ~(0xF << 8);
	GPIOA -> AFR[1] |=  (0x7 << 8);

	//Configure UART module
	//6. Enable clock access to the UART module
	RCC -> APB2ENR|= UART1EN;
	//7. Disable UART mnodule
	USART1 -> CR1 &= ~(CR1_UE);
	//8. Set UART baudrate
	USART1 -> BRR = compute_urat_bd(SYS_FREQ, UART_BAUDRATE);
	//9. Set transfer direction
	USART1 -> CR1 = (CR1_TE | CR1_RE);
	//10. Enable RXNEIE (for interrupt)
	USART1 -> CR1 |= CR1_RXNEIE;
	//11. ENable UART module
	USART1 -> CR1 |= CR1_UE;


}


void a6_uart_write_char(char ch)
{

	//1.Make sure that trasmite data register is empy
	while (!(USART1 -> SR & SR_TXE)) {}
	//2.Write to transmite data register
	USART1 -> DR = (ch & 0xFF);

}



void debug_uart_write(int ch)
{
	//1.Make sure that trasmite data register is empy
	while (!(USART2 -> SR & SR_TXE)) {}
	//2.Write to transmite data register
	USART2 -> DR = (ch & 0xFF);


}



static uint16_t compute_urat_bd(uint32_t peripher_clk, uint32_t baudrate)
{

	return ((peripher_clk + baudrate/2U)/baudrate);

}


void systick_delay_ms(uint32_t delay)
{
	//1.Reload with number of clocks per ms
	SysTick -> LOAD = SYSTICK_LOAD_VAL;

	//2.Clear systick current value register
	SysTick -> VAL = 0;

	//3.Enable systick and select internal clock source
	SysTick -> CTRL = (CTRL_CLKSRC | CTRL_ENABLE);

	for(int i = 0; i < delay; i++){

		while((SysTick -> CTRL & CTRL_COUNTFLAG) == 0) {}

	}

	SysTick-> CTRL = 0;

}







