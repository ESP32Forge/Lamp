/**
 * @file      LED.c
 * @authors   Álvaro Velasco García
 * @date      March 16, 2025
 *
 * @brief     This source file declares the functions to control the board LEDs.
 */

/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <LED.h>
#include <Debug.h>

/***************************************************************************************
 * Defines
 ***************************************************************************************/

/* Assistance macro to check if the module was initialized. */
#define CHECK_IF_MODULE_WAS_INTIALIZED      \
  if(!LED_module_was_initialized)           \
  {                                         \
    return BSP_LED_MODULE_WAS_NOT_INIT_ERR; \
  }

#if DEBUG_MODE_ENABLE == 1
  /* Tag to show traces in button BSP module. */
  #define TAG "BSP_BUTTON"
#endif

/***************************************************************************************
 * Data Type Definitions
 ***************************************************************************************/

/* Structure that contains the information of a system LEDs. */
typedef struct
{
  /* Identifier of the LED. */
  LED_ID ID;
  /* Flag to indicate if the LED was initialized. */
  bool was_initialized;
  /* Identifier of the GPIO that sets the state of the LED. */
  gpio_num_t GPIO;
  /* Indicates the pull mode of the LED GPIO. */
  gpio_pull_mode_t pull_mode;
  /* Structure that contains the configuration of the timer that controls the LED. */
  ledc_timer_config_t ledc_timer;
  /* Structure that contains the configuration of the timer channel that controls the
   * LED. 
   */
  ledc_channel_config_t ledc_channel;
} system_LED_info;

/***************************************************************************************
 * Global Variables
 ***************************************************************************************/

/* Flag that indicates if the GPIO LEDs were initialized or not. */
static bool LED_module_was_initialized;

/* Array that contains the configuration of the all the system LEDs. */
static system_LED_info system_LEDs_infos[NUM_OF_LEDS] =
{
  #define LED_CONFIG(LED_ID, LED_GPIO_ID, LED_GPIO_PULL_MODE, LED_TIMER, PWM_SPEED, \
                     PWM_CHAN, PWM_RESOL, PWM_FREQ)                                 \
    {                                                                               \
      /* LED ID */                 LED_ID,                                          \
      /* Was initialized */        false,                                           \
      /* LED GPIO ID */            LED_GPIO_ID,                                     \
      /* GPIO pull mode */         LED_GPIO_PULL_MODE,                              \
      /* Timer configuration */                                                     \
      {                                                                             \
                                   .speed_mode       = PWM_SPEED,                   \
                                   .timer_num        = LED_TIMER,                   \
                                   .duty_resolution  = PWM_RESOL,                   \
                                   .freq_hz          = PWM_FREQ,                    \
                                   .clk_cfg          = LEDC_AUTO_CLK,               \
      },                                                                            \
      /* Timer configuration */                                                     \
      {                                                                             \
                                   .speed_mode       = PWM_SPEED,                   \
                                   .channel          = PWM_CHAN,                    \
                                   .timer_sel        = LED_TIMER,                   \
                                   .intr_type        = LEDC_INTR_DISABLE,           \
                                   .gpio_num         = LED_GPIO_ID,                 \
                                   .duty             = 0,                           \
                                   .hpoint           = 0,                           \
      },                                                                            \
    },                                                                        
    LED_CONFIGURATIONS
  #undef LED_CONFIG
};

/***************************************************************************************
 * Functions Prototypes
 ***************************************************************************************/

/**
 * @brief Checks if the configuration of the system LEDs defined in 
 *        LED_CONFIGURATIONS macro (LED_physical_connection.h) list
 *        is correct. Also, as it needs to loop each button configuration,
 *        it sort them to avoid inncessary complexity cost in the rest of
 *        the module function.
 *
 * @param void
 *
 * @return True if the configuration is correct, otherwise false.
 */
static bool check_configurations_sort(void);

/**
 * @brief Checks if the given LED ID is defined in the system.
 *
 * @param ID LED identifier to check.
 *
 * @return True if the LED exists, otherwise false.
 */
static bool check_LED_ID(const LED_ID ID);

/**
 * @brief Given a duty cycle in terms of percetange, it returns the
 *        duty cycle in terms of steps.
 *
 * @param ID Identifier of the LED from wich it will modify its PWM duty cycle.
 * 
 * @param dutyPercentage Given duty cycle in terms of percentage.
 * 
 * @param dutyInSteps Return duty cycle in terms of steps.
 * 
 * @return True if the operation went well, otherwise false.
 */
static bool cacl_pwm_duty(const LED_ID ID, const uint8_t dutyPercentage, 
  uint32_t *dutyInSteps);

/***************************************************************************************
 * Functions
 ***************************************************************************************/

LED_return init_BSP_LED_module(void)
{
   if(!LED_module_was_initialized)
   {
 
     if(!check_configurations_sort())
     {
       return BSP_LED_INVALID_LEDS_CONFIG;
     }
 
     LED_module_was_initialized = true;
   }
 
   return BSP_LED_OK;
}

LED_return init_LED(const LED_ID ID)
{
  CHECK_IF_MODULE_WAS_INTIALIZED;

  if(!check_LED_ID(ID))
  {
    return BSP_LED_DOES_NOT_EXIST_ERR;
  }

  if(system_LEDs_infos[ID].was_initialized)
  {
    return BSP_LED_WAS_INIT_ERR;
  }

  if(ESP_error_check(gpio_reset_pin(system_LEDs_infos[ID].GPIO)) != ESP_OK)
  {
    return BSP_LED_MODULE_INIT_ERR;
  }

  /* Configure and check the timer. */
  if(ESP_error_check(
       ledc_timer_config(&system_LEDs_infos[system_LEDs_infos[ID].ID].ledc_timer))
                         != ESP_OK)
  {
    return BSP_LED_MODULE_INIT_ERR;
  }

  /* Configure and check the timer channel. */
  if(ESP_error_check(
       ledc_channel_config(&system_LEDs_infos[system_LEDs_infos[ID].ID].ledc_channel))
                           != ESP_OK)
  {
    return BSP_LED_MODULE_INIT_ERR;
  }

  return BSP_LED_OK;
}

LED_return de_init_LED(const LED_ID ID)
{

  CHECK_IF_MODULE_WAS_INTIALIZED;

  if(!check_LED_ID(ID))
  {
    return BSP_LED_DOES_NOT_EXIST_ERR;
  }

  if(!system_LEDs_infos[ID].was_initialized)
  {
    return BSP_LED_WAS_NOT_INIT_ERR;
  }

  if(ESP_error_check(gpio_reset_pin(system_LEDs_infos[ID].GPIO)) != ESP_OK)
  {
    return BSP_LED_MODULE_DE_INIT_ERR;
  }

  if(ESP_error_check(ledc_stop(system_LEDs_infos[ID].ledc_timer.speed_mode, 
      system_LEDs_infos[ID].ledc_channel.channel, 0u) != ESP_OK))
  {
    return BSP_LED_MODULE_DE_INIT_ERR;
  }

  return BSP_LED_OK;
}

LED_return set_LED_state(const LED_ID ID, const uint8_t duty_cycle)
{

  CHECK_IF_MODULE_WAS_INTIALIZED;

  if(!check_LED_ID(ID))
  {
    return BSP_LED_DOES_NOT_EXIST_ERR;
  }

  uint32_t dutyCycle = 0u;
  if(!cacl_pwm_duty(ID, duty_cycle, &dutyCycle))
  {
    /* Imposible to reach this line as it was checked before, defensive code. */
    return BSP_LED_DOES_NOT_EXIST_ERR;
  }

  /* Set duty cycle. */
  if(ESP_error_check(ledc_set_duty(system_LEDs_infos[ID].ledc_timer.speed_mode, 
      system_LEDs_infos[ID].ledc_channel.channel, dutyCycle)) != ESP_OK)
  {
    return BSP_LED_SET_LED_STATE_ERR;
  }

  /* Update duty to apply the new value. */
  if(ESP_error_check(ledc_update_duty(system_LEDs_infos[ID].ledc_timer.speed_mode, 
       system_LEDs_infos[ID].ledc_channel.channel)) != ESP_OK)
  {
    return BSP_LED_SET_LED_STATE_ERR;
  }

  return BSP_LED_OK;
}

LED_return turn_off_LED(const LED_ID ID)
{

  CHECK_IF_MODULE_WAS_INTIALIZED;

  if(!check_LED_ID(ID))
  {
    return BSP_LED_DOES_NOT_EXIST_ERR;
  }

  /* Set duty cycle. */
  if(ESP_error_check(ledc_set_duty(system_LEDs_infos[ID].ledc_timer.speed_mode, 
      system_LEDs_infos[ID].ledc_channel.channel, 0u)) != ESP_OK)
  {
    return BSP_LED_SET_LED_STATE_ERR;
  }

  /* Update duty to apply the new value. */
  if(ESP_error_check(ledc_update_duty(system_LEDs_infos[ID].ledc_timer.speed_mode, 
       system_LEDs_infos[ID].ledc_channel.channel)) != ESP_OK)
  {
    return BSP_LED_SET_LED_STATE_ERR;
  }

  return BSP_LED_OK;
}

inline LED_return BSP_LED_LOG(const LED_return ret)
{
  #if DEBUG_MODE_ENABLE == 1
    switch(ret)
    {
      #define LED_RETURN(enumerate)    \
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
        LED_RETURNS
      #undef LED_RETURN
      default:
        ESP_LOGE(TAG, "Unkown return.");
        break;
    }
  #endif
  return ret;
}

static bool check_configurations_sort(void)
{

  system_LED_info LEDs_info[NUM_OF_LEDS] = {0u};

  for(LED_ID i = 0u; i < NUM_OF_LEDS; i++)
  {

    if(!check_LED_ID(system_LEDs_infos[i].ID))
    {
      return false;
    }

    /* Check if the given GPIO is valid. */
    if(!GPIO_IS_VALID_GPIO(system_LEDs_infos[i].GPIO))
    {
      return false;
    }

    /* Check if the pull mode is valid. */
    switch(system_LEDs_infos[i].pull_mode)
    {
      case GPIO_PULLUP_ONLY: break;
      case GPIO_PULLDOWN_ONLY: break;
      case GPIO_PULLUP_PULLDOWN: break;
      case GPIO_FLOATING: break;
      default: 
        return false;
    }
    
    /* Ensure that is in the correct index. */
    LEDs_info[system_LEDs_infos[i].ID] = system_LEDs_infos[i];

  }

  for(LED_ID i = 0u; i < NUM_OF_LEDS; i++)
  {
    /* Copy the sorted array in the global. */
    system_LEDs_infos[i] = LEDs_info[i];
  }

  return true;
}

static bool check_LED_ID(const LED_ID ID)
{
  switch(ID)
  {
    #define LED(LED_ID)         \
      case LED_ID: return true;   
      LEDS
    #undef LED
    default: 
      return false;
  }
}

static bool cacl_pwm_duty(const LED_ID ID, const uint8_t dutyPercentage, 
  uint32_t *dutyInSteps)
{

  uint8_t dutyPerc = dutyPercentage;
  if(dutyPerc > MAX_DUTY_CYCLE_PERC)
  {
    dutyPerc = MAX_DUTY_CYCLE_PERC;
  }
  else if(dutyPerc < MIN_DUTY_CYCLE_PERC)
  {
    dutyPerc = MIN_DUTY_CYCLE_PERC;
  }

  const float duty = (float)dutyPerc/100.0f;

  switch(ID)
  {
    #define LED_CONFIG(LED_ID, LED_GPIO_ID, LED_GPIO_PULL_MODE, LED_TIMER, PWM_SPEED, \
                       PWM_CHAN, PWM_RESOL, PWM_FREQ)                                 \
      case LED_ID:                                                                    \
        /* (2^Resolution) * percetange */                                             \
        *dutyInSteps = (((1ull << PWM_RESOL) - 1ull)) * duty;                         \
        break;                                                                        
      LED_CONFIGURATIONS                                                              
    #undef LED_CONFIG 
    default:
      /* Unkown LED. */
      return false;
  }

  return true;
}