set(CompilerTests_Sources
    Main.cpp
    LargeInteger.cpp
    LiteralNumber.cpp
    
    BootstrapModule.cpp
)

add_executable(ObjectModelTests ${CompilerTests_Sources})
target_link_libraries(ObjectModelTests UnitTest++ SysmelObjectModel)

add_test(NAME RunObjectModelTests COMMAND ObjectModelTests)
