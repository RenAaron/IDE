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


void TIMG0_init(uint32_t load, uint32_t prescaler){
	if (!(TIMG0->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) { // if TIMG6 is not powered on 
		TIMG0->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT); // reset TIMG6 
		TIMG0->GPRCM.PWREN |= (GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE); // power on TIMG6
  }
	
	TIMG0->COMMONREGS.CCLKCTL &= ~GPTIMER_CCLKCTL_CLKEN_ENABLED; // disable clock timer 
	TIMG0->COUNTERREGS.CTRCTL &= 0;	  // stop the timer
	
	TIMG0->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE; // enable busclk 
	
	TIMG0->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1;
	 
	TIMG0->COMMONREGS.CPS = prescaler;								
	
	TIMG0->COUNTERREGS.LOAD = load;
	
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;
	
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1; // enable down-counting 
	
	TIMG0->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED; // enable clock timer 
	
	__disable_irq(); 																						// Disable interrupts
	TIMG0->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK; 		// Clear the ?zero event? interrupt 
	TIMG0->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_MASK;			// Enable the ?zero event? interrup
	
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
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK; 		// Clear the ?zero event? interrupt 
	TIMG6->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_MASK;			// Enable the ?zero event? interrup
	
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
    TIMG12->CPU_INT.IMASK |= GPTIMER_GEN_EVENT1_IMASK_Z_CLR;             // Clear the ?zero event? interrupt 
    TIMG12->CPU_INT.IMASK |= GPTIMER_GEN_EVENT1_IMASK_Z_SET;            // Enable the ?zero event? interrup
    TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;            // Start (enable) the timer
    NVIC_EnableIRQ(TIMG12_INT_IRQn);                                                            // Register the interrupt with the NVIC
    __enable_irq(); 
}

void TIMA0_PWM_init(uint8_t pin, uint32_t period, uint32_t prescaler, double percentDutyCycle){
		if (!(TIMA0->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) { // if TIMG6 is not powered on 
			TIMA0->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT); // reset TIMG6 
			TIMA0->GPRCM.PWREN |= (GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE); // power on TIMG6
		}
	
		TIMA0->COMMONREGS.CCLKCTL &= ~GPTIMER_CCLKCTL_CLKEN_ENABLED; // disable clock timer 
		TIMA0->COUNTERREGS.CTRCTL &= 0;	  // stop the timer
		
		TIMA0->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE; // enable busclk 
		TIMA0->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1; // clock div by 1 
		TIMA0->COMMONREGS.CPS = prescaler;								
		TIMA0->COUNTERREGS.LOAD = period;
		TIMA0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;
		TIMA0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1; // enable down-counting 
		TIMA0->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED; // enable clock timer 
		
		// Edge-Aligned PWM Configuration
		
		switch(pin){
			case 8: // pin 8, channel 0, PINCM25
				TIMA0->COUNTERREGS.CC_01[0] = (uint32_t) ((1 - (percentDutyCycle)/100)*period); // set channel 0 duty cycle // RRR
			
				TIMA0->COUNTERREGS.CCCTL_01[0] |= GPTIMER_CCCTL_01_COC_COMPARE; // Set for compare mode
				TIMA0->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP0_OUTPUT; // Configure channel 0 for output 	
				TIMA0->COUNTERREGS.CCACT_01[0] |= GPTIMER_CCACT_01_LACT_CCP_HIGH; //set action on load to output high
				TIMA0->COUNTERREGS.CCACT_01[0] |= GPTIMER_CCACT_01_CDACT_CCP_LOW; //set action on compare(down) to output low
        TIMA0->COUNTERREGS.OCTL_01[0] |= GPTIMER_OCTL_01_CCPO_FUNCVAL; //set output to signal generator output
				TIMA0->COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP0_CCP_OUTPUT_OCTL; //enable corresponding CCP output (Counter n = channel 0, 1, etc)
				TIMA0->COUNTERREGS.OCTL_01[0] |= GPTIMER_OCTL_01_CCPOINV_NOINV; // no inversion 
				TIMA0->COUNTERREGS.OCTL_01[0] |= GPTIMER_OCTL_01_CCPIV_LOW; // keep output low while disabled 
			
				IOMUX->SECCFG.PINCM[IOMUX_PINCM25] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM25_PF_TIMA0_CCP0; // set PC connected for PB8
				IOMUX->SECCFG.PINCM[IOMUX_PINCM25] |= IOMUX_PINCM_DRV_DRVVAL1; // set to high drive strength PB8 
	
				GPIOB->DOESET31_0 |= GPIO_DOESET31_0_DIO8_SET;	//set hardware pin as outuput
				GPIOB->DOE31_0 |= GPIO_DOESET31_0_DIO8_SET;
				break;
			
			case 12: // pin 12, channel 1, PINCM29
				TIMA0->COUNTERREGS.CC_01[1] = (uint32_t) ((1 - (percentDutyCycle)/100)*period); 
			
				TIMA0->COUNTERREGS.CCCTL_01[1] |= GPTIMER_CCCTL_01_COC_COMPARE; 
				TIMA0->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP1_OUTPUT; 
				TIMA0->COUNTERREGS.CCACT_01[1] |= GPTIMER_CCACT_01_LACT_CCP_HIGH;
				TIMA0->COUNTERREGS.CCACT_01[1] |= GPTIMER_CCACT_01_CDACT_CCP_LOW;
        TIMA0->COUNTERREGS.OCTL_01[1] |= GPTIMER_OCTL_01_CCPO_FUNCVAL;
				TIMA0->COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP1_CCP_OUTPUT_OCTL;
				TIMA0->COUNTERREGS.OCTL_01[1] |= GPTIMER_OCTL_01_CCPOINV_NOINV;
				TIMA0->COUNTERREGS.OCTL_01[1] |= GPTIMER_OCTL_01_CCPIV_LOW;
			
				IOMUX->SECCFG.PINCM[IOMUX_PINCM29] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM29_PF_TIMA0_CCP1; 
				IOMUX->SECCFG.PINCM[IOMUX_PINCM29] |= IOMUX_PINCM_DRV_DRVVAL1;
	
				GPIOB->DOESET31_0 |= GPIO_DOESET31_0_DIO12_SET; 
				GPIOB->DOE31_0 |= GPIO_DOESET31_0_DIO12_SET;
				break;
			
			case 17: // pin 17, channel 2, PINCM17
				TIMA0->COUNTERREGS.CC_23[0] = (uint32_t) ((1 - (percentDutyCycle)/100)*period); 
			
				TIMA0->COUNTERREGS.CCCTL_23[0] |= GPTIMER_CCCTL_23_COC_COMPARE; 
				TIMA0->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP2_OUTPUT; 
				TIMA0->COUNTERREGS.CCACT_23[0] |= GPTIMER_CCACT_23_LACT_CCP_HIGH;
				TIMA0->COUNTERREGS.CCACT_23[0] |= GPTIMER_CCACT_23_CDACT_CCP_LOW;
        TIMA0->COUNTERREGS.OCTL_23[0] |= GPTIMER_OCTL_23_CCPO_FUNCVAL;
				TIMA0->COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP1_CCP_OUTPUT_OCTL;
				TIMA0->COUNTERREGS.OCTL_23[0] |= GPTIMER_OCTL_23_CCPOINV_NOINV;
				TIMA0->COUNTERREGS.OCTL_23[0] |= GPTIMER_OCTL_23_CCPIV_LOW;
			
				IOMUX->SECCFG.PINCM[IOMUX_PINCM43] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM43_PF_TIMA0_CCP2;
				IOMUX->SECCFG.PINCM[IOMUX_PINCM43] |= IOMUX_PINCM_DRV_DRVVAL1;
	
				GPIOB->DOESET31_0 |= GPIO_DOESET31_0_DIO17_SET;
				GPIOB->DOE31_0 |= GPIO_DOESET31_0_DIO17_SET;
				break;
			
			case 13: // pin 13, channel 3, PINCM30
				TIMA0->COUNTERREGS.CC_23[1] = (uint32_t) ((1 - (percentDutyCycle)/100)*period); 
			
				TIMA0->COUNTERREGS.CCCTL_23[1] |= GPTIMER_CCCTL_23_COC_COMPARE; 
				TIMA0->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP3_OUTPUT; 
				TIMA0->COUNTERREGS.CCACT_23[1] |= GPTIMER_CCACT_23_LACT_CCP_HIGH;
				TIMA0->COUNTERREGS.CCACT_23[1] |= GPTIMER_CCACT_23_CDACT_CCP_LOW;
        TIMA0->COUNTERREGS.OCTL_23[1] |= GPTIMER_OCTL_23_CCPO_FUNCVAL;
				TIMA0->COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP1_CCP_OUTPUT_OCTL;
				TIMA0->COUNTERREGS.OCTL_23[1] |= GPTIMER_OCTL_23_CCPOINV_NOINV;
				TIMA0->COUNTERREGS.OCTL_23[1] |= GPTIMER_OCTL_23_CCPIV_LOW;
			
				IOMUX->SECCFG.PINCM[IOMUX_PINCM30] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM30_PF_TIMA0_CCP3;
				IOMUX->SECCFG.PINCM[IOMUX_PINCM30] |= IOMUX_PINCM_DRV_DRVVAL1;
	
				GPIOB->DOESET31_0 |= GPIO_DOESET31_0_DIO13_SET;
				GPIOB->DOE31_0 |= GPIO_DOESET31_0_DIO13_SET;
				break;
			
			default:
				break;
		}
		
		TIMA0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;	  // Start (enable) the timer
}

void TIMA1_PWM_init(uint8_t pin, uint32_t period, uint32_t prescaler, double percentDutyCycle){
		if (!(TIMA1->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) { // if TIMG6 is not powered on 
			TIMA1->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT); // reset TIMG6 
			TIMA1->GPRCM.PWREN |= (GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE); // power on TIMG6
		}
	
		TIMA1->COMMONREGS.CCLKCTL &= ~GPTIMER_CCLKCTL_CLKEN_ENABLED; // disable clock timer 
		TIMA1->COUNTERREGS.CTRCTL &= 0;	  // stop the timer
		
		TIMA1->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE; // enable busclk 
		TIMA1->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1; // clock div by 1 
		TIMA1->COMMONREGS.CPS = prescaler;								
		TIMA1->COUNTERREGS.LOAD = period;
		TIMA1->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;
		TIMA1->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1; // enable down-counting 
		TIMA1->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED; // enable clock timer 
		
		// Edge-Aligned PWM Configuration
		
		switch(pin){
			case 4: // pin 4, channel 0, PINCM17
				TIMA1->COUNTERREGS.CC_01[0] = (uint32_t) ((1 - (percentDutyCycle)/100)*period); // set channel 0 duty cycle // RRR
			
				TIMA1->COUNTERREGS.CCCTL_01[0] |= GPTIMER_CCCTL_01_COC_COMPARE; // Set for compare mode
				TIMA1->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP0_OUTPUT; // Configure channel 0 for output 	
				TIMA1->COUNTERREGS.CCACT_01[0] |= GPTIMER_CCACT_01_LACT_CCP_HIGH; //set action on load to output high
				TIMA1->COUNTERREGS.CCACT_01[0] |= GPTIMER_CCACT_01_CDACT_CCP_LOW; //set action on compare(down) to output low
        TIMA1->COUNTERREGS.OCTL_01[0] |= GPTIMER_OCTL_01_CCPO_FUNCVAL; //set output to signal generator output
				TIMA1->COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP0_CCP_OUTPUT_OCTL; //enable corresponding CCP output (Counter n = channel 0, 1, etc)
				TIMA1->COUNTERREGS.OCTL_01[0] |= GPTIMER_OCTL_01_CCPOINV_NOINV; // no inversion 
				TIMA1->COUNTERREGS.OCTL_01[0] |= GPTIMER_OCTL_01_CCPIV_LOW; // keep output low while disabled 
			
				IOMUX->SECCFG.PINCM[IOMUX_PINCM17] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM17_PF_TIMA1_CCP0; // set PC connected for PB8
				IOMUX->SECCFG.PINCM[IOMUX_PINCM17] |= IOMUX_PINCM_DRV_DRVVAL1; // set to high drive strength PB8 
	
				GPIOB->DOESET31_0 |= GPIO_DOESET31_0_DIO4_SET;	//set hardware pin as outuput
				GPIOB->DOE31_0 |= GPIO_DOESET31_0_DIO4_SET;
				break;
			
			default:
				break;
		}
		
		TIMA1->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;	  // Start (enable) the timer
}


/**
 * @brief Change PWM duty cycle for all Timer A0 channels
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] percentDutyCycle - Duty cycle to change to
*/
void TIMA0_PWM_DutyCycle(uint8_t pin, double percentDutyCycle){
	switch(pin){
		
		case 8: // channel 0
			TIMA0->COUNTERREGS.CC_01[0] = percentDutyCycle;
			break;
		
		case 12: // channel 1
			TIMA0->COUNTERREGS.CC_01[1] = percentDutyCycle;
			break;
		
		case 17: // channel 2
			TIMA0->COUNTERREGS.CC_23[0] = percentDutyCycle;
			break;
		
		case 13: // channel 3
			TIMA0->COUNTERREGS.CC_23[1] = percentDutyCycle;
			break;
	}
}

/**
 * @brief Change PWM duty cycle for all Timer A1 channels
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] percentDutyCycle - Duty cycle to change to
*/
void TIMA1_PWM_DutyCycle(uint8_t pin, double percentDutyCycle){
	switch(pin){
		
		case 4: // channel 0
			TIMA1->COUNTERREGS.CC_01[0] = percentDutyCycle;
			break;
		
		default:
			break;
	}
	
}
