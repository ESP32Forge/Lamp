/**
 * @file      LED.h
 * @authors   Álvaro Velasco García
 * @date      March 16, 2025
 *
 * @brief     This header file defines the functions to control the board LEDs.
 */

#ifndef BSP_LED_H_
#define BSP_LED_H_

/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <LED_physical_connection.h>

/***************************************************************************************
 * Defines
 ***************************************************************************************/

/* List of the possible return codes that module LED can return. */
#define LED_RETURNS                           \
  /* Info codes */                            \
  LED_RETURN(BSP_LED_OK)                      \
  /* Error codes */                           \
  LED_RETURN(BSP_LED_INVALID_LEDS_CONFIG)     \
  LED_RETURN(BSP_LED_MODULE_INIT_ERR)         \
  LED_RETURN(BSP_LED_MODULE_DE_INIT_ERR)      \
  LED_RETURN(BSP_LED_MODULE_WAS_NOT_INIT_ERR) \
  LED_RETURN(BSP_LED_DOES_NOT_EXIST_ERR)      \
  LED_RETURN(BSP_LED_WAS_INIT_ERR)            \
  LED_RETURN(BSP_LED_SET_LED_STATE_ERR)       \
  LED_RETURN(BSP_LED_WAS_NOT_INIT_ERR)        

/***************************************************************************************
 * Data Type Definitions
 ***************************************************************************************/

/* Enumerate that lists the posible return codes that the module can return. */
typedef enum
{
  #define LED_RETURN(enumerate) enumerate,
    LED_RETURNS
  #undef LED_RETURN
  /* Last enumerate always, indicates the number of elements. Do not delete */
  NUM_OF_LED_RETURNS,
} LED_return;

/***************************************************************************************
 * Functions Prototypes
 ***************************************************************************************/

/**
 * @brief Initializes the BSP module structures to operate the board LEDs. 
 *        It is mandatory to call first this function before any other function of this
 *        module.
 *
 * @param void
 *
 * @return BSP_LED_RET_OK If the initialization went well, 
 *         otherise:
 * 
 *           - BSP_LED_INVALID_LEDS_CONFIG: 
 *               The provided configuration in LED_physical_connection.h is not valid.            
 */
LED_return init_BSP_LED_module(void);

/**
 * @brief Initializes a board LED.
 *
 * @param ID Identifier of the LED to initialize.
 *
 * @return BSP_LED_RET_OK If the initialization went well, 
 *         otherwise:
 *  
 *           - BSP_LED_MODULE_WAS_NOT_INIT_ERR: 
 *               BSP LED module was not intialized before.
 * 
 *           - BSP_LED_DOES_NOT_EXIST_ERR: 
 *               The given ID does not exist.
 * 
 *           - BSP_LED_WAS_INIT_ERR:
 *               Led was initialized before.
 * 
 *           - BSP_LED_MODULE_INIT_ERR:
 *               An error ocurred in one of the intermediate functions.
 * 
 */
LED_return init_LED(const LED_ID ID);

/**
 * @brief De-initializes a board LED.
 *
 * @param ID Identifier of the LED to de-initialize.
 *
 * @return BSP_LED_RET_OK If the initialization went well, 
 *         otherwise:
 *  
 *           - BSP_LED_MODULE_WAS_NOT_INIT_ERR: 
 *               BSP LED module was not intialized before.
 * 
 *           - BSP_LED_DOES_NOT_EXIST_ERR: 
 *               The given ID does not exist.
 * 
 *           - BSP_LED_WAS_NOT_INIT_ERR:
 *               Led was not initialized before.
 * 
 *           - BSP_LED_MODULE_DE_INIT_ERR:
 *               An error ocurred in one of the intermediate functions.
 * 
 */
LED_return de_init_LED(const LED_ID ID);

/**
 * @brief Sets a new duty cycle to a given LED.
 *
 * @param ID Identifier of the LED in which it will modify its PWM.
 * 
 * @param duty_cycle Duty cycle in percentage terms to set to the LED.
 *
 * @return BSP_LED_RET_OK If the operation went well,
 *         otherwise:
 * 
 *           - BSP_LED_MODULE_WAS_NOT_INIT_ERR: 
 *               BSP LED module was not intialized before.
 * 
 *           - BSP_LED_DOES_NOT_EXIST_ERR: 
 *               The given ID does not exist.
 * 
 *           - BSP_LED_SET_LED_STATE_ERR:
 *               An error ocurred in one of the intermediate functions.
 * 
 */
LED_return set_LED_state(const LED_ID ID, const uint8_t duty_cycle);

/**
 * @brief Turns off a LED.
 *
 * @param ID Identifier of the LED to turn off.
 *
 * @return BSP_LED_RET_OK If the operation went well,
 *         otherwise:
 * 
 *           - BSP_LED_MODULE_WAS_NOT_INIT_ERR: 
 *               BSP LED module was not intialized before.
 * 
 *           - BSP_LED_DOES_NOT_EXIST_ERR: 
 *               The given ID does not exist.
 * 
 *           - BSP_LED_SET_LED_STATE_ERR:
 *               An error ocurred in one of the intermediate functions.
 * 
 */
LED_return turn_off_LED(const LED_ID ID);

/**
 * @brief Prints the return of a LED module function if the system was configured 
 *        in debug mode.
 *
 * @param ret Received return from a LED module function.
 *
 * @return The given return.
 */
LED_return BSP_LED_LOG(const LED_return ret);

#endif /* BSP_LED_H_ */