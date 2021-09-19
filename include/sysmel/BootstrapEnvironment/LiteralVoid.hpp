#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_VOID_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_VOID_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralVoid);

/**
 * I am literal number value.
 */
class LiteralVoid : public SubtypeOf<LiteralValue, LiteralVoid>
{
public:
    static constexpr char const __typeName__[] = "LiteralVoid";
    
    static LiteralVoidPtr uniqueInstance();

    virtual bool isLiteralVoid() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_VOID_HPP