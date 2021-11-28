#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_TYPE_HPP
#pragma once

#include "FunctionalType.hpp"
#include "SpecificMethod.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(MethodType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MethodTypeValue);

/**
 * I am an instance of a function type object.
 */
class MethodType : public SubMetaTypeOf<FunctionalType, MethodType>
{
public:
    virtual bool isMethodType() const override;

    static MethodTypePtr make(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &arguments);

    virtual TypePtr getReceiverType() const override;
    virtual FunctionalTypeValuePtr makeValueWithImplementation(const AnyValuePtr &implementation) override;
    
    virtual std::string printString() const override;

protected:
    TypePtr receiverType;
};

/**
 * I am an instance of a function type object.
 */
class MethodTypeValue : public SubtypeOf<FunctionalTypeValue, MethodTypeValue>
{
public:
    virtual bool isMethodTypeValue() const override;

    virtual TypePtr getType() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_TYPE_HPP