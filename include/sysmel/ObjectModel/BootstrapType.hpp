#ifndef SYSMEL_COMPILER_OBJECT_MODEL_BOOTSTRAP_TYPE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_BOOTSTRAP_TYPE_HPP
#pragma once

#include "Type.hpp"
#include <unordered_map>

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the base interface for a type that is specifically defined by the bootstrap environment.
 */
class BootstrapType : public SubtypeOf<Type, BootstrapType>
{
public:
    static constexpr char const __typeName__[] = "BootstrapType";

    void initializeWithMetadata(const StaticBootstrapDefinedTypeMetadata *theStaticMetadata);
    
    virtual TypePtr getSuperType() override;

    virtual AnyValuePtr lookupSelector(const AnyValuePtr &selector) override;

protected:
    const StaticBootstrapDefinedTypeMetadata *staticMetadata;
    TypePtr supertype;

    std::unordered_map<AnyValuePtr, AnyValuePtr> methodDictionary;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_BOOTSTRAP_TYPE_HPP