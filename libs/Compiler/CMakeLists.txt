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

add_library(SysmelCompiler STATIC ${Compiler_Sources})
target_link_libraries(SysmelCompiler SysmelObjectModel)
