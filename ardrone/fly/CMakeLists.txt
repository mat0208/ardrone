cmake_minimum_required(VERSION 2.6)

SET( CMAKE_C_COMPILER arm-none-linux-gnueabi-gcc )
SET( CMAKE_C_FLAGS "-std=c99 -Wall -O2")

link_libraries(m)
link_libraries(pthread)


SET( FLY_SOURCES 	main_fly.c
	controlthread.c
	control_strategies/pid_strategy.c
	control_strategies/pid.c
	controls.c
	../navboard/navboard.c
	../util/util.c
	../gpio/gpio.c
	../motorboard/motorboard.c
	../motorboard/mot.c
	../udp/udp.c
	../attitude/attitude.c
	../attitude/ars.c
	../horizontal_velocities/horizontal_velocities.c
	../video/video.c
	../video/blocksum.c
)

add_executable(fly
	${FLY_SOURCES}
)

add_executable(nofly
	${FLY_SOURCES}
)
SET_TARGET_PROPERTIES (nofly PROPERTIES COMPILE_FLAGS "-DMOTOR_DRY_RUN")


	

