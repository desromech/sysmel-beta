#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_META_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_META_TYPE_HPP
#pragma once

#include "Type.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the base interface for all of the types that are defined in the system.
 */
class MetaType : public SubtypeOf<Type, MetaType>
{
public:
    static constexpr char const __typeName__[] = "MetaType";

    virtual void setThisType(const TypePtr &instanceType);

    /// Is this type a meta type?.
    virtual bool isMetaType() const override;

    /// This method returns the instance type.
    virtual TypePtr getInstanceType() override;

    /// This method returns the meta type.
    virtual TypePtr getMetaType() override;

    std::string printString() const override;
protected:
    TypePtr thisType;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_META_TYPE_HPP