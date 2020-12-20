set(Parser_Sources
    Sysmel/Scanner.cpp
    Sysmel/Parser.cpp
)

set(DOM_Sources
    DOM.cpp
)

set(Compiler_Sources
    ${Parser_Sources}
    ${DOM_Sources}
)

add_library(SysmelBaseCompiler STATIC ${Compiler_Sources})
