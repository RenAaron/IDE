#include "i2c.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/msp.h" 
#include "sysctl.h"

void I2C1_init(uint16_t targetAddress){
	
	// power on and reset exactly like UART
	if (!(I2C1->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)){ // if UART0 is not powered on 
		I2C1->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT); // reset (why is RSTCTL GPRCM nested 
		I2C1->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE); // power on UART0
	}
		
	// use PINCMx to set SDA and SCL for PC, PF INENA (mspm0g350x.h)
	IOMUX->SECCFG.PINCM[IOMUX_PINCM16] |= IOMUX_PINCM16_PF_I2C1_SDA; // SDA 
	IOMUX->SECCFG.PINCM[IOMUX_PINCM15] |= IOMUX_PINCM15_PF_I2C1_SCL; // SCL
	IOMUX->SECCFG.PINCM[IOMUX_PINCM16] |= IOMUX_PINCM_PC_CONNECTED; 
	IOMUX->SECCFG.PINCM[IOMUX_PINCM15] |= IOMUX_PINCM_PC_CONNECTED; 
	IOMUX->SECCFG.PINCM[IOMUX_PINCM16] |= IOMUX_PINCM_INENA_ENABLE; // enable [SDA] as input (?) unsure if we do this for clock but i imagine not
	IOMUX->SECCFG.PINCM[IOMUX_PINCM15] |= IOMUX_PINCM_INENA_ENABLE;
	
	// configure BUSCLK for & a div ratio of 1 
	I2C1->CLKSEL = UART_CLKSEL_BUSCLK_SEL_ENABLE;
	I2C1->CLKDIV = UART_CLKDIV_RATIO_DIV_BY_1;
		
	// glitch filter control register, disable analog glitch supression
	I2C1->GFCTL &= ~(I2C_GFCTL_AGFEN_MASK);
		
	// clear controller control registers (see doc)
	I2C1->MASTER.MCTR = 0x0;
		
	// determine value of BUSCLK to plug into I2C CLK reg (?)
	I2C1->MASTER.MTPR = 0x1f; // hmmm 0x1f, maybe a million 

	// Use FIFO control register to set RX FIFO to trigger when RX FIFO contains a byte 
	I2C1->MASTER.MFIFOCTL |= I2C_MFIFOCTL_RXTRIG_LEVEL_1;
		
	// Use FIFO control register to set TX FIFO to trigger when TX FIFO is empty 
	I2C1->MASTER.MFIFOCTL |= I2C_MFIFOCTL_TXTRIG_EMPTY; 
	
	// disable clock stretching using controller register
	I2C1->MASTER.MCR &= ~(I2C_SCTR_SCLKSTRETCH_ENABLE); 
	
	// use controller target address register to set the targetAddress ^^^^ 
	I2C1->MASTER.MSA = targetAddress << I2C_MSA_SADDR_OFS;
	
	// enable I2C controller using the controller configuration register 
	I2C1->MASTER.MCR |= I2C_MCR_ACTIVE_ENABLE; 
	
}


/**
 * @brief Sends a single character byte over I2C1
 * @param[in] ch - Byte to send
*/
void I2C1_putchar(uint8_t ch){
	// wait until space in transmit fifo 
	// transmit byte 
	
	while(! ((I2C1->MASTER.MFIFOSR & I2C_MFIFOSR_TXFIFOCNT_MASK) >= I2C_MFIFOSR_TXFIFOCNT_MINIMUM)){};// if there isnt stuff to transmit, wait 	
	I2C1->MASTER.MTXDATA = ch;
	
}


/**
 * @brief Send full character string over I2C until limit is reached
 * @param[in] data - String pointer to data to send
 * @param[in] data_size - Amount of bytes to transmit
*/
void I2C1_put(uint8_t *data, uint16_t data_size){ // CCTR 
	I2C1->MASTER.MSA |= I2C_MSA_DIR_TRANSMIT; // transmit direction
	I2C1->MASTER.MCTR |= I2C_MCTR_MBLEN_MASK & (data_size << I2C_MCTR_MBLEN_OFS); // length
	I2C1->MASTER.MCTR |= I2C_MCTR_STOP_ENABLE; // STOP 
	I2C1->MASTER.MCTR |= I2C_MCTR_START_ENABLE; // START 
	I2C1->MASTER.MCTR |= I2C_MCTR_BURSTRUN_MASK; // RUN 
	
	for(int i = 0; i < data_size; i++){
		I2C1_putchar(*data);
		data++; 
	}
	while(!(I2C1->MASTER.MSR & I2C_MSR_IDLE_MASK)){}; // wait until idle
	
	I2C1->MASTER.MCTR &= ~(I2C_MCTR_BURSTRUN_MASK); // disable the module (clear the burst run bit) 
		
	I2C1->MASTER.MCTR &= (I2C_MCTR_MBLEN_MASK); // length
	
}
