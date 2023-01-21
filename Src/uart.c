
#include "stm32f4xx.h"
#include "uart.h"
#include <stdint.h>

#define GPIOAEN		(1u<<0)
#define UART2EN		(1u<<17)

#define SYS_FREQ 	16000000
#define APB1_CLK		SYS_FREQ
#define UART_BAUDRATE 	115200

#define CR1_TE 		(1u<<3)
#define CR1_UE		(1u<<13)

#define SR_TXE		(1u<<7)


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void uart_write(int ch);


int __io_putchar(int ch){
	uart_write(ch);
	return ch;
}
void uart_tx_init(void){
	/*
	 * 1. Enable clock to GPIOA
	 * 2. configure mode of gpioa pins PA2 to AF mode
	 * 3. Set AF Type as AF7
	 * 4. Enable clock to usart2 peripheral
	 * 5. Configure baud rate
	 * 6. Configure transfer direction (tx/rx)
	 * 7. Enable UART module
	 */
	RCC->AHB1ENR |= GPIOAEN;


	GPIOA->MODER &= ~(1u<<4);
	GPIOA->MODER |= (1u<<5);

	GPIOA->AFR[0] |= (1u<<8);
	GPIOA->AFR[0] |= (1u<<9);
	GPIOA->AFR[0] |= (1u<<10);
	GPIOA->AFR[0] &= ~(1u<<11);

	RCC->APB1ENR |= UART2EN;

	uart_set_baudrate(APB1_CLK, UART_BAUDRATE);

	USART2->CR1 = CR1_TE;

	USART2->CR1 |= CR1_UE;
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate){
	USART2->BRR = compute_uart_bd(periph_clk,baudrate);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate){

	return (periph_clk+(baudrate/2u))/baudrate;
}

static void uart_write(int ch){
	while(!(USART2->SR & SR_TXE)){}

	USART2->DR = (ch  & 0xFF);
}


