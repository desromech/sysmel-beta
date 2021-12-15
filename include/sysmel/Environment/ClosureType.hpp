#ifndef SYSMEL_ENVIRONMENT_CLOSURE_TYPE_HPP
#define SYSMEL_ENVIRONMENT_CLOSURE_TYPE_HPP
#pragma once

#include "FunctionalType.hpp"

namespace Sysmel
{
namespace Environment
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
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;

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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_CLOSURE_TYPE_HPP