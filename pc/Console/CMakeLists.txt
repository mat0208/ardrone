cmake_minimum_required(VERSION 2.6)


link_libraries(pthread)
link_libraries(boost_system)
link_libraries(boost_program_options)
link_libraries(boost_date_time)

add_executable(Console
	Console.cpp
	Joystick.cpp
	Keyboard.cpp
)
