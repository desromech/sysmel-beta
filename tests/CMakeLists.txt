if(NOT WIN32)
    set(EXECUTABLE_OUTPUT_PATH "${Sysmel_BINARY_DIR}/tool-tests")
    set(LIBRARY_OUTPUT_PATH "${Sysmel_BINARY_DIR}/tool-test")
endif()

add_subdirectory(SysmelCompiler)
