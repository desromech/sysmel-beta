set(Parser_Sources
    Sysmel/Scanner.cpp
    Sysmel/Parser.cpp
)

set(DOM_Sources
    DOM.cpp
)

set(SObjectSchemaCompiler_Sources
    SObjectSchemaParser.cpp
)

set(Compiler_Sources
    ${Parser_Sources}
    ${DOM_Sources}
    ${SObjectSchemaCompiler_Sources}
)

add_library(SysmelBaseCompiler STATIC ${Compiler_Sources})

add_executable(SObjectSchemaCompiler SObjectSchemaCompilerDriver.cpp)
target_link_libraries(SObjectSchemaCompiler SysmelBaseCompiler)
