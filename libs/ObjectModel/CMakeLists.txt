set(ObjectModel_Sources
    Object.cpp
    SObjectSchema.cpp
    ObjectEnvironment.cpp
    ObjectEnvironmentBuilders.cpp
)

add_library(SysmelObjectModel STATIC ${ObjectModel_Sources})
