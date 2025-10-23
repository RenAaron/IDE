#include "timers.h"
#include "uart.h"
#include "leds.h"

int cont = 0;

double center_duty = 6;
double clockwise_duty = 1;
double counter_clockwise_duty = 11;

void wait(){
	cont = 0;
	while(!cont){}
}

void TIMG6_IRQHandler(){
	cont = 1;
}

int main(){
	UART0_init();
	TIMG6_init(63000, 255); // arbitrary wait 
	TIMA1_PWM_init(4, 2510, 255, center_duty);
	UART0_init();
	LED2_init();
	
	while(1){
		
		LED2_set(MAGENTA);
		wait();
		LED2_set(CLEAR);
		
		TIMA1_PWM_DutyCycle(4, center_duty); // center (1.5ms pulse) 
		wait();
		wait();
		wait();
		wait();
		TIMA1_PWM_DutyCycle(4, clockwise_duty); // full clockwise (1ms pulse) 
		wait();
		wait();
		wait();
		wait();
		TIMA1_PWM_DutyCycle(4, counter_clockwise_duty); // full counter-clockwise (2ms pulse) 
		wait();
		wait();
		wait();
		wait();
		
	}
		
	return 0;
	
}