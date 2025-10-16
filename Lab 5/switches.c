#include "switches.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_gpio.h" 

/**
 * @brief Switch 1 polling initialization
 * @hint You might want to check out the schematics in the MSP User Guide
 *       The IOMUX has a hardware inversion bit
*/
void S1_init(void){
	if (!(GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)) { // if GPIOA is not powered on 
		GPIOA->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT); // reset GPIOA 
		GPIOA->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE); // power on GPIOA
  }
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= (0x80 | 0x01); // Select S1 for use as port I/O
	IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= 0x00040000; // Set S1 to input mode
	// IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= 0x00100000; // Set S1 to high drive strength
	
} 


/**
 * @brief Switch 2 polling initialization
*/
void S2_init(void){
	if (!(GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)) { // if GPIOB is not powered on 
		GPIOB->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT); // reset GPIOB
		GPIOB->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE); // power on GPIOB
  }
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= (0x80 | 0x01); // Select S2 for use as port I/O
	IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= 0x00040000; // Set S2 to input mode
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= 0x04000000; // invert logic for the IO
	
	// IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= (0x1 << 17); // also change to pullup 
	IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= (IOMUX_PINCM_PIPU_MASK); // change to pullup 
	
	
	
	
}


/**
 * @brief Check if switch 1 was pressed
 * @return True(1)/False(0) if switch 1 was pressed
*/
int S1_pressed(void){
	if(GPIOA->DIN31_0 & (0x1 << 18)){
		return 1;
	}
	return 0;
	
}


/**
 * @brief Check if switch 2 was pressed
 * @return True(1)/False(0) if switch 2 was pressed
*/
int S2_pressed(void){
	if(GPIOB->DIN31_0 & (0x1 << 21)){
		return 1;
	}
	return 0;
	
}

void S1_init_interrupt(void){
	S1_init();
	__disable_irq();	//function to disable IRQ interrupts (found in cmsis_armclang.h)
	GPIOA -> CPU_INT.ICLR &= GPIO_CPU_INT_ICLR_DIO18_CLR;	//clear SW1 interrupt
	GPIOA -> CPU_INT.IMASK |= GPIO_CPU_INT_IMASK_DIO18_SET;	//use mask to enable interrupt for SW1
	GPIOA -> POLARITY31_16 |= GPIO_POLARITY31_16_DIO18_RISE;	//set polarity (SW1 triggers interrupt on rising edge - when button is released)
	NVIC_EnableIRQ(GPIOA_INT_IRQn);	//enable interrupt request (IRQn is defined in mspm0g350x.h in IRQn enum)
	__enable_irq();	//function to enable IRQ interrupts (found in cmsis_armclang.h)
}

void S2_init_interrupt(void){
	S2_init();
	__disable_irq();	//function to disable IRQ interrupts (found in cmsis_armclang.h)
	GPIOB -> CPU_INT.ICLR &= GPIO_CPU_INT_ICLR_DIO21_CLR;	//clear SW1 interrupt
	GPIOB -> CPU_INT.IMASK |= GPIO_CPU_INT_IMASK_DIO21_SET;	//use mask to enable interrupt for SW2
	/* logic was inverted for SW2. What do we have to do for this? */
	GPIOB -> POLARITY31_16 |= GPIO_POLARITY31_16_DIO21_RISE;	//set polarity (SW2 triggers interrupt on falling edge - when button is pressed)
	NVIC_EnableIRQ(GPIOB_INT_IRQn);	//enable interrupt request (IRQn is defined in mspm0g350x.h in IRQn enum)
	__enable_irq();	//function to enable IRQ interrupts (found in cmsis_armclang.h)
}