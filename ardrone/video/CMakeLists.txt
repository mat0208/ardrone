cmake_minimum_required(VERSION 2.6)

SET( CMAKE_C_COMPILER arm-none-linux-gnueabi-gcc )
SET( CMAKE_C_FLAGS "-std=c99 -Wall")

link_libraries(pthread)

add_executable(video
	video.c
	main_video.c
	../util/util.c
)

	

