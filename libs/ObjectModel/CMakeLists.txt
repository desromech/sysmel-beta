set(ObjectModel_Sources
    LargeInteger.cpp
    AnyValue.cpp
    BootstrapMethod.cpp
    BootstrapModule.cpp
    BootstrapType.cpp
    BootstrapTypeRegistration.cpp
    LiteralInteger.cpp
    LiteralNegativeInteger.cpp
    LiteralNumber.cpp
    LiteralFloat.cpp
    LiteralPositiveInteger.cpp
    LiteralCharacter.cpp
    LiteralValue.cpp
    LiteralString.cpp
    LiteralSymbol.cpp
    Module.cpp
    ModuleReference.cpp
    ProgramEntity.cpp
    RuntimeContext.cpp
    TranslationUnitModule.cpp
    Type.cpp
    Wrappers.cpp
)

add_library(SysmelObjectModel SHARED ${ObjectModel_Sources})
