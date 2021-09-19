#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_TYPE_REGISTRATION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_TYPE_REGISTRATION_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

typedef std::vector<StaticBootstrapDefinedTypeMetadata*> StaticBootstrapDefinedTypeMetadataList;
const StaticBootstrapDefinedTypeMetadataList &getBootstrapDefinedTypeMetadataList();
void registerBootstrapDefinedTypeMetadata(StaticBootstrapDefinedTypeMetadata *metadata);

template<typename T>
struct BootstrapTypeRegistration
{
    BootstrapTypeRegistration()
    {
        registerBootstrapDefinedTypeMetadata(StaticBootstrapDefinedTypeMetadataFor<T>::get());
    }
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_TYPE_REGISTRATION_HPP