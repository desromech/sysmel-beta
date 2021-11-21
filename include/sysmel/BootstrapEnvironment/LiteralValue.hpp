#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_VALUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_VALUE_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralValue);

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class LiteralValue : public SubtypeOf<AnyValue, LiteralValue>
{
public:
    static constexpr char const __typeName__[] = "LiteralValue";
    static constexpr char const __sysmelTypeName__[] = "LiteralValue";

    static void __addTypeConversionRules__(const TypePtr &type);

    virtual bool isLiteralValue() const override;
    virtual bool isPureCompileTimeLiteralValue() const override;

    virtual bool canBeConcretizedWithType(const TypePtr &targetType);
    virtual AnyValuePtr concretizeWithType(const TypePtr &targetType);

    virtual TypePtr getStandardConcreteType();
    virtual TypePtr getBestConcreteType();
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_VALUE_HPP