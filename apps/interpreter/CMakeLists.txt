set(SysmelI_Sources
    Interpreter.cpp
)

add_executable(sysmeli ${SysmelI_Sources})
target_link_libraries(sysmeli SysmelCompiler)
