/**
 * ******************************************************************************
 * @file    : camera.c
 * @brief   : Camera module header file
 * @details : Parallax TSL1401-DB Linescan Camera initialization and interaction
 * @note    : Reserves the use of Timers G0 (CLK) and G6 (SI)
 *            and ADC0 channel 0 
 *
 * @author 
 * @date 
 * ******************************************************************************
*/


#include "camera.h"

int cameraData_complete = 1;   // definition
uint16_t cameraData[128] = {0}; // definition
int pixelCounter = 0;          // definition

/**
 * @brief Initialize camera associated components
*/
void Camera_init(void){
    
    if (!(GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)) { // if GPIOA is not powered on 
        GPIOA->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT); // reset GPIOA 
        GPIOA->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE); // power on GPIOA
    }
    
    
    // init CLK (PA12) 
    IOMUX->SECCFG.PINCM[IOMUX_PINCM34] |= (0x80 | 0x01); // Select PA12 for use as port I/O
    IOMUX->SECCFG.PINCM[IOMUX_PINCM34] |= IOMUX_PINCM_INENA_ENABLE; // Set PA12 to input mode
    IOMUX->SECCFG.PINCM[IOMUX_PINCM34] |= IOMUX_PINCM_DRV_DRVVAL0; // set drive to 0 
    IOMUX->SECCFG.PINCM[IOMUX_PINCM34] |= IOMUX_PINCM_PC_CONNECTED; // set PC connected PA12
    
    GPIOA->DOESET31_0 |= GPIO_DOESET31_0_DIO12_SET;
    
    // ENABLE TIMER CLK
    TIMG0_init(2, GPTIMER_CPSV_CPSVAL_MAXIMUM);

    
    // init SI (PA28)
    IOMUX->SECCFG.PINCM[IOMUX_PINCM3] |= (0x80 | 0x01); // Select PA28 for use as port I/O
    IOMUX->SECCFG.PINCM[IOMUX_PINCM3] |= IOMUX_PINCM_INENA_ENABLE; // Set PA28 to input mode
    IOMUX->SECCFG.PINCM[IOMUX_PINCM3] |= IOMUX_PINCM_DRV_DRVVAL0; // set drive to 0 
    IOMUX->SECCFG.PINCM[IOMUX_PINCM3] |= IOMUX_PINCM_PC_CONNECTED; // set PC connected PA28
    
    GPIOA->DOESET31_0 |= GPIO_DOESET31_0_DIO28_SET;
    
    // ENABLE TIMER SI, 75000
    TIMG6_init(940, GPTIMER_CPSV_CPSVAL_MAXIMUM);
    
    

    TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_ENABLED; // DISABLE CLOCK, TIMG0 
    TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;// ENABLE SI, TIMG6
    
}

/**
 * @brief Checks whether camer data is ready to retrieve
 * @note Make sure to check all data is available
 * @return True(1)/False(0) if camera data is ready
*/
uint8_t Camera_isDataReady(void){
    // return value of flag indicating data is ready 
    return cameraData_complete;
}


/**
 * @brief Retrieves pointer to camera data array
 * @return Pointer to global data array stored locally in this file
*/
uint16_t* Camera_getData(void){
    return cameraData;
}
