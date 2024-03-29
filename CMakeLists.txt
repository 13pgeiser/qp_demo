cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME qp_demo)
project(${PROJECT_NAME})

message("Configuring with CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(ExternalProject)

# Include location of QP
include(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)
add_definitions(-DQ_EVT_CTOR)

# Specific to ARM
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm")
  set(CORE_FLAGS "-DSTM32F429xx -march=armv7-m -mtune=cortex-m4 -Os --specs=nano.specs --specs=nosys.specs")
  set(CMAKE_C_FLAGS "${CORE_FLAGS} -fno-builtin -Wall -std=gnu99 -fdata-sections -ffunction-sections -g3 -gdwarf-2")
  set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -nodefaultlibs -Wl,-Map,${PROJECT_NAME}.map,--gc-sections -T${CMAKE_CURRENT_SOURCE_DIR}/src/arm/STM32F429.ld")
  set(QP_PORT ${QPC_FOLDER}/ports/arm-cm/qk/gnu)
  set(CMSIS STM32CubeF4/Drivers/CMSIS)
  list(APPEND INCLUDE_DIRS
    ${CMSIS}/Device/ST/STM32F4xx/Include
    ${CMSIS}/Core/Include
    STM32CubeF4/Drivers/BSP/STM32F429I-Discovery
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc
    src/arm
    )
  list(APPEND EXT_SOURCES
    ${QP_PORT}/qk_port.c
    ${QPC_FOLDER}/src/qk/qk.c
    ${CMSIS}/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c
    STM32CubeF4/Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery.c
    STM32CubeF4/Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_lcd.c
    STM32CubeF4/Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_sdram.c
    STM32CubeF4/Drivers/BSP/Components/ili9341/ili9341.c
    )
  list(APPEND SOURCES
    src/arm/startup_stm32f429.c
    src/arm/bsp.c
    )
else()
  list(APPEND EXT_SOURCES
    ${QP_PORT}/qf_port.c
    )
  list(APPEND SOURCES
    src/host/bsp.c
    )
endif()

# Shared
list(APPEND INCLUDE_DIRS
  ${QPC_FOLDER}/include
  ${QPC_FOLDER}/src
  ${QP_PORT}
  model/generated
  src/
  )

list(APPEND EXT_SOURCES
  ${QPC_FOLDER}/src/qf/qep_hsm.c
  ${QPC_FOLDER}/src/qf/qep_msm.c
  ${QPC_FOLDER}/src/qf/qf_act.c
  ${QPC_FOLDER}/src/qf/qf_actq.c
  ${QPC_FOLDER}/src/qf/qf_defer.c
  ${QPC_FOLDER}/src/qf/qf_dyn.c
  ${QPC_FOLDER}/src/qf/qf_mem.c
  ${QPC_FOLDER}/src/qf/qf_ps.c
  ${QPC_FOLDER}/src/qf/qf_qact.c
  ${QPC_FOLDER}/src/qf/qf_qeq.c
  ${QPC_FOLDER}/src/qf/qf_qmact.c
  ${QPC_FOLDER}/src/qf/qf_time.c
  model/generated/blink_leds.c
  model/generated/main_application.c
  )

list(APPEND SOURCES
  src/main.c
  )
include_directories(${INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SOURCES} ${EXT_SOURCES})
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm")
  target_link_libraries(${PROJECT_NAME} c)
  target_link_libraries(${PROJECT_NAME} gcc)
  if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    set(STLINK "C:\\Program Files (x86)\\STMicroelectronics\\STM32 ST-LINK Utility\\ST-LINK Utility\\ST-LINK_CLI.exe")
    add_custom_command(TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_OBJCOPY}
      ARGS -O srec ${PROJECT_NAME} ${PROJECT_NAME}.srec)
    add_custom_target(run
      COMMAND ${STLINK} -c SWD -V -P ${PROJECT_NAME}.srec
      COMMAND ${STLINK} -c SWD -Rst)
  else()
    add_custom_command(TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_OBJCOPY}
      ARGS -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin)
    add_custom_target(run
      COMMAND st-flash write ${PROJECT_NAME}.bin 0x8000000
      COMMAND st-flash reset)
  endif()
else()
  if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_custom_target(run
      COMMAND ./${PROJECT_NAME})
  else()
    add_custom_target(run
      COMMAND winpty ./${PROJECT_NAME})
  endif()
endif()

add_dependencies(run ${PROJECT_NAME})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  find_package(Threads REQUIRED)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  target_link_libraries(${PROJECT_NAME} Threads::Threads)
endif()


ExternalProject_Add(
  astyle
  SVN_REPOSITORY      http://svn.code.sf.net/p/astyle/code/trunk/AStyle/
  SVN_REVISION        -r672
  CONFIGURE_COMMAND   pwd
  BUILD_COMMAND       make -C build/gcc
  INSTALL_COMMAND     make -C build/gcc install prefix="${CMAKE_BINARY_DIR}" INSTALL=install
  PREFIX              ${CMAKE_BINARY_DIR}/external/astyle/prefix
  SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/astyle
  BINARY_DIR          ${CMAKE_BINARY_DIR}/external/astyle
  EXCLUDE_FROM_ALL    1
  )

list(APPEND ASTYLE_ARGS
  --style=google
  --lineend=linux
  --suffix=none
  --convert-tabs
  --indent=spaces=2
  --pad-oper
  --indent-preprocessor
  --indent-switches
  --indent-classes
  --indent-cases
  --indent-col1-comments
  --max-instatement-indent=100
  )

list(APPEND CPPCHECK_CMAKE_ARGS
  "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
  "-DFILESDIR=${CMAKE_BINARY_DIR}/bin"
  )

ExternalProject_Add(
  cppcheck
  GIT_REPOSITORY      https://github.com/danmar/cppcheck.git
  GIT_TAG             2.13.0
  GIT_SHALLOW         1
  CMAKE_ARGS          ${CPPCHECK_CMAKE_ARGS}
  PREFIX              ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
  TMP_DIR             ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
  STAMP_DIR           ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
  DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/cppcheck/download
  SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/src
  BINARY_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/build
  EXCLUDE_FROM_ALL    1
  )

list(APPEND CPPCHECK_ARGS
  --enable=warning,style,performance,portability
  --std=c++11
  --verbose
  --error-exitcode=1
  --language=c++
  --inline-suppr
  -DMAIN=main
  -I ${CMAKE_SOURCE_DIR}/src
  -I ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src
  )

add_custom_target(
  check
  COMMAND ${CMAKE_BINARY_DIR}/bin/astyle ${ASTYLE_ARGS} ${SOURCES}
  COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS} ${SOURCES}
  )

add_dependencies(check astyle cppcheck)

add_custom_target(
  mrproper
  COMMAND rm -rf bin/ CMakeFiles/ external/ share/
  COMMAND rm -rf model/generated/
  COMMAND rm -f CMakeCache.txt cmake_install.cmake compile_commands.json config.cmake qp_demo* Makefile sourceme
  )
