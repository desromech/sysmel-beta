#ifndef SYSMEL_ENVIRONMENT_LITERAL_POSITIVE_INTEGER_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_POSITIVE_INTEGER_HPP
#pragma once

#include "LiteralInteger.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralPositiveInteger);

/**
 * I am a literal positive integer value.
 */
class SYSMEL_COMPILER_LIB_EXPORT LiteralPositiveInteger : public SubtypeOf<LiteralInteger, LiteralPositiveInteger>
{
public:
    static constexpr char const __typeName__[] = "LiteralPositiveInteger";
    static constexpr char const __sysmelTypeName__[] = "LiteralPositiveInteger";

    virtual bool isLiteralPositiveInteger() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_POSITIVE_INTEGER_HPP