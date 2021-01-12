set(CompilerTests_Sources
    Main.cpp
    ScannerTests.cpp
    ParserTests.cpp
    DOMTests.cpp
)

add_executable(CompilerTests ${CompilerTests_Sources})
target_link_libraries(CompilerTests UnitTest++ SysmelCompiler)

add_test(NAME RunCompilerTests COMMAND CompilerTests)
