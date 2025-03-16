/**
 * @file      Lamp.h
 * @authors   Álvaro Velasco García
 * @date      March 16, 2025
 *
 * @brief     This header file declares the functions to control the a lamp.
 */

 #ifndef CORE_LAMP_H_
 #define CORE_LAMP_H_
 
/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <Button.h>
#include <LED.h>
#include <TCP_server.h>

/***************************************************************************************
 * Defines
 ***************************************************************************************/

/* Macro that enlist the board lamps. It is mandatory to not set values to the 
 * enumerates.
 */
#define LAMPS  \
  LAMP(LAMP_0)  

/* List of the possible return codes that module button can return. */
#define LAMP_RETURNS                        \
  /* Info codes */                          \
  LAMP_RETURN(CORE_LAMP_OK)                 \
  /* Error codes */                         \
  LAMP_RETURN(CORE_LAMP_INIT_ERR)           \
  LAMP_RETURN(CORE_LAMP_INIT_SEMAPHORE_ERR) \
  LAMP_RETURN(CORE_LAMP_UNKOWN_ID_ERR)      \
  LAMP_RETURN(CORE_LAMP_INIT_TASK_ERR)      \
  LAMP_RETURN(CORE_LAMP_DE_INIT_ERR)        \
  LAMP_RETURN(CORE_LAMP_START_SERVER_ERR)   \
  LAMP_RETURN(CORE_LAMP_STOP_SERVER_ERR)         
       
/***************************************************************************************
 * Data Type Definitions
 ***************************************************************************************/

/* Enumerate that enlist the board lamps. */
typedef enum
{
  #define LAMP(enumerate) enumerate,
    LAMPS
  #undef LAMP
  /* Last enumerate always, indicates the number of elements. Do not delete */
  NUM_OF_LAMPS,
} Lamp_ID;

/* Enumerate that lists the posible return codes that the module can return. */
typedef enum
{
  #define LAMP_RETURN(enumerate) enumerate,
    LAMP_RETURNS
  #undef LAMP_RETURN
  /* Last enumerate always, indicates the number of elements. Do not delete */
  NUM_OF_LAMP_RETURNS,
} Lamp_return;

/***************************************************************************************
 * Functions Prototypes
 ***************************************************************************************/

/**
 * @brief Initializes a lamp.
 * 
 * @param lamp Identifier of the lamp to initialize.
 * 
 * @param button Identifier of the button to associate to the lamp.
 * 
 * @param LED Identifier of the LED to associate to the lamp.
 *
 * @return CORE_LAMP_OK if the operation went well,
 *         otherwise:
 * 
 *           - CORE_LAMP_UNKOWN_ID_ERR: 
 *               Given lamp identifier does not exists. 
 * 
 *           - CORE_LAMP_INIT_ERR: 
 *               Failed trying to executing intermediate function to
 *               initialize the lamp.
 * 
 *           - CORE_LAMP_INIT_SEMAPHORE_ERR: 
 *               Error trying to initialize the semaphore.
 * 
 *           - CORE_LAMP_INIT_TASK_ERR: 
 *               Error trying to create the lamp task.
 *                                      
 */
Lamp_return Lamp_init(const Lamp_ID lamp,const Button_ID button, const LED_ID LED);

/**
 * @brief De-initializes a lamp.
 *
 * @param lamp Identifier of the lamp to de-initialize.
 *
 * @return CORE_LAMP_OK if the operation went well,
 *         otherwise:
 * 
 *           - CORE_LAMP_UNKOWN_ID_ERR: 
 *               Given lamp identifier does not exists. 
 * 
 *           - CORE_LAMP_DE_INIT_ERR: 
 *               Failed trying to executing intermediate function to
 *               de-initialize the lamp.
 * 
 */
Lamp_return de_init_lamp(const Lamp_ID lamp);

/**
 * @brief Starts the server that listen the commands.
 *
 * @param void
 *
 * @return CORE_LAMP_OK if the operation went well,
 *         otherwise:
 * 
 *           - CORE_LAMP_START_SERVER_ERR: 
 *               Failed to initialize the server. 
 * 
 */
Lamp_return lamp_start_server(void);

/**
 * @brief Stops the server that listen the commands.
 *
 * @param void
 *
 * @return CORE_LAMP_OK if the operation went well,
 *         otherwise:
 * 
 *           - CORE_LAMP_STOP_SERVER_ERR: 
 *               Failed to stop the server. 
 * 
 */
Lamp_return lamp_stop_server(void);

/**
 * @brief Prints the return of a lamp module function if the system was configured 
 *        in debug mode.
 *
 * @param ret Received return from a lamp module function.
 *
 * @return The given return.
 */
Lamp_return core_lamp_LOG(const Lamp_return ret);

#endif /* CORE_LAMP_H_ */
 