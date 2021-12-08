#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_NEGATIVE_INTEGER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_NEGATIVE_INTEGER_HPP
#pragma once

#include "LiteralInteger.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralNegativeInteger);

/**
 * I am a literal negative integer value.
 */
class LiteralNegativeInteger : public SubtypeOf<LiteralInteger, LiteralNegativeInteger>
{
public:
    static constexpr char const __typeName__[] = "LiteralNegativeInteger";
    static constexpr char const __sysmelTypeName__[] = "LiteralNegativeInteger";

    virtual bool isLiteralNegativeInteger() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_NEGATIVE_INTEGER_HPP