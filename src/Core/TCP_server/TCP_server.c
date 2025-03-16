/**
 * @file      TCP_server.c
 * @authors   Álvaro Velasco García
 * @date      March 16, 2025
 *
 * @brief     This source file defines the functions to initialize the WiFi peripheral
 *            in access point mode.
 */

/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <TCP_server.h>
#include <Debug.h>
#include <WiFi.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"

/***************************************************************************************
 * Defines
 ***************************************************************************************/

#if DEBUG_MODE_ENABLE == 1
/* Tag to show traces in button BSP module. */
  #define TAG "CORE_TCP_SERVER"
#endif

/***************************************************************************************
 * Global Variables
 ***************************************************************************************/

/* Handler of the task that initialized the server and listen to new messages. */
TaskHandle_t server_task_handler;

/***************************************************************************************
 * Functions Prototypes
 ***************************************************************************************/

/**
 * @brief Handler of the WiFi events.
 * 
 * @param event_handler_arg Arguments to pass to the handler.
 * 
 * @param event_base Which events should wait this function.
 * 
 * @param event_id Which event happened.
 * 
 * @param event_data Data associated to happened event.
 *
 * @return void
 */
static void WiFi_event_handler(void *event_handler_arg, esp_event_base_t event_base,
  int32_t event_id, void *event_data);

/**
 * @brief Function that will initialize the server and wait for messages.
 *
 * @param args arguments to pass to the function.
 *
 * @return void
 */
static void server_task_func(void *args);

/***************************************************************************************
 * Functions
 ***************************************************************************************/

TCP_server_return init_TCP_server(void)
{

  /* Confiuration of the WiFi peripheral. */
  const wifi_config_t config = 
  {
    .ap =
    {
      .ssid = WIFI_SSID,
      .ssid_len = strlen(WIFI_SSID),
      .channel = WIFI_CHANNEL,
      .password = WIFI_PASS,
      .max_connection = MAX_STA_CONN,
      .authmode = WIFI_AUTH_MODE,
      .pmf_cfg = 
      {
        .required = false,
      },
    },
  };

  /* WiFi and IP events handlers. */
  const event_handlers AP_handlers =
  {
    .WiFi_events_to_handle = ESP_EVENT_ANY_ID,
    .WiFi_event_handler = WiFi_event_handler,
    
    .IP_event_handler = NULL,
  };

  /* Start WiFi in AP and station mode. */
  if(core_WiFi_LOG(WiFi_init(WIFI_MODE_AP, config, AP_handlers) != CORE_WIFI_OK))
  {
    return CORE_TCP_SERVER_INIT_ERR;
  }

  return CORE_TCP_SERVER_OK;
}

TCP_server_return de_init_TCP_server(void)
{
  if(core_WiFi_LOG(de_init_WiFi()) != ESP_OK)
  {
    return CORE_TCP_SERVER_DE_INIT_ERR;
  }
  return CORE_TCP_SERVER_OK;
}

inline TCP_server_return core_TCP_server_LOG(const TCP_server_return ret)
{
  #if DEBUG_MODE_ENABLE == 1
    switch(ret)
    {
      #define TCP_SERVER_RETURN(enumerate) \
        case enumerate:                    \
          if(ret > 0)                      \
          {                                \
            ESP_LOGE(TAG, #enumerate);     \
          }                                \
          else                             \
          {                                \
            ESP_LOGI(TAG, #enumerate);     \
          }                                \
          break;       
        TCP_SERVER_RETURNS
      #undef TCP_SERVER_RETURN
      default:
        ESP_LOGE(TAG, "Unkown return.");
        break;
    }
  #endif
  return ret;
}

static void server_task_func(void *args)
{

  int conn_fd, listening_sock;
  struct sockaddr_in addrs_to_listen, source_addr;
  socklen_t source_addr_len = sizeof(source_addr);
  TCP_COMMAND_TYPE cmd;
  char buf[TCP_COMMAND_SIZE] = "";

  /** Set which kind of addresses server will listen. **/
  /* Set IPV4. */
  addrs_to_listen.sin_family = AF_INET;
  /* Accept any IP. */
  addrs_to_listen.sin_addr.s_addr = htonl(INADDR_ANY);
  /* Set the port defined in Network_config.h */
  addrs_to_listen.sin_port = htons(TCP_IP_PORT);

  /* Create listening socket and verify the initialization . */
  listening_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(listening_sock < 0) 
  { 
    #if DEBUG_MODE_ENABLE == 1
      ESP_LOGE(TAG, "Unable tto create socket: errno %d", errno);
    #endif
    vTaskDelete(NULL);
  }

  /* Binding newly created socket to given IP, verification. */
  if(bind(listening_sock, (struct sockaddr*)&addrs_to_listen, 
     sizeof(addrs_to_listen)) != 0) 
  { 
    #if DEBUG_MODE_ENABLE == 1
      ESP_LOGE(TAG, "Socket bind failed: errno %d", errno);
    #endif
    vTaskDelete(NULL);
  } 

  /* At this point server is ready to listen, verification. */
  if(listen(listening_sock, MAX_STA_CONN) != 0) 
  { 
    #if DEBUG_MODE_ENABLE == 1
      ESP_LOGE(TAG, "Listen socket failed: errno %d", errno);
    #endif
    vTaskDelete(NULL);
  } 

  while(true)
  {
    bzero((void*)&cmd, sizeof(cmd));

    /* Accept the data packet from client and verification. */
    conn_fd = accept(listening_sock, (struct sockaddr*)&source_addr, &source_addr_len); 
    if(conn_fd < 0) 
    { 
      #if DEBUG_MODE_ENABLE == 1
        ESP_LOGE(TAG, "Accept socket failed: errno %d", errno);
      #endif
    }

    if(read(conn_fd, (void*)buf, TCP_COMMAND_SIZE) < 0)
    {
      #if DEBUG_MODE_ENABLE == 1
        ESP_LOGE(TAG, "Read socket failed: errno %d", errno);
      #endif
    }

    /* Means GUI want to toggle the LED. */
    /* TODO: Allow GUI to do more actions. */
    if(memcmp((void*)buf, "GUI", 3) == 0)
    {  
      cmd.ID = LED_0;
      cmd.action = TOOGLE_LED;
      RX_command_frame(cmd);
    }
    else
    {
      memcpy((void*)&cmd, (void*)buf, TCP_COMMAND_SIZE);
      RX_command_frame(cmd);
    }
    shutdown(conn_fd, 0);
    close(conn_fd);
  }

  close(listening_sock);
  vTaskDelete(NULL);

}

static void WiFi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
  void* event_data)
{
  switch(event_id)
  {
    case WIFI_EVENT_AP_STACONNECTED:
    {
      #if DEBUG_MODE_ENABLE == 1
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d", MAC2STR(event->mac), event->aid);
      #endif
      break;
    }
    case WIFI_EVENT_AP_STADISCONNECTED:
    {
      #if DEBUG_MODE_ENABLE == 1
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d, reason=%d", MAC2STR(event->mac), 
          event->aid, event->reason);
      #endif
      break;
    }

    case WIFI_EVENT_AP_START:
    {
      /* Create the server task. */
      const BaseType_t ret = xTaskCreate(server_task_func, "server_task", 2048,
        (void *) 0, configMAX_PRIORITIES-1, &server_task_handler);
      if(ret != pdPASS)
      {
        /* TODO: Implement mechanisim to handle this corner case */
      }
    }
    break;
    case WIFI_EVENT_AP_STOP:
    {
      /* Delete the server task. */
      if(server_task_handler != NULL)
      {
        vTaskDelete(server_task_handler);
        server_task_handler = NULL;
      }
    }
    break;
    default:
      break;
  }
}


