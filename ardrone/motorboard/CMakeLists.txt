cmake_minimum_required(VERSION 2.6)

SET( CMAKE_C_COMPILER arm-none-linux-gnueabi-gcc )
SET( CMAKE_C_FLAGS "-std=c99 -Wall")


link_libraries(m)
link_libraries(pthread)

add_executable(motorboard
	main_motorboard.c
	motorboard.c
	mot.c
	../util/util.c
	../gpio/gpio.c
)

	

