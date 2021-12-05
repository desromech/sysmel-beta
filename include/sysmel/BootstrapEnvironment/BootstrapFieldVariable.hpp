#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_FIELD_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_FIELD_VARIABLE_HPP
#pragma once

#include "FieldVariable.hpp"
#include "Wrappers.hpp"
#include "ReferenceType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(BootstrapFieldVariableBase);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAFunction);

/**
 * I am the base interface for a type that is specifically defined by the bootstrap environment.
 */
class BootstrapFieldVariableBase : public SubtypeOf<FieldVariable, BootstrapFieldVariableBase>
{
public:
    static constexpr char const __typeName__[] = "BootstrapFieldVariable";

    BootstrapFieldVariableBase();
    
    virtual bool isBootstrapFieldVariable() const override;

    virtual AnyValuePtr getBootstrapFieldReferenceForWithType(const AnyValuePtr &receiver, const TypePtr &referenceType) override;

    virtual AnyValuePtr loadFieldFrom(const AnyValuePtr &receiver);
    virtual void storeFieldValueIn(const AnyValuePtr &value, const AnyValuePtr &receiver);
};

template<typename T>
class BootstrapFieldVariable;

template<typename CT, typename MT>
class BootstrapFieldVariable<MT CT::*> : public BootstrapFieldVariableBase
{
public:
    typedef MT MemberType;
    typedef CT ClassType;
    typedef MT CT::*MemberPointerType;
    typedef BootstrapFieldVariable<MT CT::*> SelfType;

    BootstrapFieldVariable(const AnyValuePtr &initialName, MemberPointerType initialMemberPointer)
        : memberPointer(initialMemberPointer)
    {
        name = initialName;
        valueType = wrapperTypeFor<MemberType> ();
        referenceType = valueType->ref();
    }

    virtual AnyValuePtr loadFieldFrom(const AnyValuePtr &receiver) override
    {
        auto selfPointer = reinterpret_cast<ClassType*> (receiver.get());
        return wrapValue(selfPointer->*memberPointer);
    }
    
    virtual void storeFieldValueIn(const AnyValuePtr &value, const AnyValuePtr &receiver) override
    {
        auto selfPointer = reinterpret_cast<ClassType*> (receiver.get());
        selfPointer->*memberPointer = unwrapValue<MemberType> (value);
    }

private:
    MemberPointerType memberPointer;
};

template<typename CT, typename MT>
BootstrapFieldVariableBasePtr makeBootstrapFieldVariable(const std::string &name, MT CT::*memberPointer)
{
    auto nameSymbol = internSymbol(name);
    return basicMakeObject<BootstrapFieldVariable<MT CT::*>> (nameSymbol, memberPointer);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_FIELD_VARIABLE_HPP