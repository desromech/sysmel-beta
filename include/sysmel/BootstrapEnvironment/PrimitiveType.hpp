#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_TYPE_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveType);

/**
 * I am primitive type. I typically have direct support at the CPU level.
 */
class PrimitiveType : public SubtypeOf<AnyValue, PrimitiveType>
{
public:
    static constexpr char const __typeName__[] = "PrimitiveType";
    static constexpr char const __sysmelTypeName__[] = "_PrimitiveType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isPrimitiveTypeValue() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_TYPE_HPP