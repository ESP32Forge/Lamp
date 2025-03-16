/**
 * @file      Lamp.c
 * @authors   Álvaro Velasco García
 * @date      March 16, 2025
 *
 * @brief     This source file defines the functions to control a lamp.
 */

/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <Lamp.h>
#include <freertos/FreeRTOS.h>
#include <Debug.h>

/***************************************************************************************
 * Defines
 ***************************************************************************************/

#if DEBUG_MODE_ENABLE == 1
/* Tag to show traces in button BSP module. */
  #define TAG "CORE_LAMP"
#endif

/***************************************************************************************
 * Data Type Definitions
 ***************************************************************************************/

/* Structure that contains the information of a system lamp. */
typedef struct
{
  /* Identifier of button that belongs to the lamp. */
  Button_ID button;
  /* Identifier of LED that belongs to the lamp. */
  LED_ID LED;
  /* Handler of the task that will perform an action when the button lamp is pressed. */
  TaskHandle_t lamp_handler;
  /* Semaphore to coordinate the ISR interruption from the button with the lamp task. */
  SemaphoreHandle_t lamp_semaphore;
  /* Indicates if the lamp is on or off. */
  bool state;
  /* PWM duty cycle applied to the lamp LED. */
  uint8_t PWM_percentage;
} lamp_info;

/***************************************************************************************
 * Global Variables
 ***************************************************************************************/

/* Array that contains the configuration of the all the system lamps. */
static lamp_info lamps_infos[NUM_OF_LAMPS];

/***************************************************************************************
 * Functions Prototypes
 ***************************************************************************************/

/**
 * @brief Function that will handle the action when the button LAMP_0 is pressed.
 *
 * @param args arguments to pass to the function.
 *
 * @return void
 */
static void lamp_0_handler_func(void *args);

/**
 * @brief Checks if the given lamp identifier exists.
 *
 * @param ID Identifier to check
 *
 * @return True if the given ID exists, otherwise false.
 */
static bool check_lamp_ID(const Lamp_ID ID);

/**
 * @brief Toogles the LED of a given lamp
 *
 * @param ID Identifier of the lamp to toggle.
 *
 * @return True if the given ID exists, otherwise false.
 */
static bool toogle_LED_lamp(const Lamp_ID ID);

/***************************************************************************************
 * Functions
 ***************************************************************************************/

Lamp_return Lamp_init(const Lamp_ID lamp,const Button_ID button, const LED_ID LED)
{

  /* Check if the given lamp ID exists. */
  if(!check_lamp_ID(lamp))
  {
    return CORE_LAMP_UNKOWN_ID_ERR;
  }

  /* Initialize button. */
  if(BPS_button_LOG(init_button(button)) != BSP_BUTTON_OK)
  {
    return CORE_LAMP_INIT_ERR;
  }

  /* Initilzie LED. */
  if(BSP_LED_LOG(init_LED(LED)) != BSP_LED_OK)
  {
    return CORE_LAMP_INIT_ERR;
  }

  /* Create the sempahore of the lamp. */
  switch(lamp)
  {
    case LAMP_0:
    {
      lamps_infos[lamp].lamp_semaphore = xSemaphoreCreateCounting(1, 0);
      if(lamps_infos[lamp].lamp_semaphore == NULL)
      {
        vSemaphoreDelete(lamps_infos[lamp].lamp_semaphore);
        return CORE_LAMP_INIT_SEMAPHORE_ERR;
      }
    
      /* Create the task of the lamp. */
      const BaseType_t ret = xTaskCreate(lamp_0_handler_func, "lamp_0_handler_func", 2048,
        (void *) 0, configMAX_PRIORITIES-1, &lamps_infos[lamp].lamp_handler);
      if(ret != pdPASS)
      {
        return CORE_LAMP_INIT_TASK_ERR;
      }
    }
    break;
    default:
      /* Imposible to reach this line as it was checked before, defensive code. */
      return CORE_LAMP_INIT_ERR;
  }

  lamps_infos[lamp].button = button;
  lamps_infos[lamp].LED = LED;
  lamps_infos[lamp].PWM_percentage = MIN_DUTY_CYCLE_PERC;

  return CORE_LAMP_OK;
}

Lamp_return de_init_lamp(const Lamp_ID lamp)
{

  if(!check_lamp_ID(lamp))
  {
    return CORE_LAMP_UNKOWN_ID_ERR;
  }

  /* De-Initialize button. */
  if(BPS_button_LOG(de_init_button(lamps_infos[lamp].button)) != BSP_BUTTON_OK)
  {
    return CORE_LAMP_DE_INIT_ERR;
  }

  /* De-initilzie LED. */
  if(BSP_LED_LOG(de_init_LED(lamps_infos[lamp].LED)) != BSP_LED_OK)
  {
    return CORE_LAMP_DE_INIT_ERR;
  }

  if(lamps_infos[lamp].lamp_handler != NULL)
  {
    vTaskDelete(lamps_infos[lamp].lamp_handler);
  }
  
  if(lamps_infos[lamp].lamp_semaphore != NULL)
  {
    vSemaphoreDelete(lamps_infos[lamp].lamp_semaphore);
  }
  
  return CORE_LAMP_OK;
}

Lamp_return lamp_start_server(void)
{
  if(core_TCP_server_LOG(init_TCP_server()) != ESP_OK)
  {
    return CORE_LAMP_START_SERVER_ERR;
  }

  return CORE_LAMP_OK;
}

Lamp_return lamp_stop_server(void)
{
  if(core_TCP_server_LOG(de_init_TCP_server()) != ESP_OK)
  {
    return CORE_LAMP_STOP_SERVER_ERR;
  }

  return CORE_LAMP_OK;
}

inline Lamp_return core_lamp_LOG(const Lamp_return ret)
{
  #if DEBUG_MODE_ENABLE == 1
    switch(ret)
    {
      #define LAMP_RETURN(enumerate)   \
        case enumerate:                \
          if(ret > 0)                  \
          {                            \
            ESP_LOGE(TAG, #enumerate); \
          }                            \
          else                         \
          {                            \
            ESP_LOGI(TAG, #enumerate); \
          }                            \
          break;       
        LAMP_RETURNS
      #undef LAMP_RETURN
      default:
        ESP_LOGE(TAG, "Unkown return.");
        break;
    }
  #endif
  return ret;
}

static bool check_lamp_ID(const Lamp_ID ID)
{
  switch(ID)
  {
    #define LAMP(LAMP_ID)        \
      case LAMP_ID: return true;    
      LAMPS
    #undef LAMP
    default: 
      return false;
  }
}

static bool toogle_LED_lamp(const Lamp_ID ID)
{
  /* Check if the given lamp ID exists. */
  if(!check_lamp_ID(ID))
  {
    return false;
  }

  if(!lamps_infos[ID].state)
  {
    BSP_LED_LOG(set_LED_state(lamps_infos[ID].LED, lamps_infos[ID].PWM_percentage));
  }
  else
  {
    BSP_LED_LOG(turn_off_LED(lamps_infos[ID].LED));
  }

  lamps_infos[ID].state = !lamps_infos[ID].state;

  return true;
}

/* Implemtation of the TCP server received callback. */
void __attribute__((weak)) RX_command_frame(const TCP_COMMAND_TYPE cmd)
{

  bool LED_ID_is_valid = false;
  switch(cmd.ID)
  {
    case LED_0:
      LED_ID_is_valid = true;
      break;
    default:
      ESP_LOGE(TAG, "Received invalid LED identifier.");
      break;
  }

  if(LED_ID_is_valid)
  {
    Lamp_ID ID = 0u;
    while(ID < NUM_OF_LAMPS && lamps_infos[ID].LED != cmd.ID)
    {
      ID++;
    }

    switch(cmd.action)
    {
      case TOOGLE_LED:
        if(!toogle_LED_lamp(ID))
        {
          /* Imposible to reach this line as it was cheked before. */
        }
        break;
      default:
        ESP_LOGE(TAG, "Received invalid action.");
        break;
    }
  }
}

/* Implemtation of the button callbacks. */
void __attribute__((weak)) button_CB(const Button_ID ID)
{
  BaseType_t higher_priority_task_woken = pdFALSE;
  xSemaphoreGiveFromISR(lamps_infos[LAMP_0].lamp_semaphore, 
    &higher_priority_task_woken);
}

static void lamp_0_handler_func(void *args)
{
  while(true)
  {

    if(xSemaphoreTake(lamps_infos[LAMP_0].lamp_semaphore, portMAX_DELAY) == pdTRUE)
    {
      if(!toogle_LED_lamp(LAMP_0))
      {
        /* TODO: Handle this corner case. */
      }
    }

  }
}

