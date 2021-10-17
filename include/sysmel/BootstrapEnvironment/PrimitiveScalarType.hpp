#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_SCALAR_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_SCALAR_TYPE_HPP
#pragma once

#include "PrimitiveTensorType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveScalarType);

/**
 * I am a single scalar primitive type.
 */
class PrimitiveScalarType : public SubtypeOf<PrimitiveTensorType, PrimitiveScalarType>
{
public:
    static constexpr char const __typeName__[] = "PrimitiveScalarType";
    static constexpr char const __sysmelTypeName__[] = "_PrimitiveScalarType";

    virtual bool isPrimitiveScalarTypeValue() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_SCALAR_TYPE_HPP