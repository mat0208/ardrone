SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   arm-none-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER arm-none-linux-gnueabi-g++)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv7-a -mtune=cortex-a8 -mfpu=neon -ftree-vectorize -ffast-math -mfloat-abi=softfp")
