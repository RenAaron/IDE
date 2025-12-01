#include "DCHelpers.h"
#include "stepperHelpers.h"
#include "uart.h"
#include "camera.h"
#include "i2c.h"
#include "oled.h"
#include <stdlib.h>

unsigned char OLED_TEXT_ARR[1024];
unsigned char OLED_GRAPH_ARR[1024];

/* DUTY CYCLE VARIABLES */
int index_sum;
int index_length;
int turn_index;

/* PID CONTROL */
//position variables
double PositionOld = 64.0;	//start at center position so that car starts centered
double PositionActual = 0.0;
double PositionDesired = 64.0;	//center index value
double Position = 0.0;

//error variables
double Err = 0.0;
double ErrOld1 = 0.0;
double ErrOld2 = 0.0;

//constant values for PID - should all be between 0 and 1
double Kp = 1.0;
double Ki = 0.1;	//0.1 ??
double Kd = 0.00001;	//0.00001 ??

void TIMG6_IRQHandler(){
	if(GPIOA->DOUT31_0 & GPIO_DOUTSET31_0_DIO12_SET){      //if clock is high 
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
	/* gives range of 25 - 35 PWM 	 high	low*/
	int newSpeed = (1 - (delta/64.0))*10 + 25;
	
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
	
	while(1){
		/* read camera data */
		while(Camera_isDataReady()){
//			OLED_DisplayCameraData(cameraData);
			
			index_sum = 0;
			index_length = 0;
			
			/* camera center loop */
			for(int i = 0; i < 127; i++){        // loop through all camera data
				if(Camera_getData()[i] >= 3000){
					index_sum += i;
					index_length ++;
				}//get value in array
			}
			
			turn_index = index_sum/index_length;
			
			/* PID control */
			PositionActual = turn_index; //index position from camera
			Err = PositionActual - PositionDesired;	//negative error = too far left, positive error = too far right (-64, +64)
			ErrOld2 = Kp * (Err - ErrOld1);
			Position = PositionOld +
				Kp * (Err - ErrOld1) +		//proportional control
				Ki * (Err - ErrOld1)/2.0 +	//integral control
				Kd * (Err - 2.0 * ErrOld1 + ErrOld2);	//derivative control
			PositionOld = Position;
			ErrOld2 = ErrOld1;
			ErrOld1 = Err;
			
			// UART0_put((uint8_t *)"\r\n");
			// UART0_printDec(Err);
			
			turnTowardIndex(Position);
			
			/* DC motor control */
			if(turn_index != 0 && index_length < 100){	// if the turn index passes the threshold from camera center loop
				adjustSpeed(turn_index);
				turnTowardIndex(Position);
			} else if(index_length > 100) {
				adjustSpeed(turn_index);
			}else{
				driveForward(0);	//carpet stop
			}
		}
	}

	
	return 0;
}
