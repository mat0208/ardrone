cmake_minimum_required(VERSION 2.6)

SET( CMAKE_C_COMPILER arm-none-linux-gnueabi-gcc )
SET( CMAKE_C_FLAGS "-std=c99 -Wall -O2")


link_libraries(m)
link_libraries(pthread)

add_executable(horizontal_velocities
	horizontal_velocities.c
	main_horizontal_velocities.c
	../util/util.c
	../video/video.c
	../video/blocksum.c
	../attitude/attitude.c
	../attitude/ars.c
	../navboard/navboard.c
	../gpio/gpio.c
)

	

