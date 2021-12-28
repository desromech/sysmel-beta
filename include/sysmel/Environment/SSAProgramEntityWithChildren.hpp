#ifndef SYSMEL_ENVIRONMENT_SSA_PROGRAM_ENTITY_WITH_CHILDREN_HPP
#define SYSMEL_ENVIRONMENT_SSA_PROGRAM_ENTITY_WITH_CHILDREN_HPP
#pragma once

#include "SSAProgramEntity.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAProgramEntityWithChildren : public SubtypeOf<SSAProgramEntity, SSAProgramEntityWithChildren>
{
public:
    static constexpr char const __typeName__[] = "SSAProgramEntityWithChildren";

    virtual void addChild(const SSAProgramEntityPtr &child) override;
    const SSAProgramEntityPtrList &getChildren() const;

protected:
    SSAProgramEntityPtrList children;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_PROGRAM_ENTITY_WITH_CHILDREN_HPP