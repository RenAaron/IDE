#include "DCHelpers.h"
#include "stepperHelpers.h"
#include "uart.h"
#include "camera.h"
#include "i2c.h"
#include "oled.h"
#include <stdlib.h>

unsigned char OLED_TEXT_ARR[1024];
unsigned char OLED_GRAPH_ARR[1024];

int index_sum;
int index_length;
int turn_index;

void TIMG6_IRQHandler(){
	if(GPIOA->DOUT31_0 & GPIO_DOUTSET31_0_DIO12_SET){         //if clock is high 
		GPIOA->DOUTCLR31_0 = GPIO_DOUTCLR31_0_DIO12_CLR;     //clear clock 
	}
					
	GPIOA->DOUTSET31_0 = GPIO_DOUTSET31_0_DIO28_SET; //set SI high 
	GPIOA->DOUTSET31_0 = GPIO_DOUTSET31_0_DIO12_SET; //set CLK high  
					 
	GPIOA->DOUTCLR31_0 = GPIO_DOUTSET31_0_DIO28_SET; //set SI low 
	GPIOA->DOUTCLR31_0 = GPIO_DOUTSET31_0_DIO12_SET; //set CLK low 
	
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED; //enable TIMG0
}


void TIMG0_IRQHandler(){
	GPIOA->DOUTTGL31_0 |= GPIO_DOUTTGL31_0_DIO12_TOGGLE;    //set clock to go high if it's low and low if it's high
	
	if(GPIOA->DOUT31_0 & GPIO_DOUTSET31_0_DIO12_SET){       //if clk high 
		if(pixelCounter <= 127){
			cameraData[pixelCounter] = ADC0_getVal();           //add value to buffer
			pixelCounter++;
		} else{
			pixelCounter = 0;                 //reset index 
			cameraData_complete = 1;     //set done flag
			TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_ENABLED; //STOP TIMG6 
		}
	}
}

void adjustSpeed(int turnIndex){
	int delta = abs(turnIndex - 64);
	/* gives range of 25 - 40 PWM 	 high	low*/
	int newSpeed = (1 - (delta/64.0))*10 + 30;
	
	// UART0_put((uint8_t *)"\r\n");
	// UART0_printDec(newSpeed);
	
	driveForward(newSpeed);
}

int main(){  
	UART0_init();
	TIMG6_init(32000, 0); // init millisecond counter 
	
	setLeftEnable(); 
	setRightEnable();
	initMotorTimers();

	initStepper();

	ADC0_init();
	Camera_init();
	OLED_Init();

	/* reset display */
	OLED_display_on();
	OLED_display_clear();
	OLED_display_on();

	// driveForward(30);

	while(1){
		/* read camera data */
		while(Camera_isDataReady()){
//			OLED_DisplayCameraData(cameraData);
			
			index_sum = 0;
			index_length = 0;
			
			/* camera center loop */
			for(int i = 0; i < 127; i++){        // loop through all camera data
				if(Camera_getData()[i] >= 2000){
					index_sum += i;
					index_length ++;
				}//get value in array
			}
			
			turn_index = index_sum/index_length;
//			UART0_put((uint8_t *)"Turn Index: ");
//			UART0_printDec(turn_index);
//			UART0_put((uint8_t *)"------------------------\r\n");

			/* servo and DC motor control */
			if(turn_index != 0){	// if the turn index passes the threshold from camera center loop
				turnTowardIndex(turn_index);
				adjustSpeed(turn_index);
			} else{
				driveForward(0);	//carpet stop
			}
		}
	}

	/*  */
//	while(1){
//		
//		for(int i = 0; i < 127; i++){
//			turnTowardIndex(i);
//		}
//		
//		for(int i = 127; i > 0; i--){
//			turnTowardIndex(i);
//		}
//			
//	}
	
	/* stepper motor control */
//	while(1){
//		TIMA1_PWM_DutyCycle(4, center_duty);
//		TIMA1_PWM_DutyCycle(4, left_duty);
//		TIMA1_PWM_DutyCycle(4, right_duty);
//	}
//	
//	/* DC motor control */
//	while(1){
//		driveForward(30);	//

//		driveForward(0);	
//	}
							
	return 0;
    
}