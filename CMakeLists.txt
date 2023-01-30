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

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(FATAL_ERROR "Targeting 32-bit platforms is not required for this project. Please compile for 64-bit.")
endif()

####################################
# Command Interpreter Library
####################################

# Source files used by the library.
# You may add any extra files that your implementation uses.
add_library(CommandInterpreter SHARED
	src/Control.cpp
	src/CommandInterpreterImpl.cpp
	src/CommandExamples.cpp
)

# Public interface headers for the library should go in this directory.
target_include_directories(CommandInterpreter PUBLIC include)

# Internal implementation headers and sources should go in this directory.
target_include_directories(CommandInterpreter PRIVATE src/include)

# This definition is added when building the library, so that public
# library functions may be exported correctly on Windows (see LibExport.h).
target_compile_definitions(CommandInterpreter PRIVATE -DCMDINT_BUILD_PRODUCER)

# You may add any extra target properties you wish to this section, eg. to specify
# a target C++ standard. Note that third party libraries or other external code
# may not be used in the project.

####################################
# Sanity Tests
####################################

add_executable(SanityTests
	sanity_tests/Main.cpp
)

target_link_libraries(SanityTests PUBLIC CommandInterpreter)

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

if(IS_ROOT_PROJECT)
	# If this project is a root project, allow installing the library to a directory.
	# This means that you can specify -DCMAKE_INSTALL_PREFIX=... and run
	# `cmake --build . --target install` to check how your library implementation
	# will be structured for an end user.
	install(TARGETS CommandInterpreter
		ARCHIVE DESTINATION "CommandInterpreter/lib"
		LIBRARY DESTINATION "CommandInterpreter/lib"
		RUNTIME DESTINATION "CommandInterpreter/bin"
	)

	install(DIRECTORY "include/CommandInterpreter"
		DESTINATION "CommandInterpreter/include"
	)

	# The sanity test executable will also be installed alongside the library.
	install(TARGETS SanityTests
		RUNTIME DESTINATION "SanityTests/bin"
	)

	install(IMPORTED_RUNTIME_ARTIFACTS CommandInterpreter
		RUNTIME DESTINATION "SanityTests/bin"
		LIBRARY DESTINATION "SanityTests/lib"
	)
endif()
