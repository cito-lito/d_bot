# SPDX-License-Identifier: Apache-2.0
set(BOARD "nucleo_f446re")
cmake_minimum_required(VERSION 3.13.1)

set(CONF_FILE ${CMAKE_CURRENT_SOURCE_DIR}/prj.conf)
set(DTC_OVERLAY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/prj.overlay)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(d_boot)

#includes
include_directories("${CMAKE_SOURCE_DIR}/src/motors")
include_directories("${CMAKE_SOURCE_DIR}/src/ir_sensors")
include_directories("${CMAKE_SOURCE_DIR}/src/color_sensor")
include_directories("${CMAKE_SOURCE_DIR}/src/usonic")
include_directories("${CMAKE_SOURCE_DIR}/src/d_bot_main")

target_sources(app 
		PRIVATE src/motors/motors_engine.c
		PRIVATE src/d_bot_main/d_bot_main.c
		PRIVATE src/ir_sensors/ir_sensors.c
		PRIVATE src/color_sensor/tcs3200.c
		PRIVATE src/usonic/usonic.c
		)
