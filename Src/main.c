#include "led.h"
#include "uart.h"
#include "timebase.h"

#define TRUE 1U
#define FALSE 0U

uint8_t flag = FALSE;

int main(void){

	led_init();
	uart_tx_init();
	timebase_init();
	led_off();
	while(1){

		delay(1);
		printf("A second just occurred!!...........................\n\r");
		if(flag == FALSE){
			led_on();
			flag = TRUE;
		}
		else{
			led_off();
			flag = FALSE;
		}

	}

}
