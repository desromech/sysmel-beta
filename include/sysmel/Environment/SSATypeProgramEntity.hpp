#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_TYPE_PROGRAM_ENTITY_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_TYPE_PROGRAM_ENTITY_HPP
#pragma once

#include "SSAProgramEntityWithChildren.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSATypeProgramEntity : public SubtypeOf<SSAProgramEntityWithChildren, SSATypeProgramEntity>
{
public:
    static constexpr char const __typeName__[] = "SSATypeProgramEntity";

    virtual bool isSSATypeProgramEntity() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    const TypePtr &getValue() const;
    void setValue(const TypePtr &newValue);

    virtual TypePtr getValueType() const override;

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;

protected:
    TypePtr value;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_TYPE_PROGRAM_ENTITY_HPP