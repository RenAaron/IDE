#include "switches.h"
#include "leds.h"
#include "uart.h"
#include "timers.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "adc12.h"

int pressed_1 = 0;
int pressed_2 = 0;
int ms_1 = 0;
int ms_2 = 0;
int color_counter = 0;

COLOR c_color[] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};

void GROUP1_IRQHandler(){
    
    if (S2_pressed()){
        
        if(pressed_2 == 1){
            UART0_put((uint8_t *)"(tim2) Timed: ");
            UART0_printDec(ms_2);
            UART0_put((uint8_t *)" ms");
            UART0_putchar('\n');
            UART0_putchar('\r');
            pressed_2 = 0;
            ms_2 = 0;
            LED2_set(CLEAR);
        } else{
            pressed_2 = 1;
            LED2_set(CLEAR);
            LED2_set(c_color[color_counter % 7]);
            color_counter++;
        }
        
    }
    
    if(S1_pressed()){
        if(pressed_1 == 1){
            UART0_put((uint8_t *)"(tim1) Timed: ");
            UART0_printDec(ms_1);
            UART0_put((uint8_t *)" ms");
            UART0_putchar('\n');
            UART0_putchar('\r');
            pressed_1 = 0;
            ms_1 = 0;
            LED1_set(OFF);
        } else{
                pressed_1 = 1;
        }
    } 
    
}

void TIMG6_IRQHandler(){
    
				uint32_t adc_val = ADC0_getVal();
        // float p_val = ((3300*adc_val)/4096.0 - 500)/10; 
        UART0_printDec(adc_val);
        UART0_putchar('\n');
        UART0_putchar('\r');
}

void TIMG12_IRQHandler(){
    if(pressed_1){
        ms_1++;
    }
    
    if(pressed_2){
        ms_2++;
    }
    
}

int main(){
    
    UART0_init();
  LED1_init();
  LED2_init();
    
  UART0_putchar('\n');
  UART0_putchar('\r');
    
  TIMG6_init(62000, GPTIMER_CPSV_CPSVAL_MAXIMUM);
  TIMG12_init(32000);
  S1_init_interrupt();
  S2_init_interrupt();
    
    ADC0_init();
    
    while(1){
        
    }
    
    /*
    while(1){
        UART0_printDec((int) ADC0_getVal);
        // UART0_printDec(72);
        UART0_putchar('\n');
        UART0_putchar('\r');
    }
    
    */

    return 0;
}