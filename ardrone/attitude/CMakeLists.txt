cmake_minimum_required(VERSION 2.6)

SET( CMAKE_C_COMPILER arm-none-linux-gnueabi-gcc )
SET( CMAKE_C_FLAGS "-std=c99 -Wall")


link_libraries(m)

add_executable(attitude
	attitude.c
	main_attitude.c
	../util/util.c
	../gpio/gpio.c
	ars.c
	../navboard/navboard.c
)

	

