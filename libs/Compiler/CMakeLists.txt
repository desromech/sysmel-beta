set(Parser_Sources
    Sysmel/Scanner.cpp
    Sysmel/Parser.cpp
)

set(Compiler_Sources
    ${Parser_Sources}
)

add_library(SysmelCompiler SHARED ${Compiler_Sources})
target_link_libraries(SysmelCompiler SysmelObjectModel)
