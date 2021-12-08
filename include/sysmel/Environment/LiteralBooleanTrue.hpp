#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_TRUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_TRUE_HPP
#pragma once

#include "LiteralBoolean.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralBooleanTrue);


/**
 * I am literal number value.
 */
class LiteralBooleanTrue : public SubtypeOf<LiteralBoolean, LiteralBooleanTrue>
{
public:
    static constexpr char const __typeName__[] = "LiteralBooleanTrue";
    static constexpr char const __sysmelTypeName__[] = "LiteralBooleanTrue";

    static LiteralBooleanTruePtr uniqueInstance();

    virtual bool isLiteralBooleanTrue() const override;

    virtual bool unwrapAsBoolean() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_TRUE_HPP