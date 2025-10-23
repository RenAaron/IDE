#include "timers.h"
#include "uart.h"

void wait(){
	for(volatile long int i = 0; i < 20000; i++){
	
	}
}

int main(){
	
	TIMA0_PWM_init(8, 3200, 0, 20);
	TIMA0_PWM_init(12, 3200, 0, 20);
	TIMA0_PWM_init(17, 3200, 0, 20);
	TIMA0_PWM_init(13, 3200, 0, 20);
	
	int forward = 1;
	int phase = 0;
	
	UART0_init();
	
	if(forward){
		
	}
	
}