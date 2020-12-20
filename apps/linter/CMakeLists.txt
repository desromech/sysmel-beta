set(SysmelLinter_Sources
    Linter.cpp
)

add_executable(SysmelLinter ${SysmelLinter_Sources})
target_link_libraries(SysmelLinter SysmelBaseCompiler)
