add_library(repl_interpreter CommandInterpreterImpl.cpp)
target_include_directories(repl_interpreter PUBLIC include)

include(GenerateExportHeader)
set(INTERPRETER_EXPORT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include/interpreter")
generate_export_header(repl_interpreter EXPORT_FILE_NAME "${INTERPRETER_EXPORT_DIRECTORY}/interpreter_export.h")
