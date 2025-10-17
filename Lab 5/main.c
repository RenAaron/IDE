#include "switches.h"
#include "leds.h"
#include "uart.h"
#include "timers.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "adc12.h"
#include "camera.h"

void GROUP1_IRQHandler(){ 
    
}

void TIMG0_IRQHandler(){
    // GPIOA->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO12_SET; 
        // GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;
    
        GPIOA->DOUTTGL31_0 |= GPIO_DOUTTGL31_0_DIO12_TOGGLE;
    
    if(GPIOA->DOUT31_0 & GPIO_DOUTSET31_0_DIO12_SET){ // if clk high 
        if(pixelCounter <= 127){
            cameraData[pixelCounter] = ADC0_getVal(); // add value to buffer
            pixelCounter++;
        } else{
            pixelCounter = 0; // reset index 
            cameraData_complete = 1; // set done flag
            TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_ENABLED; // STOP TIMG6 
        }
    }    
}

void TIMG6_IRQHandler(){
    
        if(GPIOA->DOUT31_0 & GPIO_DOUTSET31_0_DIO12_SET){ // if clock is high 
            GPIOA->DOUTCLR31_0 = GPIO_DOUTCLR31_0_DIO12_CLR; // clear clock 
        }
        
    GPIOA->DOUTSET31_0 = GPIO_DOUTSET31_0_DIO28_SET; // set SI high 
    GPIOA->DOUTSET31_0 = GPIO_DOUTSET31_0_DIO12_SET; // set CLK high  
         
    GPIOA->DOUTCLR31_0 = GPIO_DOUTSET31_0_DIO28_SET; // set SI low 
    GPIOA->DOUTCLR31_0 = GPIO_DOUTSET31_0_DIO12_SET; // set CLK low 
    
    TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED; // enable TIMG0
    
}

void TIMG12_IRQHandler(){
    
    
}

int main(){
    
    UART0_init();
    
    ADC0_init();
    
    Camera_init();
    
    while(1){
        while(Camera_isDataReady()){
            UART0_put((uint8_t *)"-1\r\n");
            
            for(int i = 0; i < 128; i++){
                UART0_printDec(Camera_getData()[i]);
                UART0_put((uint8_t *)"\r\n");
            }
            UART0_put((uint8_t *)"-2\r\n");
        }
    }

    return 0;
}
