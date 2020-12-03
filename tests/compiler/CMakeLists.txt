set(CompilerTests_Sources
    Main.cpp
    ScannerTests.cpp
    ParserTests.cpp
)

add_executable(CompilerTests ${CompilerTests_Sources})
target_link_libraries(CompilerTests UnitTest++ SysmelCompiler)


add_custom_command(TARGET CompilerTests
    POST_BUILD COMMAND CompilerTests
    COMMENT "Running unit tests for the compiler"
)
