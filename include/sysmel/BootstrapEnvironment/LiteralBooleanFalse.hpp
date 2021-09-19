#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_FALSE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_FALSE_HPP
#pragma once

#include "LiteralBoolean.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

class LiteralBooleanFalse;
typedef std::shared_ptr<LiteralBooleanFalse> LiteralBooleanFalsePtr;


/**
 * I am literal number value.
 */
class LiteralBooleanFalse : public SubtypeOf<LiteralBoolean, LiteralBooleanFalse>
{
public:
    static constexpr char const __typeName__[] = "LiteralBooleanFalse";

    static LiteralBooleanFalsePtr uniqueInstance();

    virtual bool isLiteralBooleanFalse() const override;
    
    virtual bool unwrapAsBoolean() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_FALSE_HPP