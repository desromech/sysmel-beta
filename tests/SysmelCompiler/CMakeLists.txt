set(BootstrapEnvironmentTests_Sources
    LargeInteger.cpp
    LiteralNumber.cpp
    
    BootstrapModule.cpp
    Wrappers.cpp
    Type.cpp
    StringUtilities.cpp
)

list(TRANSFORM BootstrapEnvironmentTests_Sources PREPEND "BootstrapEnvironment/")

set(CompilerTests_Sources
    ScannerTests.cpp
    ParserTests.cpp
    EvaluationTests.cpp
)

list(TRANSFORM CompilerTests_Sources PREPEND "Compiler/")

add_executable(SysmelCompilerTests Main.cpp ${BootstrapEnvironmentTests_Sources} ${CompilerTests_Sources})
target_link_libraries(SysmelCompilerTests UnitTest++ SysmelCompiler)

add_test(NAME RunSysmelCompilerTests COMMAND SysmelCompilerTests)
