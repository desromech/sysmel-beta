set(SysmelC_Sources
    Compiler.cpp
)

add_executable(SysmelC ${SysmelC_Sources})
target_link_libraries(SysmelC SysmelCompiler)
