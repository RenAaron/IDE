/**
 * ******************************************************************************
 * @file    : timers.c
 * @brief   : Timers module header file
 * @details : Timers initialization and interaction
 * 
 * @author  : Fariha Khan & RenAaron Ellis 
 * @date 		: 10/5/2025
 * ******************************************************************************
*/

#include "timers.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_gpio.h" 

int get_load(float period, uint32_t prescaler){
	return 32000000/((1/period)*prescaler);
}

void TIMG0_init(uint32_t load, uint32_t prescaler){
	if (!(TIMG0->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) { // if TIMG6 is not powered on 
		TIMG0->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT); // reset TIMG6 
		TIMG0->GPRCM.PWREN |= (GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE); // power on TIMG6
  }
	
	TIMG0->COMMONREGS.CCLKCTL &= ~GPTIMER_CCLKCTL_CLKEN_ENABLED; // disable clock timer 
	TIMG0->COUNTERREGS.CTRCTL &= 0;	  // stop the timer
	
	TIMG0->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE; 					// enable busclk 
	
	TIMG0->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1; 						// (?) are we actually doing this here? 
	 
	TIMG0->COMMONREGS.CPS = prescaler; 													// ASK DR SOLDIVINI, I AM CONFUSED
	
	TIMG0->COUNTERREGS.LOAD = load;
	
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;
	
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1; // enable down-counting 
	
	TIMG0->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED; // enable clock timer 
	
	__disable_irq(); 																						// Disable interrupts
	TIMG0->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK; 		// Clear the �zero event� interrupt 
	TIMG0->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_MASK;			// Enable the �zero event� interrup
	
	NVIC_EnableIRQ(TIMG0_INT_IRQn);															// Register the interrupt with the NVIC
	__enable_irq(); 																						// Enable interrupts
	
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;	  // Start (enable) the timer
	
	
	
}


void TIMG6_init(uint32_t load, uint32_t prescaler){
	
	if (!(TIMG6->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) { // if TIMG6 is not powered on 
		TIMG6->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT); // reset TIMG6 
		TIMG6->GPRCM.PWREN |= (GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE); // power on TIMG6
  }
	
	TIMG6->COMMONREGS.CCLKCTL &= ~GPTIMER_CCLKCTL_CLKEN_ENABLED; // disable clock timer 
	TIMG6->COUNTERREGS.CTRCTL &= 0;	  // stop the timer
	
	TIMG6->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE; 					// enable busclk 
	
	TIMG6->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1; 						// (?) are we actually doing this here? 
	 
	TIMG6->COMMONREGS.CPS = prescaler; 													// ASK DR SOLDIVINI, I AM CONFUSED
	
	TIMG6->COUNTERREGS.LOAD = load;
	
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;
	
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1; // enable down-counting 
	
	TIMG6->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED; // enable clock timer 
	
	__disable_irq(); 																						// Disable interrupts
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK; 		// Clear the �zero event� interrupt 
	TIMG6->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_MASK;			// Enable the �zero event� interrup
	
	NVIC_EnableIRQ(TIMG6_INT_IRQn);															// Register the interrupt with the NVIC
	__enable_irq(); 																						// Enable interrupts
	
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;	  // Start (enable) the timer
}

void TIMG12_init(uint32_t load){
    if (!(TIMG12->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) { // if TIMG12 is not powered on 
        TIMG12->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT); // reset TIMG12
        TIMG12->GPRCM.PWREN |= (GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE); // power on TIMG12
  }
    
    TIMG12->COMMONREGS.CCLKCTL &= ~GPTIMER_CCLKCTL_CLKEN_ENABLED; // disable clock timer 
    TIMG12->COUNTERREGS.CTRCTL &= 0;      // stop the timer
    
    TIMG12->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;                 // enable busclk 
    
    TIMG12->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1;                         // (?) are we actually doing this here? 
    
    TIMG12->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED; // enable clock timer 
    
    TIMG12->COUNTERREGS.LOAD = load;
    
    TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CM_DOWN;                 // enable down-counting 
    
    TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_MASK;             // Counter Value after EnablCounter Value after Enabl
    
    TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_REPEAT_REPEAT_1;// periodic mode, do we have to shift ? 
    
    __disable_irq();                                                                                             // Disable interrupts
    TIMG12->CPU_INT.IMASK |= GPTIMER_GEN_EVENT1_IMASK_Z_CLR;             // Clear the �zero event� interrupt 
    TIMG12->CPU_INT.IMASK |= GPTIMER_GEN_EVENT1_IMASK_Z_SET;            // Enable the �zero event� interrup
    TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;            // Start (enable) the timer
    NVIC_EnableIRQ(TIMG12_INT_IRQn);                                                            // Register the interrupt with the NVIC
    __enable_irq(); 
}