set(ObjectModelTests_Sources
    Main.cpp
    SObjectSchemaTests.cpp
)

add_executable(ObjectModelTests ${ObjectModelTests_Sources})
target_link_libraries(ObjectModelTests UnitTest++ SysmelObjectModel)

add_test(NAME RunObjectModelTests COMMAND ObjectModelTests)
