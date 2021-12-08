#ifndef SYSMEL_ENVIRONMENT_PRIMITIVE_SCALAR_TYPE_HPP
#define SYSMEL_ENVIRONMENT_PRIMITIVE_SCALAR_TYPE_HPP
#pragma once

#include "PrimitiveTensorType.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PRIMITIVE_SCALAR_TYPE_HPP