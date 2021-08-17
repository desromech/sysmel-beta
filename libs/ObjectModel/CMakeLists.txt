set(ObjectModel_Sources
    LargeInteger.cpp
    Object.cpp
    LiteralInteger.cpp
    LiteralNegativeInteger.cpp
    LiteralNumber.cpp
    LiteralFloat.cpp
    LiteralPositiveInteger.cpp
    LiteralCharacter.cpp
    LiteralValue.cpp
    LiteralString.cpp
    LiteralSymbol.cpp
)

add_library(SysmelObjectModel STATIC ${ObjectModel_Sources})
