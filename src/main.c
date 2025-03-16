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
#include <LED.h>
#include <Debug.h>

#include <freertos/FreeRTOS.h>

/***************************************************************************************
 * Functions
 ***************************************************************************************/

void app_main() 
{

  int counter = 0;

  if(init_BSP_LED_module() != BSP_LED_OK)
  {
    printf("Error BSP LED init.\n");
  }

  if(init_LED(LED_0) != BSP_LED_OK)
  {
    printf("Error BSP init LED.\n");
  }

  while(true)
  {

    if(counter % 2)
    {
      if(set_LED_state(LED_0, MIN_DUTY_CYCLE_PERC+(counter%100)) != BSP_LED_OK)
      {
        printf("Error set_LED_state.\n");
      }
    }
    else
    {
      turn_off_LED(LED_0);
    }

    counter++;
    vTaskDelay(1000u/portTICK_PERIOD_MS);
  }

}

