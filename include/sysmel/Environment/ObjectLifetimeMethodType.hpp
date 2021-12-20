#ifndef SYSMEL_ENVIRONMENT_OBJECT_LIFETIME_METHOD_TYPE_HPP
#define SYSMEL_ENVIRONMENT_OBJECT_LIFETIME_METHOD_TYPE_HPP
#pragma once

#include <stdint.h>

namespace Sysmel
{
namespace Environment
{

/**
 * I represent a specific special object lifetime method.
 */
enum class ObjectLifetimeMethodType : uint8_t
{
    Initialize = 0,
    InitializeCopyingFrom,
    InitializeMovingFrom,
    AssignCopyingFrom,
    AssignMovingFrom,
    Finalize,

};

inline const char *objectLifetimeMethodTypeToString(ObjectLifetimeMethodType type)
{
    switch(type)
    {
    case ObjectLifetimeMethodType::Initialize: return "initialize";
    case ObjectLifetimeMethodType::InitializeCopyingFrom: return "initializeCopyingFrom";
    case ObjectLifetimeMethodType::InitializeMovingFrom: return "initializeMovingFrom";
    case ObjectLifetimeMethodType::AssignCopyingFrom: return "assignCopyingFrom";
    case ObjectLifetimeMethodType::AssignMovingFrom: return "assignMovingFrom";
    case ObjectLifetimeMethodType::Finalize: return "finalize";
    }
}


} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_OBJECT_LIFETIME_METHOD_TYPE_HPP