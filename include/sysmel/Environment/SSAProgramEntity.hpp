#ifndef SYSMEL_ENVIRONMENT_SSA_PROGRAM_ENTITY_HPP
#define SYSMEL_ENVIRONMENT_SSA_PROGRAM_ENTITY_HPP
#pragma once

#include "SSAGlobalValue.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAProgramEntity : public SubtypeOf<SSAGlobalValue, SSAProgramEntity>
{
public:
    static constexpr char const __typeName__[] = "SSAProgramEntity";

    virtual bool isSSAProgramEntity() const override;

    virtual AnyValuePtr getName() const override;
    void setName(const AnyValuePtr &newName);

    virtual void addChild(const SSAProgramEntityPtr &child);

protected:
    AnyValuePtr name;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_PROGRAM_ENTITY_HPP