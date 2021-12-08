#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_NAMESPACE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_NAMESPACE_HPP
#pragma once

#include "SSAProgramEntityWithChildren.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSANamespace : public SubtypeOf<SSAProgramEntityWithChildren, SSANamespace>
{
public:
    static constexpr char const __typeName__[] = "SSANamespace";

    virtual bool isSSANamespace() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_NAMESPACE_HPP