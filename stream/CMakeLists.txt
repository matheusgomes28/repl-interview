cmake_minimum_required(VERSION 3.28)
project(CppReplInterview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_subdirectory(src/cmd_interpreter)
add_subdirectory(src/terminal_app)
add_subdirectory(tests)
