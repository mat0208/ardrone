cmake_minimum_required(VERSION 2.6)

SET( CMAKE_C_COMPILER arm-none-linux-gnueabi-gcc )

link_libraries(m)

add_executable(gpio
	../util/util.c
	gpio.c
	main_gpio.c
)

	

