#ifndef SYSMEL_ENVIRONMENT_PRIMITIVE_NUMBER_TYPE_HPP
#define SYSMEL_ENVIRONMENT_PRIMITIVE_NUMBER_TYPE_HPP
#pragma once

#include "PrimitiveScalarType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveNumberType);

/**
 * I am a single scalar number type.
 */
class PrimitiveNumberType : public SubtypeOf<PrimitiveScalarType, PrimitiveNumberType>
{
public:
    static constexpr char const __typeName__[] = "PrimitiveNumberType";
    static constexpr char const __sysmelTypeName__[] = "_PrimitiveNumberType";

    virtual bool isPrimitiveNumberTypeValue() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PRIMITIVE_NUMBER_TYPE_HPP