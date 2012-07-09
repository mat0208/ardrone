cmake_minimum_required(VERSION 2.6)


link_libraries(pthread)
link_libraries(boost_system)
link_libraries(boost_program_options)


add_executable(Console
	Console.cpp
	Joystick.cpp
)

	

