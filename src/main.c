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
#include <Button.h>
#include <Debug.h>

#include <freertos/FreeRTOS.h>


static int num_of_calls = 0;
void button_CB(const Button_ID ID)
{
  num_of_calls++;
}

/***************************************************************************************
 * Functions
 ***************************************************************************************/

void app_main() 
{

  if(init_BSP_button_module() != BSP_BUTTON_OK)
  {
    printf("Error BSP module init.\n");
  }

  if(init_button(BUTTON_0) != BSP_BUTTON_OK)
  {
    printf("Error BSP init button.\n");
  }

  uint64_t num_of_presses = 0u;
  while(true)
  {
    get_num_of_presses(BUTTON_0, &num_of_presses);
    printf("Num of pressed times: %lld\n", num_of_presses);
    printf("Num of calls: %d\n", num_of_calls);
    vTaskDelay(1000u/portTICK_PERIOD_MS);
  }

}

