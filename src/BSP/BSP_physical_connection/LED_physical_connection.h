/**
 * @file      LED_physical_connection.c
 * @authors   Álvaro Velasco García
 * @date      March 15, 2025
 *
 * @brief     This header file defines the board LEDs and their physical connections.
 */

#ifndef LED_PHYSICAL_CONNECTION_H_
#define LED_PHYSICAL_CONNECTION_H_
 
/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <System_lights.h>
 
/***************************************************************************************
 * Defines
 ***************************************************************************************/
  
/* Macro that describes the GPIO configurations of the LEDs. 
 *
 * Parameters:
 * 
 *   1) Identifier of the LED, it is mandatory to put a value defined inside LEDS.
 *   2) Identifier of the GPIO that sets the LED state, it is mandtory to use
 *      an enumerate defined in gpio_num_t enum -> gpio_num.h
 *   3) Indicates the pull mode of the GPIO. It is mandtory to use an enumerate defined 
 *      in gpio_pull_mode_t enum -> gpio_types.h
 *   4) Timer that will control the LED. It is mandatory to use 
 *      ledc_timer_t enumerate -> ledc_types.h
 *   5) Speed of the timer. It is mandatory to use ledc_mode_t enumerate -> ledc_types.h 
 *   6) Channel of the timer that will control the LED. It is mandatory to use 
 *      ledc_channel_t enumerate -> ledc_channel_t.
 *   7) Resolution in bits of the PWM duty cycle. It is mandatory to use ledc_timer_bit_t
 *      enumerate -> ledc_types.h.
 *   8) Frequency in Hertz of the PWM that controls the LED.
 *   
 */
#define LED_CONFIGURATIONS                                                         \
  LED_CONFIG(LED_0, GPIO_NUM_20, GPIO_FLOATING, LEDC_TIMER_0, LEDC_LOW_SPEED_MODE, \
             LEDC_CHANNEL_0, LEDC_TIMER_13_BIT, 4000u)     

#endif /* LED_PHYSICAL_CONNECTION_H_ */
  