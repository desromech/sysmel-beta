#ifndef SYSMEL_ENVIRONMENT_PRIMITIVE_TYPE_HPP
#define SYSMEL_ENVIRONMENT_PRIMITIVE_TYPE_HPP
#pragma once

#include "AnyValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveType);

/**
 * I am primitive type. I typically have direct support at the CPU level.
 */
class SYSMEL_COMPILER_LIB_EXPORT PrimitiveType : public SubtypeOf<AnyValue, PrimitiveType>
{
public:
    static constexpr char const __typeName__[] = "PrimitiveType";
    static constexpr char const __sysmelTypeName__[] = "_PrimitiveType";

    static constexpr bool __isDynamicCompileTimeType__ = false;
    static constexpr bool __isNullableType__ = false;
    static constexpr bool __isImmutableType__ = true;

    virtual bool isPrimitiveTypeValue() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PRIMITIVE_TYPE_HPP