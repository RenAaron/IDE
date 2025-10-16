#include "leds.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_gpio.h" 

/**
 * @brief Initialze LED1
 * @hint You might want to check out the schematics in the MSP User Guide
 *       The IOMUX has a hardware inversion bit
*/
void LED1_init(void){
	
	if (!(GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)) { // if GPIOA is not powered on 
		GPIOA->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT); // reset GPIOA 
		GPIOA->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE); // power on GPIOA
	}
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= (0x80 | 0x01); // Select LED1 for use as port I/O (for all) 
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= 0x04000000; // LED1, invert logic for the IO, for active low 
	
	GPIOA->DOESET31_0 |= 0x01; // set up GPIO to output 
	
}


/**
 * @brief Initialize LED2
 * @note You must account for each LED color
*/
void LED2_init(void){
	
	if (!(GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)) { // if GPIOB is not powered on 
		GPIOB->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT); // reset GPIOB
		GPIOB->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE); // power on GPIOB
  }
	
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM57] |= (0x80 | 0x01); // Select LED_RED for use as port I/O
	IOMUX->SECCFG.PINCM[IOMUX_PINCM57] |= 0x00100000; // Set LED_RED to high drive strength
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM58] |= (0x80 | 0x01); // Select LED_GREEN for use as port I/O
	IOMUX->SECCFG.PINCM[IOMUX_PINCM58] |= 0x00100000; // Set LED_GREEN to high drive strength
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM50] |= (0x80 | 0x01); // Select LED_BLUE for use as port I/O
	IOMUX->SECCFG.PINCM[IOMUX_PINCM50] |= 0x00100000; // Set LED_BLUE to high drive strength
	
	GPIOB->DOESET31_0 |= (1 << 26); 
	GPIOB->DOESET31_0 |= (1 << 27); 
	GPIOB->DOESET31_0 |= (1 << 22); 	
	
}


/**
 * @brief Set LED1 output state
 * @note ON, OFF, TOGGLE
*/
void LED1_set(LED_STATE state){
	switch(state){
		case ON:
			GPIOA->DOUTSET31_0 |= 0x01;
			break;
		case OFF:
			GPIOA->DOUTCLR31_0 |= 0x01;
			break;
		case TOGGLE:
			GPIOA->DOUTTGL31_0 |= 0x01;
			break;
		default:
			break;
	}
}


/**
 * @brief Set LED2 output state
 * @note RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, OFF
*/
void LED2_set(COLOR color){
	switch(color){
		case RED: 
			GPIOB->DOUTSET31_0 |= (1 << RED_SHIFT);
			break;
		case GREEN:
			GPIOB->DOUTSET31_0 |= (1 << GREEN_SHIFT);
			break;
		case BLUE:
			GPIOB->DOUTSET31_0 |= (1 << BLUE_SHIFT);
			break;
		case CYAN:
			GPIOB->DOUTSET31_0 |= (1 << BLUE_SHIFT);
			GPIOB->DOUTSET31_0 |= (1 << GREEN_SHIFT);
			break;
		case MAGENTA:
			GPIOB->DOUTSET31_0 |= (1 << BLUE_SHIFT);
			GPIOB->DOUTSET31_0 |= (1 << RED_SHIFT);
			break;
		case YELLOW:
			GPIOB->DOUTSET31_0 |= (1 << RED_SHIFT);
			GPIOB->DOUTSET31_0 |= (1 << GREEN_SHIFT);
			break;
		case WHITE:
			GPIOB->DOUTSET31_0 |= (1 << BLUE_SHIFT);
			GPIOB->DOUTSET31_0 |= (1 << GREEN_SHIFT);
			GPIOB->DOUTSET31_0 |= (1 << RED_SHIFT);
			break;
		case CLEAR:
			GPIOB->DOUTCLR31_0 |= (1 << BLUE_SHIFT);
			GPIOB->DOUTCLR31_0 |= (1 << GREEN_SHIFT);
			GPIOB->DOUTCLR31_0 |= (1 << RED_SHIFT);
		default:
			break;
	}
}