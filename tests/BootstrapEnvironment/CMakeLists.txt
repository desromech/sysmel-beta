set(CompilerTests_Sources
    Main.cpp
    LargeInteger.cpp
    LiteralNumber.cpp
    
    BootstrapModule.cpp
    Wrappers.cpp
    Type.cpp
)

add_executable(BootstrapEnvironmentTests ${CompilerTests_Sources})
target_link_libraries(BootstrapEnvironmentTests UnitTest++ SysmelBootstrapEnvironment)

add_test(NAME RunBootstrapEnvironmentTests COMMAND BootstrapEnvironmentTests)
