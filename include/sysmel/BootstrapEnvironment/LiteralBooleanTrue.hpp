#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_TRUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_TRUE_HPP
#pragma once

#include "LiteralBoolean.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralBooleanTrue);


/**
 * I am literal number value.
 */
class LiteralBooleanTrue : public SubtypeOf<LiteralBoolean, LiteralBooleanTrue>
{
public:
    static constexpr char const __typeName__[] = "LiteralBooleanTrue";

    static LiteralBooleanTruePtr uniqueInstance();

    virtual bool isLiteralBooleanTrue() const override;

    virtual bool unwrapAsBoolean() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_TRUE_HPP