/**
 * ******************************************************************************
 * @file    : adc12.h
 * @brief   : ADC module header file
 * @details : ADC initialization and interaction
 * @note    : ADC does not require IOMUX interaction
 * 
 * @author 
 * @date 
 * ******************************************************************************
*/
#include <stdint.h>
#include "adc12.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"

/**
 * @brief Initialize ADC0
*/
void ADC0_init(void) {
    /* reset and power on module */
    if (!(ADC0 -> ULLMEM.GPRCM.PWREN & GPTIMER_PWREN_ENABLE_MASK)){        //gives a one if power is disabled
        ADC0 -> ULLMEM.GPRCM.RSTCTL |= GPTIMER_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;    //force reset (Key | ResetAssert)
        ADC0 -> ULLMEM.GPRCM.PWREN |= GPTIMER_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;    //enable power for PWREN (Key | Enable)
    }
    /* set ADCCLK to low power clock and highest frequency range */
    ADC0 -> ULLMEM.GPRCM.CLKCFG |= ADC12_CLKCFG_SAMPCLK_ULPCLK;    //set ADCCLK to ULPCLK
    ADC0 -> ULLMEM.CLKFREQ |= ADC12_CLKFREQ_FRANGE_RANGE40TO48;    //set ADC to highest freq range
    /* set power down mode so ADC stays on after conversion */
    ADC0 -> ULLMEM.CTL0 |= ADC12_CTL0_PWRDN_MANUAL;    //set power down to manual so that it doesn't power down on it's own after a conversion
    ADC0 -> ULLMEM.CTL0 |= ADC12_CTL0_SCLKDIV_DIV_BY_8;    //set the sampling clock divider to a factor of 8
    /* st single channel single conversion mode with auto software trigger */
    //ADC0 -> ULLMEM.CTL1 |= ADC12_CTL1_CONSEQ_SINGLE;    //use the CONSEQ bits in the CTL1 register to select the desired ADC conversion mode
    //ADC0 -> ULLMEM.MEMCTL[0] |= ADC12_MEMCTL_CHANSEL_CHAN_0;    //assign an ADC input channel to the appropriate MEMCTLx register using the CHANSEL bits
    ADC0 -> ULLMEM.CTL1 |= ADC12_CTL1_TRIGSRC_SOFTWARE;    //select HW or SW trigger using the TRIGSRC bit in the CTL1 register
    ADC0 -> ULLMEM.CTL1 |= ADC12_CTL1_SAMPMODE_AUTO;    //select AUTO or MANUAL sampling mode using the SAMPMODE bit in the CTL1 register
    //If using AUTO mode, program the desired sample timer value in the SCOMPx register and use the
    //STIME bits in the MEMCTLx register to select the appropriate sample timer source (SCOMP0 or
    //SCOMP1) -- what are we supposed to do here??
    ADC0 -> ULLMEM.CTL0 |= ADC12_CTL0_ENC_ON; //set the ENC bit in the CTL1 (is this a typo? there's no CTL0_ENC) register to enable ADC conversions -- is this the same as starting it??
    /* result is stored memory result register MEMRES[0] (in SVTMEM struct) -- is this what the last step is saying?? */
}


/**
 * @brief Retrieve a the value from the ADC0
 * @note The ADC channel in use is set during initialization
 *       The channel is not the same as where the module stores the value
 * @return ADC0 processed value
*/
uint32_t ADC0_getVal(void) {
    ADC0 -> ULLMEM.CTL0 |= ADC12_CTL0_ENC_ON; //set the ENC bit in the CTL1 (is this a typo? there's no CTL0_ENC) register to enable ADC conversions -- is this the same as starting it??
    ADC0 -> ULLMEM.CTL1 |= ADC12_CTL1_SC_START;    //set SC bit to start the sample phase (duration determined by sample timer)
    //wait for ADC to not be busy (when SC bit is cleared) -- is this not busy when ENC is cleared or SC is cleared
    while (ADC0 -> ULLMEM.STATUS == ADC12_STATUS_BUSY_ACTIVE) {
        //wait until ADC is not busy
    }
    return ADC0 -> ULLMEM.MEMRES[0];    //read output from memory result register
}
