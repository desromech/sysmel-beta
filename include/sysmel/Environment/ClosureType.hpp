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
class SYSMEL_COMPILER_LIB_EXPORT ClosureType : public SubMetaTypeOf<FunctionalType, ClosureType>
{
public:
    virtual bool isClosureType() const override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;

    virtual std::string getQualifiedName() const override;
    virtual SExpression asSExpression() const override;

    static ClosureTypePtr make(const TypePtr &resultType, const TypePtrList &arguments);
    
    virtual FunctionalTypeValuePtr makeValueWithEnvironmentAndImplementation(const AnyValuePtr &environment, const AnyValuePtr &implementation) override;
};

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT ClosureTypeValue : public SubtypeOf<FunctionalTypeValue, ClosureTypeValue>
{
public:
    static constexpr char const __typeName__[] = "ClosureType";
    static constexpr char const __sysmelTypeName__[] = "_ClosureType";

    virtual bool isClosureTypeValue() const override;

    virtual TypePtr getType() const override;

    virtual AnyValuePtr applyWithArguments(const std::vector<AnyValuePtr> &arguments) override;

    AnyValuePtr environment;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_CLOSURE_TYPE_HPP