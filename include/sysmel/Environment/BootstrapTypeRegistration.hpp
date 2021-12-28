#ifndef SYSMEL_ENVIRONMENT_BOOTSTRAP_TYPE_REGISTRATION_HPP
#define SYSMEL_ENVIRONMENT_BOOTSTRAP_TYPE_REGISTRATION_HPP
#pragma once

#include "AnyValue.hpp"

namespace Sysmel
{
namespace Environment
{

typedef std::vector<StaticBootstrapDefinedTypeMetadata*> StaticBootstrapDefinedTypeMetadataList;
SYSMEL_COMPILER_LIB_EXPORT const StaticBootstrapDefinedTypeMetadataList &getBootstrapDefinedTypeMetadataList();
SYSMEL_COMPILER_LIB_EXPORT void registerBootstrapDefinedTypeMetadata(StaticBootstrapDefinedTypeMetadata *metadata);

template<typename T>
struct BootstrapTypeRegistration
{
    BootstrapTypeRegistration()
    {
        registerBootstrapDefinedTypeMetadata(StaticBootstrapDefinedTypeMetadataFor<T>::get());
    }
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_BOOTSTRAP_TYPE_REGISTRATION_HPP