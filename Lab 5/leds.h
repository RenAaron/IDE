/**
 * ******************************************************************************
 * @file    : leds.h
 * @brief   : LEDs module header file
 * @details : LED initialization and interaction
 * 
 * @author 
 * @date 
 * ******************************************************************************
*/

#ifndef _LEDS_H_
#define _LEDS_H_

#define RED_SHIFT 26
#define GREEN_SHIFT 27
#define BLUE_SHIFT 22

typedef enum{
	RED, 
	GREEN,
	BLUE,
	CYAN,
	MAGENTA,
	YELLOW,
	WHITE,
	CLEAR
} COLOR;

typedef enum{
	ON,
	OFF,
	TOGGLE
}LED_STATE;


/**
 * @brief Initialze LED1
 * @hint You might want to check out the schematics in the MSP User Guide
 *       The IOMUX has a hardware inversion bit
*/
void LED1_init(void);


/**
 * @brief Initialize LED2
 * @note You must account for each LED color
*/
void LED2_init(void);


/**
 * @brief Set LED1 output state
 * @note ON, OFF, TOGGLE
*/
void LED1_set(LED_STATE state);


/**
 * @brief Set LED2 output state
 * @note RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, OFF
*/
void LED2_set(COLOR color);



#endif // _LEDS_H_
