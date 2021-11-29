#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_FUNCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_FUNCTION_HPP
#pragma once

#include "SSAGlobalValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAFunction : public SubtypeOf<SSAGlobalValue, SSAFunction>
{
public:
    static constexpr char const __typeName__[] = "SSAFunction";

    virtual bool isSSAFunction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
    virtual SExpression asFullSExpression() const override;

    virtual AnyValuePtr getName() const override;
    virtual TypePtr getValueType() const override;
    const FunctionalTypePtr &getFunctionalType();

    void initializeWithNameAndType(const AnyValuePtr &newName, const FunctionalTypePtr &newFunctionalType);

protected:
    AnyValuePtr name;
    FunctionalTypePtr functionalType;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_FUNCTION_HPP