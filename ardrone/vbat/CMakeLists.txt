cmake_minimum_required(VERSION 2.6)

SET( CMAKE_C_COMPILER arm-none-linux-gnueabi-gcc )

add_executable(vbat
	vbat.c
	main_vbat.c
)

	

