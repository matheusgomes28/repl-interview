add_executable(repl repl.cpp)

target_link_libraries(repl PRIVATE repl_interpreter)
