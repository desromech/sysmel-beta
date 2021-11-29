#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLOSURE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLOSURE_TYPE_HPP
#pragma once

#include "FunctionalType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClosureType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClosureTypeValue);

/**
 * I am an instance of a function type object.
 */
class ClosureType : public SubMetaTypeOf<FunctionalType, ClosureType>
{
public:
    virtual bool isClosureType() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    static ClosureTypePtr make(const TypePtr &resultType, const TypePtrList &arguments);
    
    virtual FunctionalTypeValuePtr makeValueWithEnvironmentAndImplementation(const AnyValuePtr &environment, const AnyValuePtr &implementation) override;
};

/**
 * I am an instance of a function type object.
 */
class ClosureTypeValue : public SubtypeOf<FunctionalTypeValue, ClosureTypeValue>
{
public:
    virtual bool isClosureTypeValue() const override;

    virtual TypePtr getType() const override;

    virtual AnyValuePtr applyWithArguments(const std::vector<AnyValuePtr> &arguments) override;

    AnyValuePtr environment;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLOSURE_TYPE_HPP