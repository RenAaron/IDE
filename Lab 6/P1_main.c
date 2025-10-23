#include "timers.h"
#include "uart.h"

int cont = 0;

void wait(){
	cont = 0;
	while(!cont){}
}

void TIMG6_IRQHandler(){
	cont = 1;
}

int main(){
	UART0_init();
	TIMG6_init(32000, 100);
	
	TIMA0_PWM_init(8, 3200, 0, 20);
	TIMA0_PWM_init(12, 3200, 0, 20);
	UART0_init();
	
	while(1){
		for(int i = 0; i <= 100; i++){
			UART0_printDec(i);
			UART0_put((uint8_t *)"\r\n");
			TIMA0_PWM_DutyCycle(8, i);
			wait();
		}
		
		for(int i = 100; i >= 0; i--){
			UART0_printDec(i);
			UART0_put((uint8_t *)"\r\n");
			TIMA0_PWM_DutyCycle(8, i);
			wait();
		}
		

		TIMA0_PWM_DutyCycle(8, 1);
		
		for(int i = 0; i <= 100; i++){
			UART0_printDec(i);
			UART0_put((uint8_t *)"\r\n");
			TIMA0_PWM_DutyCycle(12, i);
			wait();
		}
		
		for(int i = 100; i >= 0; i--){
			UART0_printDec(i);
			UART0_put((uint8_t *)"\r\n");
			TIMA0_PWM_DutyCycle(12, i);
			wait();
		}
		
		TIMA0_PWM_DutyCycle(12, 1);
	}
		
	return 0;
	
}