/*
 * timebase.c
 *
 *  Created on: Jan 21, 2023
 *      Author: ravisha96
 */
#include "timebase.h"
#include "stm32f4xx.h"

#define ONE_SEC_LOAD	 16000000

#define CTRL_ENABLE		 (1U<<0)
#define CTRL_TICKINT 	 (1U<<1)
#define CTRL_CLKSRC 	 (1U<<2)
#define CTRL_COUNTFLAG 	 (1U<<16)

#define MAX_DELAY		0XFFFFFFFFU

volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;
volatile uint32_t tick_freq = 1;

// called whenever a systick timeout occurs
// every second 1 is added to global current tick value
void tick_increment(void){
	g_curr_tick += tick_freq;
}

uint32_t get_tick(void){
	__disable_irq();

	g_curr_tick_p = g_curr_tick;

	__enable_irq();

	return g_curr_tick_p;

}

// delay in seconds
void delay(uint32_t delay){

	uint32_t tickstart = get_tick();
	uint32_t wait = delay;
	if(wait < MAX_DELAY){
		wait += (uint32_t)(tick_freq);
	}
	while(get_tick() - tickstart < wait){}
}

void timebase_init(void){
	/*
	 * 1. Reload the timer with number of cycles for a second
	 * 2. Clear systick current value register
	 * 3. Enable systick and select internal clock source
	 * 4. Enable interrupt
	 * 5. Enable systick
	 * 6. Enable global interrupt
	 * */

	SysTick->LOAD = ONE_SEC_LOAD -1;

	SysTick->VAL = 0;


	SysTick->CTRL = CTRL_CLKSRC;

	SysTick->CTRL |= CTRL_TICKINT;

	SysTick->CTRL |= CTRL_ENABLE;

	__enable_irq();

}

void SysTick_Handler(void){
	tick_increment();
}
