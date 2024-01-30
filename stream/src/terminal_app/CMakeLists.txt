
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
add_executable(terminal terminal.cpp)
target_link_libraries(terminal PRIVATE cmd_interpreter)
