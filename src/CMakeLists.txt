###########
# SOURCES #
###########

# Root of the sources path.
set(SOURCES_ROOT_PATH ${CMAKE_SOURCE_DIR}/src)

###########
#   BSP   #
###########

# Path to the BSP folder.
set(BSP_SOURCE_PATH ${SOURCES_ROOT_PATH}/BSP)

# Path to the BSP physical connection folder.
set(BSP_PHYSICAL_CONNECTION_SOURCE_PATH ${BSP_SOURCE_PATH}/BSP_physical_connection)

# Path to the BSP Button folder.
set(BSP_BUTTON_SOURCE_PATH ${BSP_SOURCE_PATH}/Button)

# Path to the BSP LED folder.
set(BSP_LED_SOURCE_PATH ${BSP_SOURCE_PATH}/LED)

# General BSP sources.
set(SOURCE_BSP ${BSP_BUTTON_SOURCE_PATH}/Button.c ${BSP_LED_SOURCE_PATH}/LED.c)

# General include for BSP headers.
set(INC_BSP ${BSP_PHYSICAL_CONNECTION_SOURCE_PATH} ${BSP_BUTTON_SOURCE_PATH} ${BSP_LED_SOURCE_PATH})

###########
#  CORE   #
###########

# Path to the Core folder.
set(CORE_SOURCE_PATH ${SOURCES_ROOT_PATH}/Core)

# Path to the Core debug folder.
set(CORE_DEBUG_FOLDER ${CORE_SOURCE_PATH}/Debug)

# Path to the Core lamp folder.
set(CORE_LAMP_FOLDER ${CORE_SOURCE_PATH}/Lamp)

# Path to the Core WiFi folder.
set(CORE_WIFI_FOLDER ${CORE_SOURCE_PATH}/WiFi)

# Path to the Core TCP client folder.
set(CORE_TCP_SERVER_FOLDER ${CORE_SOURCE_PATH}/TCP_server)

# Path to the Core System config folder.
set(CORE_SYSTEM_CONFIG_FOLDER ${CORE_SOURCE_PATH}/System_config)

# General Core sources.
set(SOURCE_CORE ${CORE_DEBUG_FOLDER}/Debug.c ${CORE_LAMP_FOLDER}/Lamp.c ${CORE_WIFI_FOLDER}/WiFi.c ${CORE_TCP_SERVER_FOLDER}/TCP_server.c)

# General include for Core headers.
set(INC_CORE ${CORE_DEBUG_FOLDER} ${CORE_LAMP_FOLDER} ${CORE_WIFI_FOLDER} ${CORE_TCP_SERVER_FOLDER} ${CORE_SYSTEM_CONFIG_FOLDER})

###########
#   REG   #
###########

idf_component_register(SRCS ${SOURCES_ROOT_PATH}/main.c ${SOURCE_BSP} ${SOURCE_CORE}          
                       INCLUDE_DIRS ${INC_CORE} ${INC_BSP})
