set(SysmelC_Sources
    Compiler.cpp
)

add_executable(sysmelc ${SysmelC_Sources})
target_link_libraries(sysmelc SysmelCompiler)
