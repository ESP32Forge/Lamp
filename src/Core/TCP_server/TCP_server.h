/**
 * @file      TCP_server.h
 * @authors   Álvaro Velasco García
 * @date      March 16, 2025
 *
 * @brief     This header file declares the functions to initialize the WiFi peripheral
 *            in access point mode.
 */

#ifndef CORE_TCP_SERVER_H_
#define CORE_TCP_SERVER_H_

/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <Network_config.h>

/***************************************************************************************
 * Defines
 ***************************************************************************************/

/* List of the possible return codes that module button can return. */
#define TCP_SERVER_RETURNS                       \
  /* Info codes */                               \
  TCP_SERVER_RETURN(CORE_TCP_SERVER_OK)          \   
  /* Error codes */                              \
  TCP_SERVER_RETURN(CORE_TCP_SERVER_INIT_ERR)    \
  TCP_SERVER_RETURN(CORE_TCP_SERVER_DE_INIT_ERR)                          
 
/***************************************************************************************
 * Data Type Definitions
 ***************************************************************************************/

/* Enumerate that lists the posible return codes that the module can return. */
typedef enum
{
  #define TCP_SERVER_RETURN(enumerate) enumerate,
    TCP_SERVER_RETURNS
  #undef TCP_SERVER_RETURN
  /* Last enumerate always, indicates the number of elements. Do not delete */
  NUM_OF_TCP_SERVER_RETURNS,
} TCP_server_return;

/***************************************************************************************
 * Functions Prototypes
 ***************************************************************************************/

/**
 * @brief Initialize the WiFi peripheral starting the device as a WiFi access point. 
 *        It is mandatory to call this function before any other function of this module.
 *
 * @param void
 *
 * @return CORE_TCP_SERVER_OK if the operation went well,
 *         otherwise:
 * 
 *         - CORE_TCP_SERVER_INIT_ERR:
 *             An error ocurred while executing an intermediate function.
 *           
 */
TCP_server_return init_TCP_server(void);

/**
 * @brief De-Initialize the WiFi peripheral. 
 *        It is mandatory to call this function before any other function of this module.
 *
 * @param void
 *
 * @return CORE_TCP_SERVER_OK if the operation went well,
 *         otherwise:
 * 
 *         - CORE_TCP_SERVER_DE_INIT_ERR:
 *             An error ocurred while executing an intermediate function 
 */
TCP_server_return de_init_TCP_server(void);

/**
 * @brief Prints the return of a TCP server module function if the system was configured 
 *        in debug mode.
 *
 * @param ret Received return from a TCP server module function.
 *
 * @return The given return.
 */
TCP_server_return core_TCP_server_LOG(const TCP_server_return ret);

/**
 * @brief Function that will be called if a message is received. This function should
 *        be implemented in other application module.
 *
 * @param cmd Structure that contains the received command.
 *
 * @return void
 */
void __attribute__((weak)) RX_command_frame(const TCP_COMMAND_TYPE cmd); 

#endif /* CORE_TCP_SERVER_H_ */
 