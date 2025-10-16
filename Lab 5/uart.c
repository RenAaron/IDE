#include "uart.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/msp.h" 
#include "sysctl.h"


/**
 * @brief Initialize UART0
*/
void UART0_init(void){
		// if power not enabled 
        // power on and reset module 

    if (!(UART0->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)){ // if UART0 is not powered on 
			UART0->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT); // reset UART0 (why is RSTCTL GPRCM nested 
			UART0->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE); // power on UART0
		}

		// set PF for RX and TX for respective PINMCx's 
		IOMUX->SECCFG.PINCM[IOMUX_PINCM21] |= IOMUX_PINCM1_PF_UART0_TX; 
		IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= IOMUX_PINCM2_PF_UART0_RX; 

    // set peripheral is connected to be enabled (RX and TX)
		IOMUX->SECCFG.PINCM[IOMUX_PINCM21] |= IOMUX_PINCM_PC_CONNECTED; 
		IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= IOMUX_PINCM_PC_CONNECTED; 

    //  enable pin as input for RX 
		IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= IOMUX_PINCM_INENA_ENABLE;
		
		// enable BUSCLK for UART0
		UART0->CLKSEL |= UART_CLKSEL_BUSCLK_SEL_ENABLE;
		
		// divide the clock by 1 
		UART0->CLKDIV |= UART_CLKDIV_RATIO_DIV_BY_1;
		
		// diable UART by clearing enable bit, we need this for the following operations 
		UART0->CTL0 &= ~UART_CTL0_ENABLE_MASK;
		
		// set oversampling rate to 16x 
		UART0->CTL0 |= UART_CTL0_HSE_OVS16; // this is all 0s?? why does oring this in do anything ??
		
		// set TX, RX, and FIFO
		UART0->CTL0 |= UART_CTL0_TXE_MASK;
		UART0->CTL0 |= UART_CTL0_RXE_MASK;
		UART0->CTL0 |= UART_CTL0_FEN_ENABLE; // how do I find this organically? TRM doesn't mention it.
		
		// set baud rate, oh boy
		
		// int int_BDR = SYSCTL_SYSCLK_getULPCLK()/(16*9600); // 208.333 <- use thif fractional part 
		// int frac_BDR = (((int_BDR - (SYSCTL_SYSCLK_getULPCLK()/(16*9600)))*64) + 0.5); // 0.33333*64)+0.5 = 21.something, take 21
		
		
		int int_BDR = 208;
		int frac_BDR = 21; 
		
		UART0->IBRD = int_BDR; // set integer IDBR 
		UART0->FBRD = frac_BDR; // set fractional IDBR 
		
		// set line Control Register? such that the UART uses 8 data bits, 1 stop bit, and no parity bit
		
		UART0->LCRH |= UART_LCRH_WLEN_DATABIT8; // 8 data bit
		UART0->LCRH |= UART_LCRH_STP2_DISABLE; // 1 stop bit 
		UART0->LCRH |= UART_LCRH_SPS_DISABLE; // disable parity bit
		
		
		// enable UART0 again
		UART0->CTL0 |= UART_CTL0_ENABLE_MASK;
}


/**
 * @brief Put a character over UART0
 * @param[in] ch - Character to print
*/
void UART0_putchar(uint8_t ch){
	while(UART0->STAT & UART_STAT_TXFF_SET); // wait until TX FIFO is not full 
	UART0->TXDATA = ch;
}


/**
 * @brief Retrieve a single character from UART0
*/
char UART0_getchar(void){
	while((UART0->STAT & UART_STAT_RXFE_MASK)); // wait until RX fifo is not empty 
	return UART0->RXDATA;
}


/**
 * @brief Send a full character string over UART0
 * @param[in] ptr_str - Pointer to the string to print
*/
void UART0_put(uint8_t *ptr_str){
	uint8_t* p = ptr_str;
	while(*p != '\0'){
		UART0_putchar(*p);
		p += 1;
	}
}

void UART0_printDec(int num) {
    int neg = 0;
    char String[12];
    char *StringPtr = &String[11]; // Point to the end of the buffer
    *StringPtr = '\0'; // Null terminator for the string

    // Handle zero case directly
    if (num == 0) {
        *(--StringPtr) = '0';
    } else {
        if (num < 0) {
            neg = 1;
            num = -num; // Convert to positive value
        }
        // Convert number to string
        while (num > 0) {
            *(--StringPtr) = (num % 10) + '0';
            num /= 10;
        }
        if (neg) *(--StringPtr) = '-';
    }
    UART0_put(StringPtr);
}

void UART0_printFloat(double num) {
    if (num < 0) {
        UART0_putchar('-');
        num = -num;
    }
    int integerPart = (int) num;
    double fractionalPart = num - integerPart;

    UART0_printDec(integerPart);
    UART0_putchar('.');

    int precision = 0;
    while (fractionalPart > 1e-6 && precision++ < 6) {
        fractionalPart *= 10;
        UART0_putchar((char) ('0' + (int) fractionalPart));
        fractionalPart -= (int) fractionalPart;
    }
}

