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

void changePhase(int phase){
	switch(phase){
		case 0:
			UART0_put((uint8_t *) "0 \r \n");
			TIMA0_PWM_DutyCycle(8, 100);
			TIMA0_PWM_DutyCycle(12, 100);
			TIMA0_PWM_DutyCycle(17, 0);
			TIMA0_PWM_DutyCycle(13, 0);
			break;
		
		case 1:
			UART0_put((uint8_t *) "1 \r \n");
			TIMA0_PWM_DutyCycle(8, 0);
			TIMA0_PWM_DutyCycle(12, 100);
			TIMA0_PWM_DutyCycle(17, 100);
			TIMA0_PWM_DutyCycle(13, 0);
			break;
		
		case 2:
			UART0_put((uint8_t *) "2 \r \n");
			TIMA0_PWM_DutyCycle(8, 0);
			TIMA0_PWM_DutyCycle(12, 0);
			TIMA0_PWM_DutyCycle(17, 100);
			TIMA0_PWM_DutyCycle(13, 100);
			break;
		
		case 3:
			UART0_put((uint8_t *) "3 \r \n");
			TIMA0_PWM_DutyCycle(8, 100);
			TIMA0_PWM_DutyCycle(12, 0);
			TIMA0_PWM_DutyCycle(17, 0);
			TIMA0_PWM_DutyCycle(13, 100);
			break;
		
		default:
			break;
	}
}

int main(){
	UART0_init();
	
	TIMG6_init(62000, 255); // wait timer 
	
	TIMA0_PWM_init(8, 3200, 0, 0); // PB8, A
	TIMA0_PWM_init(12, 3200, 0, 0); // PB12, B 
	TIMA0_PWM_init(17, 3200, 0, 0); // PB17, C 
	TIMA0_PWM_init(13, 3200, 0, 0); // PB13, D
	
	int forward = 1;
	int phase = 0;
	
	// int forward = 0;
	// int phase = 3;
	
	while(1){
		if(forward){
			changePhase(phase);
			if(phase < 3){
				phase++;
			} else{
				phase = 0;
			}
		} else{
			changePhase(phase);
			if(phase > 0){
				phase--;
			} else{
				phase = 3;
			}
			
		}
	}
		
	return 0;
	
}