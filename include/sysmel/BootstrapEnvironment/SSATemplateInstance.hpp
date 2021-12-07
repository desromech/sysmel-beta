#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_TEMPLATE_INSTANCE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_TEMPLATE_INSTANCE_HPP
#pragma once

#include "SSAProgramEntityWithChildren.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSATemplateInstance : public SubtypeOf<SSAProgramEntityWithChildren, SSATemplateInstance>
{
public:
    static constexpr char const __typeName__[] = "SSATemplateInstance";

    virtual bool isSSATemplateInstance() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_TEMPLATE_INSTANCE_HPP