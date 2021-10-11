#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VOID_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VOID_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Void);

/**
 * I am literal number value.
 */
class Void : public SubtypeOf<AnyValue, Void>
{
public:
    static constexpr char const __typeName__[] = "Void";
    static constexpr char const __sysmelTypeName__[] = "Void";
    static constexpr bool __isDynamicCompileTimeType__ = false;
    
    static VoidPtr uniqueInstance();

    virtual bool isVoid() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VOID_HPP