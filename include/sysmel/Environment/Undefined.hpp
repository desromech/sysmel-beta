#ifndef SYSMEL_ENVIRONMENT_UNDEFINED_HPP
#define SYSMEL_ENVIRONMENT_UNDEFINED_HPP
#pragma once

#include "AnyValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Undefined);


/**
 * I am literal number value.
 */
class Undefined : public SubtypeOf<AnyValue, Undefined>
{
public:
    static constexpr char const __typeName__[] = "Undefined";
    static constexpr char const __sysmelTypeName__[] = "Undefined";
    static constexpr bool __isDynamicCompileTimeType__ = false;

    static UndefinedPtr uniqueInstance();

    virtual bool isUndefined() const override;
    virtual bool isAnonymousNameSymbol() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_UNDEFINED_HPP