#ifndef SYSMEL_COMPILER_OBJECT_MODEL_BOOTSTRAP_TYPE_REGISTRATION_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_BOOTSTRAP_TYPE_REGISTRATION_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

typedef std::vector<const StaticBootstrapDefinedTypeMetadata*> StaticBootstrapDefinedTypeMetadataList;
const StaticBootstrapDefinedTypeMetadataList &getBootstrapDefinedTypeMetadataList();
void registerBootstrapDefinedTypeMetadata(const StaticBootstrapDefinedTypeMetadata *metadata);

template<typename T>
struct BootstrapTypeRegistration
{
    BootstrapTypeRegistration()
    {
        registerBootstrapDefinedTypeMetadata(StaticBootstrapDefinedTypeMetadataFor<T>::get());
    }
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_BOOTSTRAP_TYPE_REGISTRATION_HPP