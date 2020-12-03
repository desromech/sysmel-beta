set(Parser_Sources
    Sysmel/Scanner.cpp
    Sysmel/Parser.cpp
)

set(Compiler_Sources
    ${Parser_Sources}
)

add_library(SysmelCompiler STATIC ${Compiler_Sources})
