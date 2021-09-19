#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_NEGATIVE_INTEGER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_NEGATIVE_INTEGER_HPP
#pragma once

#include "LiteralInteger.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralNegativeInteger);

/**
 * I am a literal negative integer value.
 */
class LiteralNegativeInteger : public SubtypeOf<LiteralInteger, LiteralNegativeInteger>
{
public:
    static constexpr char const __typeName__[] = "LiteralNegativeInteger";

    virtual bool isLiteralNegativeInteger() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_NEGATIVE_INTEGER_HPP