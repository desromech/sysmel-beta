#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_UNDEFINED_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_UNDEFINED_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralUndefined);


/**
 * I am literal number value.
 */
class LiteralUndefined : public SubtypeOf<LiteralValue, LiteralUndefined>
{
public:
    static constexpr char const __typeName__[] = "LiteralUndefined";

    static LiteralUndefinedPtr uniqueInstance();

    virtual bool isLiteralUndefined() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_UNDEFINED_HPP