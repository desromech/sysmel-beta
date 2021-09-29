#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_UNDEFINED_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_UNDEFINED_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Undefined);


/**
 * I am literal number value.
 */
class Undefined : public SubtypeOf<AnyValue, Undefined>
{
public:
    static constexpr char const __typeName__[] = "Undefined";

    static UndefinedPtr uniqueInstance();

    virtual bool isUndefined() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_UNDEFINED_HPP