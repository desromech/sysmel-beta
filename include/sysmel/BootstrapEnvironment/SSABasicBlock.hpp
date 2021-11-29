#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BASIC_BLOCK_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BASIC_BLOCK_HPP
#pragma once

#include "SSAValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSABasicBlock : public SubtypeOf<SSAValue, SSABasicBlock>
{
public:
    static constexpr char const __typeName__[] = "SSABasicBlock";

    virtual bool isSSABasicBlock() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
    virtual SExpression asFullSExpression() const override;

    const SSACodeRegionPtr &getParentCodeRegion() const;
    void setParentCodeRegion(const SSACodeRegionPtr &newParent);

protected:
    SSAInstructionPtrList instructions;
    SSACodeRegionPtr parentCodeRegion;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BASIC_BLOCK_HPP