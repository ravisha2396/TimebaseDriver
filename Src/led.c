#include "led.h"

#define GPIOAEN		(1u<<0)
#define LED_PIN		(1u<<5)

void led_init(){

	/*  USER LED INITIALIZE SECTION
	 *  1. Enable clock to the GPIO A
	 *  2. Set led pin as output pin
	 */
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1u<<10);
	GPIOA->MODER &= ~(1u<<11);
}

void led_on(){

	/*
	 * Set LED pin high
	 */
	GPIOA->ODR |= LED_PIN;

}

void led_off(){

	/*
	 * Set LED pin low
	 */
	GPIOA->ODR &= ~LED_PIN;

}
