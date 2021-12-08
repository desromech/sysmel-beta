#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_MODULE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_MODULE_HPP
#pragma once

#include "SSAProgramEntity.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAModule : public SubtypeOf<SSAProgramEntity, SSAModule>
{
public:
    static constexpr char const __typeName__[] = "SSAModule";

    virtual bool isSSAModule() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;

    AnyValuePtr getName() const;
    void setName(const AnyValuePtr &newName);

    const SSAValuePtr &getGlobalNamespace() const;
    void setGlobalNamespace(const SSAValuePtr &newGlobalNamespace);

private:
    AnyValuePtr name;
    SSAValuePtr globalNamespace;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_MODULE_HPP