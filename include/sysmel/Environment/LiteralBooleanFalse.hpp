#ifndef SYSMEL_ENVIRONMENT_LITERAL_BOOLEAN_FALSE_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_BOOLEAN_FALSE_HPP
#pragma once

#include "LiteralBoolean.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralBooleanFalse);


/**
 * I am literal number value.
 */
class LiteralBooleanFalse : public SubtypeOf<LiteralBoolean, LiteralBooleanFalse>
{
public:
    static constexpr char const __typeName__[] = "LiteralBooleanFalse";
    static constexpr char const __sysmelTypeName__[] = "LiteralBooleanFalse";

    static LiteralBooleanFalsePtr uniqueInstance();

    virtual bool isLiteralBooleanFalse() const override;
    
    virtual bool unwrapAsBoolean() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_BOOLEAN_FALSE_HPP