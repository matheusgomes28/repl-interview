cmake_minimum_required(VERSION 3.21)

# Must go before the project() command:
if(NOT DEFINED PROJECT_NAME)
	set(IS_ROOT_PROJECT "YES")
else()
	set(IS_ROOT_PROJECT "NO")
endif()

project(CommandInterpreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED "On")
set(CMAKE_CXX_EXTENSIONS "Off")


# You may add any extra target properties you wish to this section, eg. to specify
# a target C++ standard. Note that third party libraries or other external code
# may not be used in the project.

####################################
# Sanity Tests
####################################

#add_executable(SanityTests
#	sanity_tests/Main.cpp
#)

#target_link_libraries(SanityTests PUBLIC CommandInterpreter)

if (WIN32)
    # I stayed away from exceptions in this implementation, however,
	# the MSVC standard assumes that all extern functions will throw unless
	# the following compiler flag is defined. If not added, the extern definitions
	# coming from <iostream> will be assumed to throw, even when the standard 
	# guarantees they dont. I.e. leave the definition below for the use of
	# std::cout and it's << operator, as it doesn't throw anyway.
    target_compile_options(CommandInterpreter PRIVATE /EHsc)
    target_compile_options(SanityTests PRIVATE /EHsc)
endif()


####################################
# Installation
####################################


add_subdirectory(src/interpreter)
