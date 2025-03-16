/**
 * @file      main.c
 * @authors   Álvaro Velasco García
 * @date      March 16, 2025
 *
 * @brief     This source file is the entry point of the Proto pixel interview
 */

/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <Lamp.h>
#include <Debug.h>

/***************************************************************************************
 * Functions
 ***************************************************************************************/

void app_main() 
{

  bool error = false;

  /** Initialize BSP modules **/
  if(BPS_button_LOG(init_BSP_button_module()) != BSP_BUTTON_OK)
  {
    error = true;
    #if DEBUG_MODE_ENABLE == 1
      ESP_LOGE("MAIN", "Can not initialize BSP button.");
    #endif
  }

  if(!error && BPS_button_LOG(init_BSP_LED_module()) != BSP_BUTTON_OK)
  {
    error = true;
    #if DEBUG_MODE_ENABLE == 1
      ESP_LOGE("MAIN", "Can not initialize BSP button.");
    #endif
  }

  if(!error)
  {
  
    if(core_lamp_LOG(Lamp_init(LAMP_0, BUTTON_0, LED_0) != CORE_LAMP_OK))
    {
      #if DEBUG_MODE_ENABLE == 1
        ESP_LOGE("MAIN", "Failed to initialize LAMP.");
      #endif
    }
  }

}

