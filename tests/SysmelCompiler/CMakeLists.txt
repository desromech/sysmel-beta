set(EnvironmentTests_Sources
    LargeInteger.cpp
    LiteralNumber.cpp
    
    BootstrapModule.cpp
    Wrappers.cpp
    Type.cpp
    StringUtilities.cpp
)

list(TRANSFORM EnvironmentTests_Sources PREPEND "Environment/")

set(FrontendTests_Sources
    ScannerTests.cpp
    ParserTests.cpp
    EvaluationTests.cpp
)

list(TRANSFORM FrontendTests_Sources PREPEND "Frontend/")

add_executable(SysmelCompilerTests Main.cpp ${EnvironmentTests_Sources} ${FrontendTests_Sources})
target_link_libraries(SysmelCompilerTests UnitTest++ SysmelCompiler)

add_test(NAME RunSysmelCompilerTests COMMAND SysmelCompilerTests)
