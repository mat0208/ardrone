cmake_minimum_required(VERSION 2.6)

SET( CMAKE_CXX_FLAGS "-g")
SET( CMAKE_CXX_FLAGS_DEBUG "-g" )
SET( CMAKE_C_COMPILER arm-none-linux-gnueabi-gcc )

link_libraries(m)

add_executable(navboard
	navboard.c
	main_navboard.c
	../util/util.c
	../gpio/gpio.c
)

	

