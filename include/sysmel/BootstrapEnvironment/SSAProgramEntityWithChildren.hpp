#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_PROGRAM_ENTITY_WITH_CHILDREN_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_PROGRAM_ENTITY_WITH_CHILDREN_HPP
#pragma once

#include "SSAProgramEntity.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAProgramEntityWithChildren : public SubtypeOf<SSAProgramEntity, SSAProgramEntityWithChildren>
{
public:
    static constexpr char const __typeName__[] = "SSAProgramEntityWithChildren";

    virtual void addChild(const SSAProgramEntityPtr &child) override;

protected:
    SSAProgramEntityPtrList children;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_PROGRAM_ENTITY_WITH_CHILDREN_HPP