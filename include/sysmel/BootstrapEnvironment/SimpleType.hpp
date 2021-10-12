#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SIMPLE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SIMPLE_TYPE_HPP
#pragma once

#include "Type.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the base implementation for a simple type
 */
class SimpleType : public SubtypeOf<Type, SimpleType>
{
public:
    static constexpr char const __typeName__[] = "SimpleType";

    virtual bool isSimpleType() const override;

    virtual TypePtr getType() const override;
    void setType(const TypePtr &theMetaType);

protected:
    TypePtr metaType;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SIMPLE_TYPE_HPP