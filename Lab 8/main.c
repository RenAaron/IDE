/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : main source file
 * @details : reads analog output values from bandpass filter and converts to them 
 * 						to digital values
 * 
 * @author RenAaron Ellis and Fariha Khan
 * @date 10/06/2025
 * ******************************************************************************
*/

#include "uart.h"
#include "timers.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "adc12.h"
#include "uart_extras.h"

static uint16_t ADC_val;
int period = 0;
int peak_flag = 0;
int bpm = 0;

int thresh = 100;

void TIMG6_IRQHandler(){
	/* store ADC value in variable */
	ADC_val = ADC0_getVal();
	
	if(peak_flag == 2 || peak_flag == 3){
		period ++;
	}
	
	
	/* check for when adc value is 160 (rising) and then 160 (falling) and set flag for each stage */
	switch(peak_flag){
		case 0: // look for first rising 
			
			if(ADC_val > thresh + 20){ // rising edge 
				peak_flag = 1;
				// UART0_put((uint8_t *)"\n \r first rising ");
			}
			
			break;
		
		case 1: // look for first falling, start timing 
			if(ADC_val < thresh - 20){ // falling edge 
				peak_flag = 2;
				// UART0_put((uint8_t *)"\n \r first falling ");
			}
			
			break;
		
		case 2: // look for second rising
			if(ADC_val > thresh + 20){ // rising edge 
				peak_flag = 3;
				// UART0_put((uint8_t *)"\n \r second rising");
			}
			
			break;
		
		case 3: // look for second falling 
			if(ADC_val < thresh - 20){ // rising edge 
				peak_flag = 0;
				// UART0_put((uint8_t *)"\n \r second falling ");
				double freq = 1/(period/1000.0);
				UART0_put((uint8_t *)"Heartrate is ");
				UART0_printDec(freq * 60.0);
				UART0_put((uint8_t *)" BPM");
				UART0_put((uint8_t *)"\r\n");
				period = 0;
				
			}
			
			
			break;
		
		default:
			break;
		
	}
	
	
	
//	if(ADC_val >= 160 && peak_flag == 1){
//		peak_flag = 2;
//	}
//	
//	/* if rising edge flag is 2, start timer and stop at second falling edge */
//	if (peak_flag == 2) {
//		period ++;
//		
////		if((ms_counter % 1000) == 1){
////		}
//	}
//	//bpm = 1 cycle/second * 60 second/minute
//	bpm = (1/period)*(60);
}

void TIMG12_IRQHandler(){
	
}

int main(){
	UART0_init();
	ADC0_init();
	
	TIMG6_init(32000, 0);	//initialize timer for 1000 Hz (1 sample/ms)
	while(1) {
//		UART0_put((uint8_t *)"Heartrate is ");
//		UART0_printDec(bpm);
//		UART0_put((uint8_t *)" BPM");
//		UART0_put((uint8_t *)"\r\n");
//		
//		UART0_put((uint8_t *)"Period: ");
		// UART0_printDec(peak_flag);
		// UART0_put((uint8_t *)"\r\n");
//		
		// UART0_put((uint8_t *)"ADC Val: ");
		// UART0_printDec(ADC0_getVal());
		// UART0_put((uint8_t *)"\r\n");
	}
	return 0;
}