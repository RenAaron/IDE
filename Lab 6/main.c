#include "timers.h"

int main(){
	TIMA0_PWM_init(8, 3200, 0, 20);
	// TIMA1_PWM_init(4, 3200, 0, 20);
	
	while(1){}
		
	return 0;
	
}