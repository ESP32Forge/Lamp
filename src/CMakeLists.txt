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

# General BSP sources.
set(SOURCE_BSP ${BSP_BUTTON_SOURCE_PATH}/Button.c)

# General include for BSP headers.
set(INC_BSP ${BSP_PHYSICAL_CONNECTION_SOURCE_PATH} ${BSP_BUTTON_SOURCE_PATH})

###########
#  CORE   #
###########

# Path to the Core folder.
set(CORE_SOURCE_PATH ${SOURCES_ROOT_PATH}/Core)

# Path to the Core debug folder.
set(CORE_DEBUG_FOLDER ${CORE_SOURCE_PATH}/Debug)

# General Core sources.
set(SOURCE_CORE ${CORE_DEBUG_FOLDER}/Debug.c)

# General include for Core headers.
set(INC_CORE ${CORE_DEBUG_FOLDER})

###########
#   REG   #
###########

idf_component_register(SRCS ${SOURCES_ROOT_PATH}/main.c ${SOURCE_BSP} ${SOURCE_CORE}          
                       INCLUDE_DIRS ${INC_CORE} ${INC_BSP})
